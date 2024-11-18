#include "Utils_SessionManager.h"
#include "Utils_ffmpegHelper.h"
#include "Utility/AssetsManager/Audio/AudioManager.h"
#include <QDir>

namespace SDL_UTILS
{
    Utils_SessionManager* Utils_SessionManager::s_instance = nullptr;
    const char* Utils_SessionManager::s_cachePath;
    char Utils_SessionManager::s_AudioOutputFile[256];

    Utils_SessionManager::Utils_SessionManager() {

    }

    Utils_SessionManager::~Utils_SessionManager() {
        delete m_audioOut;
        delete m_AudioIO;
        fclose(m_AudioFileHandler);
        delete m_AudioFileHandler;
    }

    Utils_SessionManager *Utils_SessionManager::getInstance() {
        if(!s_instance)
        {
            s_instance = new Utils_SessionManager();
        }
        return s_instance;
    }

    void Utils_SessionManager::setAudioCachePath(const char *path) {
        QDir dir_helper;
        if(!dir_helper.exists(path))
        {
            dir_helper.mkdir(path);
        }
        Utils_SessionManager::s_cachePath = path;
        strcpy(s_AudioOutputFile,s_cachePath);
        strcat(s_AudioOutputFile,"/out.pcm");
    }

    void Utils_SessionManager::cleanCache() {
        QDir dir(QDir::currentPath() + "/" + s_cachePath);
        dir.removeRecursively();
    }

    void Utils_SessionManager::setAudioVolume(float volume) {
        m_crtAudioVolume.store(volume,std::memory_order_release);
    }

    void Utils_SessionManager::run() {
        while(true)
        {
            while(!m_task_queue.empty())
            {
                std::pair<QObject*,SDL_UTILS_TASK> crt_task = m_task_queue.front();
                switch (crt_task.second.type) {
                    case PlayAudio:
                    {
                        handlePalyAudio(crt_task);
                        break;
                    }
                    case DecodeGif:
                    {
                        handleGifDecode(crt_task);
                        break;
                    }
                    m_task_queue.pop();
                }
            }
            QThread::usleep(1);
        }
        QThread::msleep(1);
    }

    void Utils_SessionManager::handlePalyAudio(const std::pair<QObject *, SDL_UTILS_TASK> &crt_task) {
        AUDIO_STATE crt_state = m_audioState.load(std::memory_order_consume);
        if (crt_state == Duplicated)
        {
            if(m_AudioFileHandler != NULL)
            {
                fclose(m_AudioFileHandler);
                delete m_audioOut;
                delete m_AudioIO;
                delete m_audioBuf;
                m_AudioFileHandler = NULL;
                m_audioState = Idle;
            }
            return;
        }

        if(m_AudioFileHandler == NULL)
        {
        }
    }

    void Utils_SessionManager::handleGifDecode(const std::pair<QObject *, SDL_UTILS_TASK> &crt_task) {

    }

    void Utils_SessionManager::AudioPlay() {
        m_audioState.store(OnPlay,std::memory_order_release);
    }

    void Utils_SessionManager::AudioPause() {
        m_audioState.store(Pause,std::memory_order_release);
    }

    void Utils_SessionManager::AudioRewind() {
        m_audioState.store(Rewind,std::memory_order_release);
    }

    void Utils_SessionManager::AudioMute() {
        m_audioState.store(Mute,std::memory_order_release);
    }

    void Utils_SessionManager::AudioQuit() {
        m_audioState.store(Duplicated,std::memory_order_release);
    }

    void Utils_SessionManager::AudioResume() {
        m_audioState.store(Resume,std::memory_order_release);
        std::pair<QObject*,SDL_UTILS_TASK> audioResumeTask;
        audioResumeTask.first = AudioManager::getAudioManager();
        audioResumeTask.second.type = PlayAudio;
        addTask(audioResumeTask);
    }

    void Utils_SessionManager::AudioReplay() {
        m_audioState.store(OnPlay);
        rewind(m_AudioFileHandler);
        std::pair<QObject*,SDL_UTILS_TASK> audioReplayTask;
        audioReplayTask.first = AudioManager::getAudioManager();
        audioReplayTask.second.type = PlayAudio;
        addTask(audioReplayTask);
    }

    void inline Utils_SessionManager::raiseVolume(char *buf, int len, float factor) {

    }
}
