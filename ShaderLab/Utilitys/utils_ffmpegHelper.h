#ifndef UTILS_FFMPEG_HELPER_H
#define UTILS_FFMPEG_HELPER_H

#include <QObject>
#include <QMap>
#include "utilsDfs.h"
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/time.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}


class XA_FFMPEG_HELPER : QObject
{
public:
	static XA_FFMPEG_HELPER* getHelper();
	void XA_FFMPEG_HELPER::disableLog(){	av_log_set_callback(nullptr);	}
	void getAudioInfo(utils_ffmpeg::XA_FFMPEG_AU_INFO* info, const char* src);
	void setAVRecordParam(const std::string& filename, int width, int height);
	void pushFrame(uchar* frameraw);
	void AVRecordDone();
	
private:
	XA_FFMPEG_HELPER();
	~XA_FFMPEG_HELPER();
	void init_context();
	void finish();
	void free();

private:
	static XA_FFMPEG_HELPER* _instance;

	std::string rec_target_file;
	AVFrame* videoFrame = nullptr;
	AVCodecContext* cctx = nullptr;
	SwsContext* swsCtx = nullptr;
	AVFormatContext* ofctx = nullptr;
	AVOutputFormat* oformat = nullptr;
	int fps = 30;
	int width = 1920;
	int height = 1080;
	int bitrate = 3000;
	int frameCounter;
};
#endif
