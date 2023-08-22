#ifndef UTILS_FFMPEG_HELPER_H
#define UTILS_FFMPEG_HELPER_H

#include <QObject>
#include <QMap>

extern "C"
{
#include <libavformat/avformat.h>
}

class XA_FFMPEG_HELPER : QObject
{
public:
	static XA_FFMPEG_HELPER* getHelper();
	QMap<QString, int> getAudioInfo(const QString& src);

private:
	XA_FFMPEG_HELPER();
	~XA_FFMPEG_HELPER();

private:
	static XA_FFMPEG_HELPER* _instance;
};
#endif
