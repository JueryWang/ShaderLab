#ifndef UTILS_SESSIONMANAGER_H
#define UTILS_SESSIONMANAGER_H

#include "Utils_DefaultDfs.h"
#include "SessionManager.h"
#include <QAudioFormat>
#include <QAudioOutput>

namespace SDL_UTILS
{
    class Utils_SessionManager : public SessionManager<QObject,SDL_UTILS_TASK>{
        friend class AudioManager;
        Q_OBJECT
    public:
        static Utils_SessionManager* getInstance();
        static void setAudioCachePath(const char* path);
        static void cleanCache();
        void setAudioVolume(float volume);

    signals:
        void AudioPlayDone();

    protected:
        void run() override;

    private:
        Utils_SessionManager();
        ~Utils_SessionManager();
        void handlePalyAudio(const std::pair<QObject*,SDL_UTILS_TASK> &crt_task);
        void handleGifDecode(const std::pair<QObject*,SDL_UTILS_TASK> &crt_task);
        void AudioPlay();
        void AudioPause();
        void AudioRewind();
        void AudioMute();
        void AudioQuit();
        void AudioResume();
        void AudioReplay();
        void inline raiseVolume(char* buf,int len,float factor);

    private:
        static Utils_SessionManager* s_instance;
        static const char* s_cachePath;
        static char s_AudioOutputFile[256];

        std::atomic<AUDIO_STATE> m_audioState;
        std::atomic<float> m_crtAudioVolume;
        QAudioFormat m_audioFormat;
        QAudioOutput *m_audioOut = nullptr;
        int m_audioPeriodSize;
        char* m_audioBuf;

        QIODevice* m_AudioIO = nullptr;

        FILE* m_AudioFileHandler = NULL;
    };
}


#endif
