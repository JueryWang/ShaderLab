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
	Q_OBJECT
public:
	static XA_UTILS_BACKEND* getUtilBackend();
	static void setCachePath(const char* path);
	static void cleanCache();
	void addTask(std::pair<QObject*, XA_UTILS_TASK> newTask);
protected:
	void run() override;

private:
	XA_UTILS_BACKEND();
	~XA_UTILS_BACKEND();
	void handlePlayAudio(const std::pair<QObject*, XA_UTILS_TASK> &crt_task);

private:
	static XA_UTILS_BACKEND* _instance;
	static const char* cache_path;
	static char default_au_outputFile[256];
	
	QAudioFormat audio_format;
	QAudioOutput *crt_audioOut = NULL;
	int audio_periodsz;
	char* audio_buf;

	QIODevice* crt_IO;

	FILE* audio_fileHandler = NULL;
	std::queue<std::pair<QObject*, XA_UTILS_TASK>> _task_queue;
};


#endif
