#ifndef UTILS_FFMPEGHELPER_H
#define UTILS_FFMPEGHELPER_H

#include <QtCore/QObject>
#include "Utils_DefaultDfs.h"
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/frame.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
};

namespace SDL_UTILS
{
    class Utils_ffmpegHelper : QObject {
    public:
        static Utils_ffmpegHelper* getHelper();
        static void disableLog(){ av_log_set_callback(nullptr);}
        void getAudioInfo(UTILS_AUDIO_INFO* info,const char* src);
        void setAVRecordParam(const std::string& filename,int width,int height);
        void pushFrame(uint8_t* rawframe);
        void AVRecordDone();

    private:
        Utils_ffmpegHelper();
        ~Utils_ffmpegHelper();
        void initContext();
        void finishRecord();
        void clean();

    private:
        static Utils_ffmpegHelper* s_instance;

        std::string m_recordTargetFile;
        AVFrame* m_videoFrame = nullptr;
        AVCodecContext* m_cctx = nullptr;
        SwsContext* m_swsCtx = nullptr;
        AVFormatContext* m_ofctx = nullptr;
        AVOutputFormat* m_ofmat = nullptr;
        AVStream* m_stream = nullptr;
        AVCodec* m_codec = nullptr;

        int m_fps;
        int m_recordScreenWidth;
        int m_recordScreenHeight;
        int m_bitrate;
        int frameCounter = 0;
    };
}


#endif //SHADERLAB_UTILS_FFMPEGHELPER_H
