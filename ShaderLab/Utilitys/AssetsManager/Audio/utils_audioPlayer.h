#ifndef  UTIL_AUDIO_PLAYER_H
#define UTIL_AUDIO_PLAYER_H

#include <QObject>
#include <QThread>

class XA_AUDIO_PLAYER : public QObject
{
public:
	static XA_AUDIO_PLAYER* get_player();
	void paly(const QString& au_src);
	void pause();
	void rewind();
	void mute();
	void quit();
	void resume();
private:
	XA_AUDIO_PLAYER();
	~XA_AUDIO_PLAYER();

private:
	static XA_AUDIO_PLAYER* _instance;
};

#endif

