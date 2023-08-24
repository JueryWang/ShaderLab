#include "utils_ffmpegHelper.h"
using namespace utils_ffmpeg;

XA_FFMPEG_HELPER* XA_FFMPEG_HELPER::_instance = nullptr;

XA_FFMPEG_HELPER::XA_FFMPEG_HELPER()
{

}

XA_FFMPEG_HELPER::~XA_FFMPEG_HELPER()
{

}

XA_FFMPEG_HELPER* XA_FFMPEG_HELPER::getHelper()
{
	if (_instance == nullptr)
	{
		_instance = new XA_FFMPEG_HELPER();
	}
	return _instance;
}

void XA_FFMPEG_HELPER::getAudioInfo(utils_ffmpeg::XA_FFMPEG_AU_INFO* info, const char* src)
{
	AVFormatContext* ic = NULL;
	avformat_open_input(&ic, src, 0, NULL);
	int re = 0;
	if (re != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		//Do some log here
	}

	re = avformat_find_stream_info(ic, 0);
	info->duration = ic->duration / (AV_TIME_BASE / 1000);

	//check src only have audio stream and contains no other stream
	AVStream* as = ic->streams[0];
	if (as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
	{
		info->sampleRate = as->codecpar->sample_rate;
		info->channel = as->codecpar->channels;
		info->sampleSize = 16;
	}

	if (ic)
		avformat_close_input(&ic);

}

