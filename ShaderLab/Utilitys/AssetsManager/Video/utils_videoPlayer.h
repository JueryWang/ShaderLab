/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef  UTIL_VIDEO_PLAYER_H
#define UTIL_VIDEO_PLAYER_H

#include <QObject>
#include "Utilitys/utils_ffmpegHelper.h"
#include "Utilitys/utilsDfs.h"

class XA_VIDEO_PLAYER : public QObject
{
	Q_OBJECT
public:
	static XA_VIDEO_PLAYER* get_player();
	void setRecordParam(const char* name,int width,int height);
	void writeRecord(uchar* frameraw);
	bool acquireGifImageSeq(const char* path, QObject* receiver);

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

	utils_video::GifInfo gif_info;
	GifFileType* _gif_file;
};

#endif