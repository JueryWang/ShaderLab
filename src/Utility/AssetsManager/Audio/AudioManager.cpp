#include "AudioManager.h"
#include "Utility/Utils_SessionManager.h"
#include "UI/AssetWindow.h"

namespace SDL_UTILS {

    AudioManager* AudioManager::s_instance = nullptr;

    AudioManager *AudioManager::getAudioManager() {
        if(!s_instance)
        {
            s_instance = new AudioManager();
            connect(Utils_SessionManager::getInstance(),&Utils_SessionManager::AudioPlayDone,s_instance,&AudioManager::playDone);
        }
        return s_instance;
    }

    void AudioManager::play(const QString &src) {
        std::pair<QObject*,SDL_UTILS_TASK> audioPlayTask;
        audioPlayTask.first = this;
        audioPlayTask.second.type = SDL_UTILS_TASK_TYPE::PlayAudio;
        QByteArray s = src.toLatin1();
        strcpy(audioPlayTask.second.param.playAudio_param.audio_path,s.data());
        Utils_SessionManager::getInstance()->addTask(audioPlayTask);
        Utils_SessionManager::getInstance()->AudioPlay();
    }

    void AudioManager::pause() {
        Utils_SessionManager::getInstance()->AudioPause();
    }

    void AudioManager::rewind() {
        Utils_SessionManager::getInstance()->AudioRewind();
    }

    void AudioManager::mute() {
        Utils_SessionManager::getInstance()->AudioMute();
    }

    void AudioManager::quit() {
        Utils_SessionManager::getInstance()->AudioQuit();
        m_invoker = nullptr;
    }

    void AudioManager::resume() {
        Utils_SessionManager::getInstance()->AudioResume();
    }

    void AudioManager::replay() {
        Utils_SessionManager::getInstance()->AudioPlay();
    }

    void AudioManager::playDone() {
        if(this->m_invoker != nullptr)
        {
            m_invoker->setAudioPlayDone();
        }
    }

    AudioManager::AudioManager() {

    }

    AudioManager::~AudioManager() {

    }
} // SDL_UTILS