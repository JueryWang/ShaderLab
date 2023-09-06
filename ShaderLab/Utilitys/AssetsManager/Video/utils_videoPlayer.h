#ifndef  UTIL_VIDEO_PLAYER_H
#define UTIL_VIDEO_PLAYER_H

#include <QObject>
#include "Utilitys/utils_ffmpegHelper.h"

class XA_VIDEO_PLAYER : public QObject
{
	Q_OBJECT
public:
	static XA_VIDEO_PLAYER* get_player();
	void setRecordParam(const char* name,int width,int height);
	void writeRecord(uchar* frameraw);

public slots:
	void recordDone();

private:
	XA_VIDEO_PLAYER();
	~XA_VIDEO_PLAYER();

private:
	static XA_VIDEO_PLAYER* _instance;

	std::string rec_target_file;
	AVFrame* videoFrame = nullptr;
	AVCodecContext* cctx = nullptr;
	SwsContext* swsCtx = nullptr;
	AVFormatContext* ofctx = nullptr;
	AVOutputFormat* oformat = nullptr;
	int fps = 30;
	int width = 1920;
	int height = 1080;
	int bitrate = 2000;
	int frameCounter;
};

#endif