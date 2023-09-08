#include "utils_ffmpegHelper.h"
#include <iostream>
#include <QThread>
#include <memory>
using namespace std;
using namespace utils_ffmpeg;

XA_FFMPEG_HELPER* XA_FFMPEG_HELPER::_instance = nullptr;

XA_FFMPEG_HELPER::XA_FFMPEG_HELPER()
{

}

XA_FFMPEG_HELPER::~XA_FFMPEG_HELPER()
{
	if (videoFrame) {
		av_frame_free(&videoFrame);
	}
	if (cctx) {
		avcodec_free_context(&cctx);
	}
	if (ofctx) {
		avformat_free_context(ofctx);
	}
	if (swsCtx) {
		sws_freeContext(swsCtx);
	}
}

void XA_FFMPEG_HELPER::init_context()
{
	oformat = (AVOutputFormat*)av_guess_format(nullptr, rec_target_file.c_str(), nullptr);
	int err = avformat_alloc_output_context2(&ofctx, oformat, nullptr, rec_target_file.c_str());

	AVCodec* codec = nullptr;
	codec = (AVCodec*)avcodec_find_encoder(oformat->video_codec);

	AVStream* stream = avformat_new_stream(ofctx, codec);

	cctx = avcodec_alloc_context3(codec);

	stream->codecpar->codec_id = oformat->video_codec;
	stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
	stream->codecpar->width = width;
	stream->codecpar->height = height;
	stream->codecpar->format = AV_PIX_FMT_YUV420P;
	stream->codecpar->bit_rate = bitrate * 1000;
	avcodec_parameters_to_context(cctx, stream->codecpar);

	cctx->time_base = { 1,1 };
	cctx->max_b_frames = 0;
	cctx->gop_size = 12;
	cctx->framerate = { fps,1 };

	videoFrame = av_frame_alloc();
	videoFrame->format = AV_PIX_FMT_YUV420P;
	videoFrame->width = cctx->width;
	videoFrame->height = cctx->height;
	if ((err = av_frame_get_buffer(videoFrame, 32)) < 0) {
		std::cout << "Failed to allocate picture" << err << std::endl;
	}

	swsCtx = sws_getContext(cctx->width, cctx->height, AV_PIX_FMT_RGB24, cctx->width,
		cctx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, 0, 0, 0);

	if (stream->codecpar->codec_id == AV_CODEC_ID_H264)
	{
		av_opt_set(cctx, "preset", "ultrafast", 0);
	}
	else if (stream->codecpar->codec_id == AV_CODEC_ID_H265)
	{
		av_opt_set(cctx, "preset", "ultrafast", 0);
	}

	avcodec_parameters_from_context(stream->codecpar, cctx);

	if ((err = avcodec_open2(cctx, codec, NULL)) < 0)
	{
		std::cout << "Failed to open codec" << err << std::endl;
		//do some log here
	}

	if (!(oformat->flags & AVFMT_NOFILE)) {
		if ((err = avio_open(&ofctx->pb, rec_target_file.c_str(), AVIO_FLAG_WRITE)) < 0)
		{
			std::cout << "Failed to open file" << err << std::endl;
			//do some log here
		}
	}

	if ((err = avformat_write_header(ofctx, NULL)) < 0) {
		std::cout << "Failed to write header" << err << std::endl;
	}

	av_dump_format(ofctx, 0, rec_target_file.c_str(), 1);
}

void XA_FFMPEG_HELPER::finish()
{
	//DELAYED FRAMES
	AVPacket pkt;
	av_init_packet(&pkt);
	pkt.data = NULL;
	pkt.size = 0;

	for (;;) {
		avcodec_send_frame(cctx, NULL);
		if (avcodec_receive_packet(cctx, &pkt) == 0) {
			av_interleaved_write_frame(ofctx, &pkt);
			av_packet_unref(&pkt);
		}
		else {
			break;
		}
	}

	av_write_trailer(ofctx);

	if (!(oformat->flags & AVFMT_NOFILE)) {
		int err = avio_close(ofctx->pb);
		if (err < 0) {
			std::cout << "Failed to close file" << err << std::endl;
		}
	}
}

void XA_FFMPEG_HELPER::free()
{
	if (videoFrame) {
		av_frame_free(&videoFrame);
		videoFrame = nullptr;
	}
	if (cctx) {
		avcodec_free_context(&cctx);
		cctx = nullptr;
	}
	if (ofctx) {
		avformat_free_context(ofctx);
		ofctx = nullptr;
	}
	if (swsCtx) {
		sws_freeContext(swsCtx);
		swsCtx = nullptr;
	}
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

void XA_FFMPEG_HELPER::setAVRecordParam(const std::string& filename, int width, int height)
{
	rec_target_file = filename;
	this->width = width;
	this->height = height;
	this->frameCounter = 0;
	free();
	init_context();
}

void XA_FFMPEG_HELPER::pushFrame(uchar* frameraw)
{
	int err;

	if (!videoFrame || !swsCtx || !cctx || !ofctx) {
		return;
	}
	int inLinesize[1] = { 3 * cctx->width };
	// From RGB to YUV
	sws_scale(swsCtx, (const uint8_t* const*)&frameraw, inLinesize, 0, cctx->height,
		videoFrame->data, videoFrame->linesize);
	videoFrame->pts = (1.0 / 30.0) * 90000 * (frameCounter++);
	if ((err = avcodec_send_frame(cctx, videoFrame)) < 0) {
		frameCounter--;
		return;
	}

	AVPacket pkt;
	av_init_packet(&pkt);
	pkt.data = NULL;
	pkt.size = 0;
	pkt.flags |= AV_PKT_FLAG_KEY;
	if (avcodec_receive_packet(cctx, &pkt) == 0)
	{
		uint8_t* size = ((uint8_t*)pkt.data);
		av_interleaved_write_frame(ofctx, &pkt);
		av_packet_unref(&pkt);
	}
}

void XA_FFMPEG_HELPER::AVRecordDone()
{
	finish();
	this->frameCounter = 0;
}

