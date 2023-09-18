/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef UTILS_SUPPORT
#define UTILS_SUPPORT

#include <QProcess>
#include <QDebug>
#include "libgif/gif_lib.h"

namespace utils_video
{
	struct GifInfo
	{
		int width;
		int height;
		int interval;
		int colors;
		int loopcount;
	};
}

enum XA_UTILS_TASK_TYPE
{
	XA_UTIL_PLAYAUDIO = 0,
	XA_UTIL_DECODEGIF
};

enum XA_AUDIO_STATE
{
	AUDIO_ST_ONPLAY = 0,
	AUDIO_ST_PAUSE,
	AUDIO_ST_RESUME,
	AUDIO_ST_REWIND,
	AUDIO_ST_MUTE,
	AUDIO_ST_DEPRECATED
};

struct PlayAudioTask_param
{
	char audio_path[128];
};

struct DecodeGifTask_param
{
	utils_video::GifInfo* info;
	GifFileType* gifFile;
	QObject* receiver;
};

union UTILS_TASK_param
{
	PlayAudioTask_param playAudio_param;
	DecodeGifTask_param decodeGif_param;
};

struct XA_UTILS_TASK
{
	XA_UTILS_TASK_TYPE type;
	UTILS_TASK_param param;
};


namespace utils_ffmpeg
{
	struct XA_FFMPEG_AU_INFO
	{
		int sampleRate = -1;
		int channel = -1;
		int sampleSize = -1;
		int duration = -1;
		int anchor = -1;//when audio stop ,set the anchor 0<anchor<duration
		bool onplay = false;
	};

	static void _UTILS_GENERATE_PCM(const XA_FFMPEG_AU_INFO& info, const QString& input, const QString& output)
	{
		QStringList argument;
		argument << "-y";
		argument << "-i" << input;
		argument << "-f" << "s16le";
		char c_str_temp[8] = { 0 };
		itoa(info.channel, c_str_temp, 10);
		argument << "-ac" << QString::fromLatin1(c_str_temp);
		memset(c_str_temp, 0, 8);
		itoa(info.sampleRate, c_str_temp, 10);
		argument << "-ar" << QString::fromLatin1(c_str_temp);
		argument << "-acodec" << "pcm_s16le";
		argument << output;
		QProcess::startDetached("ffmpeg", argument);
	}
}

#define eps 0.001
#endif
