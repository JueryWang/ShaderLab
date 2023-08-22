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

protected:
	void run() override;

private:
	XA_UTILS_BACKEND();
	~XA_UTILS_BACKEND();

private:
	static XA_UTILS_BACKEND* _instance;

	QAudioFormat audio_format;
	FILE* audio_fileHandler = NULL;
	std::queue<std::pair<QObject*, XA_UTILS_TASK>> _task_queue;
};


#endif
