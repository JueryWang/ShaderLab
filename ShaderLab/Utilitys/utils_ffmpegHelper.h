#ifndef UTILS_FFMPEG_HELPER_H
#define UTILS_FFMPEG_HELPER_H

#include <QObject>
#include <QMap>
#include "utilsDfs.h"
extern "C"
{
#include <libavformat/avformat.h>
}

class XA_FFMPEG_HELPER : QObject
{
public:
	static XA_FFMPEG_HELPER* getHelper();
	void getAudioInfo(utils_ffmpeg::XA_FFMPEG_AU_INFO* info, const char* src);

private:
	XA_FFMPEG_HELPER();
	~XA_FFMPEG_HELPER();
private:
	static XA_FFMPEG_HELPER* _instance;
};
#endif
