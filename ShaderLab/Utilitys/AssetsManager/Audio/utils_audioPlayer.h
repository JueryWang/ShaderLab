/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef  UTIL_AUDIO_PLAYER_H
#define UTIL_AUDIO_PLAYER_H

#include <QObject>
#include <QThread>

class XA_UIMODULE_ASSET_WINDOW;

class XA_AUDIO_PLAYER : public QObject
{
	Q_OBJECT
public:
	static XA_AUDIO_PLAYER* get_player();
	void play(const QString& au_src);
	void pause();
	void rewind();
	void mute();
	void quit();
	void resume();
	void replay();


public slots:
	void playDone();

private:
	XA_AUDIO_PLAYER();
	~XA_AUDIO_PLAYER();

public:
	XA_UIMODULE_ASSET_WINDOW* _invoker = nullptr;
private:
	static XA_AUDIO_PLAYER* _instance;
};

#endif

