#include "Utils_ffmpegHelper.h"
#include "UI/UI_DefaultDfs.h"
#include <mutex>
#include <iostream>

namespace SDL_UTILS
{

    Utils_ffmpegHelper* Utils_ffmpegHelper::s_instance = nullptr;
    std::mutex videoRecordMutex;

    Utils_ffmpegHelper *Utils_ffmpegHelper::getHelper() {
        if(s_instance == nullptr)
        {
            s_instance = new Utils_ffmpegHelper();
        }
        return s_instance;
    }

    void Utils_ffmpegHelper::getAudioInfo(UTILS_AUDIO_INFO *info, const char *src) {
        AVFormatContext* context = NULL;
        avformat_open_input(&context,src,0,NULL);
        int re = 0;
        if(re != 0)
        {
            char buf[1024] = {0};
            av_strerror(re,buf,sizeof(buf)-1);
            //Do some log here
        }

        re = avformat_find_stream_info(context,0);
        info->duration = context->duration/(AV_TIME_BASE/1000);

        //check src only have audio stream and contains no other stream
        AVStream* as = context->streams[0];
        if(as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            info->sampleRate = as->codecpar->sample_rate;
            info->channel = as->codecpar->bits_per_coded_sample;
            info->sampleSize = 16;
        }

        if(context)
            avformat_close_input(&context);
    }

    void Utils_ffmpegHelper::setAVRecordParam(const std::string &filename, int width, int height) {
        m_recordTargetFile = filename;
        m_recordScreenWidth = width;
        m_recordScreenHeight = height;
        frameCounter = 0;
        clean();
        initContext();
    }

    void Utils_ffmpegHelper::pushFrame(uint8_t* rawframe) {

        if (videoRecordMutex.try_lock())
        {
		    int err;

		    if (!m_videoFrame || !m_swsCtx || !m_cctx || !m_ofctx) {
			    return;
		    }
		    int inLinesize[1] = { 3 * m_cctx->width };
		    // From RGB to YUV

            if (rawframe != nullptr)
            {    
			    sws_scale(m_swsCtx, (const uint8_t* const*)&rawframe, inLinesize, 0, m_cctx->height,
				    m_videoFrame->data, m_videoFrame->linesize);
            }
		    m_videoFrame->pts = (1.0 / m_fps) * 40000 * (frameCounter++);
		    if ((err = avcodec_send_frame(m_cctx, m_videoFrame)) < 0) {
			    frameCounter--;
			    return;
		    }

		    AVPacket pkt;
		    av_init_packet(&pkt);
		    pkt.data = NULL;
		    pkt.size = 0;
		    pkt.flags |= AV_PKT_FLAG_KEY;
		    if (avcodec_receive_packet(m_cctx, &pkt) == 0)
		    {
			    uint8_t* size = ((uint8_t*)pkt.data);
			    av_interleaved_write_frame(m_ofctx, &pkt);
			    av_packet_unref(&pkt);
		    }

		    free(rawframe);
            rawframe = nullptr;
            videoRecordMutex.unlock();
        }
        else
        {
			free(rawframe);
			rawframe = nullptr;
			videoRecordMutex.unlock();
        }
    }

    void Utils_ffmpegHelper::AVRecordDone() {
        finishRecord();
        clean();
        frameCounter = 0;
    }

    Utils_ffmpegHelper::Utils_ffmpegHelper() {
        m_fps = 30;
        m_recordScreenWidth = SDL_UI_MODULE::screen_resolution_x;
        m_recordScreenHeight = SDL_UI_MODULE::screen_resolution_y;
        m_bitrate = 3000;
    }

    Utils_ffmpegHelper::~Utils_ffmpegHelper() {
        if(m_videoFrame){
            av_frame_free(&m_videoFrame);
        }
        if(m_cctx){
            avcodec_free_context(&m_cctx);
        }
        if(m_ofctx){
            avformat_free_context(m_ofctx);
        }
        if(m_swsCtx){
            sws_freeContext(m_swsCtx);
        }
    }

