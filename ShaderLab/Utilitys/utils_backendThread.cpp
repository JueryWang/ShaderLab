#include "utilsDfs.h"
#include "utils_backendThread.h"
#include "utils_ffmpegHelper.h"
#include "libgif/gif_lib.h"
#include "AssetsManager/Audio/utils_audioPlayer.h"
#ifdef Q_OS_WIN
#include <io.h>
#else
#include <unistd.h>
#endif // Q_OS_WIN
#include <QMutex>
#include <QDir>
#include <QImage>
#include <QDebug>
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
	_task_queue.push(newTask);
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
				case XA_UTIL_DECODEGIF:
				{
					handleGifDecode(crt_task);
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
		crt_audioOut->setVolume(1.0);
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
				if(crt_state != AUDIO_ST_PAUSE)
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

void XA_UTILS_BACKEND::handleGifDecode(const std::pair<QObject*, XA_UTILS_TASK>& crt_task)
{
	GifFileType* gif_file = crt_task.second.param.decodeGif_param.gifFile;
	QVector<GifByteType> screen_buf(gif_file->SWidth * gif_file->SHeight);
	QVector<QImage> imgSequence;
	memset(screen_buf.data(), gif_file->SBackGroundColor, sizeof(GifByteType) * screen_buf.size());

	QImage image_temp = QImage(gif_file->SWidth, gif_file->SHeight, QImage::Format_RGB888);
	ColorMapObject* color_map;
	int trans_color = -1;
	
	for (int im = 0; im < gif_file->ImageCount; im++)
	{
		SavedImage* image = &gif_file->SavedImages[im];
		for (ExtensionBlock* ep = image->ExtensionBlocks; ep < image->ExtensionBlocks + image->ExtensionBlockCount; ep++)
		{
			bool last = (ep - image->ExtensionBlocks == (image->ExtensionBlockCount - 1));
			if (ep->Function == GRAPHICS_EXT_FUNC_CODE)
			{
				GraphicsControlBlock gcb;
				if (DGifExtensionToGCB(ep->ByteCount, ep->Bytes, &gcb) == GIF_ERROR) {
					qDebug() << "invalid graphics control block";
					return;
				}
				trans_color = gcb.TransparentColor;
				crt_task.second.param.decodeGif_param.info->interval = gcb.DelayTime * 10;
			}
			else if (!last
				&& ep->Function == APPLICATION_EXT_FUNC_CODE
				&& ep->ByteCount >= 11
				&& (ep + 1)->ByteCount >= 3
				&& memcmp(ep->Bytes, "NETSCAPE2.0", 11) == 0)
			{
				unsigned char* params = (++ep)->Bytes;
				unsigned int loopcount = params[1] | (params[2] << 8);
				qDebug() << "netscape loop " << loopcount;
			}else
			{
				while (!last && ep[1].Function == CONTINUE_EXT_FUNC_CODE)
				{
					++ep;
					last = (ep - image->ExtensionBlocks == (image->ExtensionBlockCount - 1));
				}
			}
		}
		if (image->ImageDesc.Interlace)
			qDebug() << "image interlaced";
		color_map = (image->ImageDesc.ColorMap ? image->ImageDesc.ColorMap : gif_file->SColorMap);
		if (color_map == NULL) {
			qDebug() << "color map error.";
			return;
		}

		//save last frame as P frame
		QImage img = image_temp.copy();
		GifColorType* color_map_entry;
		for (int i = image->ImageDesc.Top; i < image->ImageDesc.Top + image->ImageDesc.Height; i++)
		{
			memcpy(screen_buf.data() + i * gif_file->SWidth + image->ImageDesc.Left,
				image->RasterBits + (i - image->ImageDesc.Top) * image->ImageDesc.Width,
				image->ImageDesc.Width);
			for (int j = image->ImageDesc.Left; j < image->ImageDesc.Left + image->ImageDesc.Width; j++)
			{
				if (trans_color != -1 && trans_color == screen_buf[i * gif_file->SWidth + j])
					continue;
				color_map_entry = &color_map->Colors[screen_buf[i * gif_file->SWidth + j]];
				img.setPixelColor(j, i, QColor(
					color_map_entry->Red,
					color_map_entry->Green,
					color_map_entry->Blue));
			}
		}
		image_temp = img;
		imgSequence.push_back(img);
	}
	/*crt_task.second.param.decodeGif_param.receiver->setGifSequence(imgSequence);*/

	int error_code;
	if (DGifCloseFile(gif_file, &error_code) == GIF_ERROR) {
		const char* gif_error = GifErrorString(error_code);
		qDebug() << "close error:" << gif_error;
	}
	qDebug() << "gif to image finish";
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
