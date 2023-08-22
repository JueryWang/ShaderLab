#include "utils_backendThread.h"
#include "utils_ffmpegHelper.h"
#include <QMutex>

XA_UTILS_BACKEND* XA_UTILS_BACKEND::_instance = nullptr;
static QMutex taskLocker;

XA_UTILS_BACKEND* XA_UTILS_BACKEND::getUtilBackend()
{
	if (!_instance)
	{
		_instance = new XA_UTILS_BACKEND();
	}
	return _instance;
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
							QMap<QString, int> audioInfo = XA_FFMPEG_HELPER::getHelper()
								->getAudioInfo(crt_task.second.param.playAudio_pram.audio_path);
							int sampleRate = audioInfo["sample_rate"];
							int channelCount = audioInfo["channel"];
							int sampleSize = audioInfo["format"] * channelCount;
							char cmd[64];

							audio_format.setSampleRate(audioInfo["sample_rate"]);
							audio_format.setSampleSize(16);
							audio_format.setChannelCount(audioInfo["channel"]);
							audio_format.setCodec("audio/pcm");

						}
						break;
					}

				}
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

