/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef UITLS_BACKEND_THREAD_H
#define UITLS_BACKEND_THREAD_H
#include <QThread>
#include <QAudioFormat>
#include <QAudioOutput>
#include <utility>
#include <queue>
#include "utilsDfs.h"

class XA_UTILS_BACKEND : public QThread
{
	friend class XA_AUDIO_PLAYER;
	Q_OBJECT
public:
	static XA_UTILS_BACKEND* getUtilBackend();
	static void setCachePath(const char* path);
	static void cleanCache();
	void addTask(std::pair<QObject*, XA_UTILS_TASK> newTask);
	void setAuVolume(float volume);

signals:
	void audioplayDone();

protected:
	void run() override;

private:
	XA_UTILS_BACKEND();
	~XA_UTILS_BACKEND();
	void handlePlayAudio(const std::pair<QObject*, XA_UTILS_TASK> &crt_task);
	void handleGifDecode(const std::pair<QObject*, XA_UTILS_TASK>& crt_task);
	void AudioPlay();
	void AudioPause();
	void AudioRewind();
	void AudioMute();
	void AudioQuit();
	void AudioResume();
	void AudioReplay();
	void inline raiseVolume(char* buf,int len,float factor);

private:
	static XA_UTILS_BACKEND* _instance;
	static const char* cache_path;
	static char default_au_outputFile[256];
	
	std::atomic<XA_AUDIO_STATE> au_state;
	std::atomic<float> crt_auVolume = 1.0;
	QAudioFormat audio_format;
	QAudioOutput *crt_audioOut = NULL;
	int audio_periodsz;
	char* audio_buf;

	QIODevice* crt_IO;

	FILE* audio_fileHandler = NULL;
	std::queue<std::pair<QObject*, XA_UTILS_TASK>> _task_queue;
};


#endif
