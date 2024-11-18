#include "VideoManager.h"
#include "DefaultDfs.h"
#include "GL/GLSessionManager.h"
#include "Utility/Utils_DefaultDfs.h"
#include "Utility/Utils_SessionManager.h"

using namespace SDL_GL_MODULE;

namespace SDL_UTILS
{
    VideoManager* VideoManager::s_instance = nullptr;

    VideoManager *VideoManager::getVideoManager() {
        if(!s_instance)
        {
            s_instance = new VideoManager();
            connect(GLSessionManager::getInstance(),&GLSessionManager::videoRecordDone,s_instance,&VideoManager::recordDone);
        }
        return s_instance;
    }

    void VideoManager::setRecordParam(const char *name, int width, int height) {
        std::string s = USER_RECORD_VIDEO_PATH;
        s += "/";
        s += name;
        s += ".mp4";
        Utils_ffmpegHelper::getHelper()->setAVRecordParam(s,width,height);
    }

    void VideoManager::writeRecord(uint8_t*rawFrame) {
        Utils_ffmpegHelper::getHelper()->pushFrame(rawFrame);
    }

    void VideoManager::recordDone() {
        Utils_ffmpegHelper::getHelper()->AVRecordDone();
    }

    VideoManager::VideoManager() {
        Utils_ffmpegHelper::getHelper()->disableLog();
    }

    VideoManager::~VideoManager() {

    }
}