#include "utils_backendThread.h"
#include "utils_ffmpegHelper.h"
#include "AssetsManager/Audio/utils_audioPlayer.h"
#ifdef Q_OS_WIN
#include <io.h>
#else
#include <unistd.h>
#endif // Q_OS_WIN
#include <QMutex>
#include <QDir>
#include <QDebug>
#include "utilsDfs.h"
using namespace utils_ffmpeg;
using namespace std;

XA_UTILS_BACKEND* XA_UTILS_BACKEND::_instance = nullptr;
const char* XA_UTILS_BACKEND::cache_path;
char XA_UTILS_BACKEND::default_au_outputFile[256];
static QMutex taskLocker;

XA_UTILS_BACKEND* XA_UTILS_BACKEND::getUtilBackend()
{
	if (!_instance)
	{
		_instance = new XA_UTILS_BACKEND();
	}
	return _instance;
}

void XA_UTILS_BACKEND::setCachePath(const char* path)
{
	QDir dir_helper;
	if (!dir_helper.exists(path))
	{
		dir_helper.mkdir(path);
	}
	XA_UTILS_BACKEND::cache_path = path;
	strcpy(default_au_outputFile, XA_UTILS_BACKEND::cache_path);
	strcat(default_au_outputFile, "/out.pcm");
}

void XA_UTILS_BACKEND::cleanCache()
{
	QDir dir(QDir::currentPath() + "/" + cache_path);
	dir.removeRecursively();
}

void XA_UTILS_BACKEND::addTask(std::pair<QObject*, XA_UTILS_TASK> newTask)
{
	taskLocker.lock();
	_task_queue.push(newTask);
	taskLocker.unlock();
}

void XA_UTILS_BACKEND::setAuVolume(float volume)
{
	crt_auVolume.store(volume, memory_order_release);
}

void XA_UTILS_BACKEND::run()
{
	while (true)
	{
		while (!_task_queue.empty())
		{
			if (taskLocker.try_lock())
			{
				std::pair<QObject*, XA_UTILS_TASK> crt_task = _task_queue.front();
				switch (crt_task.second.type)
				{
				case XA_UTIL_PLAYAUDIO:
				{
					handlePlayAudio(crt_task);
					break;
				}
				}
				_task_queue.pop();
				taskLocker.unlock();
			}
			QThread::usleep(1);//reserved time to add new task in main thread 
		}
		QThread::msleep(1);
	}
}

XA_UTILS_BACKEND::XA_UTILS_BACKEND()
{

}

XA_UTILS_BACKEND::~XA_UTILS_BACKEND()
{
	delete crt_audioOut;
	delete crt_IO;
	delete audio_fileHandler;
}

void XA_UTILS_BACKEND::handlePlayAudio(const std::pair<QObject*, XA_UTILS_TASK>& crt_task)
{
	XA_AUDIO_STATE crt_state = au_state.load(memory_order_consume);
	if (crt_state == AUDIO_ST_DEPRECATED)
	{
		if (audio_fileHandler != NULL)
		{
			fclose(audio_fileHandler);
			delete crt_audioOut;
			delete crt_IO;
			delete audio_buf;
			audio_fileHandler = NULL;
			crt_state = AUDIO_ST_ONPLAY;
			return;
		}
	}

	if (audio_fileHandler == NULL)
	{
		XA_FFMPEG_AU_INFO au_info;

		XA_FFMPEG_HELPER::getHelper()
			->getAudioInfo(&au_info, crt_task.second.param.playAudio_param.audio_path);
		_UTILS_GENERATE_PCM(au_info, crt_task.second.param.playAudio_param.audio_path, default_au_outputFile);
		audio_format.setSampleRate(au_info.sampleRate);
		audio_format.setSampleSize(au_info.sampleSize);
		audio_format.setChannelCount(au_info.channel);
		audio_format.setCodec("audio/pcm");
		audio_format.setByteOrder(QAudioFormat::LittleEndian);
		audio_format.setSampleType(QAudioFormat::UnSignedInt);
		crt_audioOut = new QAudioOutput(audio_format);
		crt_IO = crt_audioOut->start();

		audio_fileHandler = fopen(default_au_outputFile, "rb");
		audio_periodsz = crt_audioOut->periodSize();
		audio_buf = new char[audio_periodsz];
		_task_queue.push(crt_task);
	}
	else
	{
		if (!feof(audio_fileHandler))
		{
			if (crt_audioOut->bytesFree() < audio_periodsz)
			{
				_task_queue.push(crt_task);
				return;
			}
			int len = 0;
			if(crt_state != AUDIO_ST_PAUSE)
				len = fread(audio_buf, 1, audio_periodsz, audio_fileHandler);

			if (len <= 0)
			{
				emit audioplayDone();
				return;
			}

			float factor = crt_auVolume.load(memory_order_consume);
			if (abs(factor - 1.0) > eps) //remap pcm data to certain volume level
			{
				raiseVolume(audio_buf, audio_periodsz, factor);
			}

			switch (crt_state)
			{
				case AUDIO_ST_PAUSE:
					//no need to push this task again stop reading audio
					return;
					break;
				case AUDIO_ST_REWIND:
					rewind(audio_fileHandler);
					au_state.store(AUDIO_ST_ONPLAY, memory_order_release);
					_task_queue.push(crt_task);
					break;
				case AUDIO_ST_MUTE:
					_task_queue.push(crt_task);//don't write to Sound output,so there is no Sound
					break;
				default://resume | onplay
					crt_IO->write(audio_buf, len);
					_task_queue.push(crt_task);
					break;
			}
			
		}
		else//play done
		{
			XA_AUDIO_PLAYER::get_player()->playDone();
			if (audio_fileHandler != NULL)
			{
				fclose(audio_fileHandler);
				delete crt_audioOut;
				delete crt_IO;
				delete audio_buf;
				audio_fileHandler = NULL;
			}
		}
	}
}

