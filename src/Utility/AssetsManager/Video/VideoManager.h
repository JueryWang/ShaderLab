#ifndef VIDEOMANAGER_H
#define VIDEOMANAGER_H

#include <QObject>
#include "Utility/Utils_ffmpegHelper.h"
#include "Utility/Utils_DefaultDfs.h"

namespace SDL_UTILS{
    class VideoManager : public QObject{
        Q_OBJECT
    public:
        static VideoManager* getVideoManager();
        void setRecordParam(const char* name,int width,int height);
        void writeRecord(uint8_t* rawFrame);

    public slots:
        void recordDone();

    private:
        VideoManager();
        ~VideoManager();

    private:
        static VideoManager* s_instance;

        std::string m_recordTargetFile;
        AVFrame *m_videoFrame = nullptr;
        AVCodecContext* m_cctx = nullptr;
        SwsContext* m_swsCtx = nullptr;
        AVFormatContext* m_ofctx = nullptr;
        AVOutputFormat* m_oformat = nullptr;

        int m_fps;
        int m_recordScreenWidth;
        int m_recordScreenHeight;
        int m_bitrate;
        int frameCounter = 0;
    };
}

#endif