    void Utils_ffmpegHelper::initContext() {

        m_ofmat = (AVOutputFormat*)av_guess_format(nullptr, m_recordTargetFile.c_str(), nullptr);
        int err = avformat_alloc_output_context2(&m_ofctx, m_ofmat, nullptr, m_recordTargetFile.c_str());

        AVStream* m_stream = avformat_new_stream(m_ofctx, m_codec);
        m_codec = (AVCodec*)avcodec_find_encoder(m_ofmat->video_codec);

        m_cctx = avcodec_alloc_context3(m_codec);
        m_stream->codecpar->codec_id = m_ofmat->video_codec;
        m_stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
        m_stream->codecpar->width = m_recordScreenWidth;
        m_stream->codecpar->height = m_recordScreenHeight;
        m_stream->codecpar->format = AV_PIX_FMT_YUV420P;
        m_stream->codecpar->bit_rate = m_bitrate * 1000;
        avcodec_parameters_to_context(m_cctx, m_stream->codecpar);

        m_cctx->time_base = { 1,40000 };
        m_cctx->max_b_frames = 0;
        m_cctx->gop_size = 12;
        m_cctx->framerate = { m_fps,1 };

		m_videoFrame = av_frame_alloc();
		m_videoFrame->format = AV_PIX_FMT_YUV420P;
		m_videoFrame->width = m_cctx->width;
		m_videoFrame->height = m_cctx->height;
		if ((err = av_frame_get_buffer(m_videoFrame, 32)) < 0) {
			std::cout << "Failed to allocate picture" << err << std::endl;
		}

		m_swsCtx = sws_getContext(m_cctx->width, m_cctx->height, AV_PIX_FMT_RGB24, m_cctx->width,
			m_cctx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, 0, 0, 0);

		if (m_stream->codecpar->codec_id == AV_CODEC_ID_H264)
		{
			av_opt_set(m_cctx, "preset", "ultrafast", 0);
		}
		else if (m_stream->codecpar->codec_id == AV_CODEC_ID_H265)
		{
			av_opt_set(m_cctx, "preset", "ultrafast", 0);
		}

		avcodec_parameters_from_context(m_stream->codecpar, m_cctx);

		if ((err = avcodec_open2(m_cctx, m_codec, NULL)) < 0)
		{
			std::cout << "Failed to open codec" << err << std::endl;
			//do some log here
		}

		if (!(m_ofmat->flags & AVFMT_NOFILE)) {
			if ((err = avio_open(&m_ofctx->pb, m_recordTargetFile.c_str(), AVIO_FLAG_WRITE)) < 0)
			{
				std::cout << "Failed to open file" << err << std::endl;
				//do some log here
			}
		}

		if ((err = avformat_write_header(m_ofctx, NULL)) < 0) {
			std::cout << "Failed to write header" << err << std::endl;
		}

		av_dump_format(m_ofctx, 0, m_recordTargetFile.c_str(), 1);

    }

    void Utils_ffmpegHelper::finishRecord() {
        //Delayed frames
        AVPacket pkt;
        av_init_packet(&pkt);
        pkt.data = nullptr;
        pkt.size = 0;

        for(;;){
            avcodec_send_frame(m_cctx,NULL);
            if(avcodec_receive_packet(m_cctx,&pkt) == 0){
                av_interleaved_write_frame(m_ofctx,&pkt);
                av_packet_unref(&pkt);
            }else{
                break;
            }
        }

        av_write_trailer(m_ofctx);

        if(!(m_ofctx->flags & AVFMT_NOFILE)){
            int err = avio_close(m_ofctx->pb);
            if(err < 0){
                std::cout<<"Failed to close file "<< err<<std::endl;
            }
        }
    }

    void Utils_ffmpegHelper::clean() {
        if(m_videoFrame){
            av_frame_free(&m_videoFrame);
            m_videoFrame = nullptr;
        }
        if(m_cctx){
            avcodec_free_context(&m_cctx);
            m_cctx = nullptr;
        }
        if(m_ofctx){
            avformat_free_context(m_ofctx);
            m_ofctx = nullptr;
        }
        if(m_swsCtx){
            sws_freeContext(m_swsCtx);
            m_swsCtx = nullptr;
        }
    }
}