void XA_UTILS_BACKEND::AudioPlay()
{
	au_state.store(AUDIO_ST_ONPLAY, memory_order_release);
}

void XA_UTILS_BACKEND::AudioPause()
{
	au_state.store(AUDIO_ST_PAUSE, memory_order_release);
}

void XA_UTILS_BACKEND::AudioRewind()
{
	au_state.store(AUDIO_ST_REWIND, memory_order_release);
}

void XA_UTILS_BACKEND::AudioMute()
{
	au_state.store(AUDIO_ST_MUTE, memory_order_release);
}

void XA_UTILS_BACKEND::AudioQuit()
{
	au_state.store(AUDIO_ST_DEPRECATED, memory_order_release);
}

void XA_UTILS_BACKEND::AudioResume()
{
	au_state.store(AUDIO_ST_RESUME, memory_order_release);
	std::pair<QObject*, XA_UTILS_TASK> audio_playTask;
	audio_playTask.first = XA_AUDIO_PLAYER::get_player();
	audio_playTask.second.type = XA_UTIL_PLAYAUDIO;
	addTask(audio_playTask);
}

void XA_UTILS_BACKEND::AudioReplay()
{
	au_state = AUDIO_ST_ONPLAY;
	rewind(audio_fileHandler);
	std::pair<QObject*, XA_UTILS_TASK> audio_playTask;
	audio_playTask.first = XA_AUDIO_PLAYER::get_player();
	audio_playTask.second.type = XA_UTIL_PLAYAUDIO;
	addTask(audio_playTask);
}

void XA_UTILS_BACKEND::raiseVolume(char* buf, int len, float factor)
{
	signed short MIN = -0x8000;
	signed short MAX = 0x7FFF;
	signed short low = 0, high = 0, data = 0, maxData = 0, minData = 0;
	for (int i = 0; i < len; i += 2)
	{
		low = buf[i];
		high = buf[i + 1];
		data = low + (high << 8);
		maxData = maxData > data ? maxData : data;
		minData = minData < data ? minData : data;
	}

	signed short maxfactor = maxData != 0 ? MAX / maxData : 1;
	signed short minfactor = minData != 0 ? MIN / minData : 1;
	if(minfactor == 1 || maxfactor == 1)
	{
		return;
	}

	signed short newData = 0;
	for (int i = 0; i < len; i += 2) {
		low = buf[i];
		high = buf[i + 1];
		data = low + (high << 8);
		newData = data * factor + 0.50;
		if (newData < MIN) {
			newData = MIN;
		}
		else if (newData > MAX)
		{
			newData = MAX;
		}
		data = newData & 0xffff;
		buf[i] = data & 0x00ff;
		buf[i + 1] = (data & 0xff00) >> 8;
	}
}
