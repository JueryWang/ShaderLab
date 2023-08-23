#include "utils_backendThread.h"
#include "utils_ffmpegHelper.h"
#ifdef Q_OS_WIN
#include <io.h>
#else
#include <unistd.h>
#endif // Q_OS_WIN
#include <QMutex>
#include <QDir>
#include "utilsDfs.h"
using namespace utils_ffmpeg;

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

void XA_UTILS_BACKEND::run()
{
	while (true)
	{
		if (taskLocker.try_lock())
		{
			while (!_task_queue.empty())
			{
				std::pair<QObject*, XA_UTILS_TASK> crt_task = _task_queue.front();
				switch (crt_task.second.type)
				{
					case XA_UTIL_PLAYAUDIO:
					{
						if (audio_fileHandler == NULL)
						{
							XA_FFMPEG_AU_INFO au_info;

							XA_FFMPEG_HELPER::getHelper()
								->getAudioInfo(&au_info,crt_task.second.param.playAudio_pram.audio_path);
							_UTILS_GENERATE_PCM(au_info, crt_task.second.param.playAudio_pram.audio_path, default_au_outputFile);
							QThread::usleep(10);//wait for detached ffmpeg gen-pcm process completely executed
							audio_format.setSampleRate(au_info.sampleRate);
							audio_format.setSampleSize(au_info.sampleSize);
							audio_format.setChannelCount(au_info.channel);
							audio_format.setByteOrder(QAudioFormat::LittleEndian);
							audio_format.setSampleType(QAudioFormat::UnSignedInt);
							crt_audioOut = new QAudioOutput(audio_format);
							crt_IO = crt_audioOut->start();
							
							audio_fileHandler = fopen(default_au_outputFile, "rb");
							audio_periodsz = crt_audioOut->periodSize();
							audio_buf = new char[audio_periodsz];
						}
						else
						{
							if (!feof(audio_fileHandler))
							{
								if (crt_audioOut->bytesFree() >= audio_periodsz)
								{
									int len = fread(audio_buf, 1, audio_periodsz, audio_fileHandler);
									if (len > 0)
									{
										crt_IO->write(audio_buf, len);
										//not play end yet ,so make task again to the queue tail
										_task_queue.push(crt_task);
									}
								}
							}
							else
							{
								fclose(audio_fileHandler);
								delete crt_audioOut;
								delete crt_IO;
								delete audio_buf;
							}
						}
						break;
					}

				}
				_task_queue.pop();
			}
			taskLocker.unlock();
		}
		QThread::msleep(1);
	}
}

XA_UTILS_BACKEND::XA_UTILS_BACKEND()
{


}

XA_UTILS_BACKEND::~XA_UTILS_BACKEND()
{

}