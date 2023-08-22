#include "utils_ffmpegHelper.h"


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

QMap<QString, int> XA_FFMPEG_HELPER::getAudioInfo(const QString& src)
{
	QMap<QString, int> res;
	QByteArray src_cstr = src.toLatin1();
	AVFormatContext* ic = NULL;
	avformat_open_input(&ic, src_cstr.data(), 0, NULL);
	int re = 0;
	if (re != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		//Do sone log here
		return res;
	}

	re = avformat_find_stream_info(ic, 0);
	res["total_duration"] = ic->duration / (AV_TIME_BASE / 1000);

	//check src only have audio stream and contains no other stream
	AVStream* as = ic->streams[0];
	if (as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
	{
		res["sample_rate"] = as->codecpar->sample_rate;
		res["format"] = as->codecpar->format;
		res["channel"] = as->codecpar->channels;
	}

	if (ic)
		avformat_close_input(&ic);

	return res;
}

