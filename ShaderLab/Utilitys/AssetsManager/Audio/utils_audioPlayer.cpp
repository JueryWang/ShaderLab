#include "utils_audioPlayer.h"
#include "../../utils_backendThread.h"
#include <QSound>
#include <QDebug>
XA_AUDIO_PLAYER* XA_AUDIO_PLAYER::_instance = nullptr;

XA_AUDIO_PLAYER* XA_AUDIO_PLAYER::get_player()
{
	if (!_instance)
	{
		_instance = new XA_AUDIO_PLAYER();
	}
	return _instance;
}

void XA_AUDIO_PLAYER::paly(const QString& au_src)
{

	std::pair<QObject*, XA_UTILS_TASK> audio_playTask;
	audio_playTask.first = this;
	audio_playTask.second.type = XA_UTIL_PLAYAUDIO;
	QByteArray s = au_src.toLatin1();
	strcpy(audio_playTask.second.param.playAudio_pram.audio_path, s.data());
	printf("audio path = %s\n", audio_playTask.second.param.playAudio_pram.audio_path);
	XA_UTILS_BACKEND::getUtilBackend()->addTask(audio_playTask);
}

XA_AUDIO_PLAYER::XA_AUDIO_PLAYER()
{
	
}

XA_AUDIO_PLAYER::~XA_AUDIO_PLAYER()
{

}

