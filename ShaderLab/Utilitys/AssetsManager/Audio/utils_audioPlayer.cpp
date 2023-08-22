#include "utils_audioPlayer.h"
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
	QSound::play(au_src);
}

XA_AUDIO_PLAYER::XA_AUDIO_PLAYER()
{
	
}

XA_AUDIO_PLAYER::~XA_AUDIO_PLAYER()
{

}

