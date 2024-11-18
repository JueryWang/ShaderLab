#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>
#include <QThread>

namespace SDL_UI_MODULE
{
    class AssetWindowBox;
}

namespace SDL_UTILS {

    class AudioManager : public QObject{
        Q_OBJECT
    public:
        static AudioManager* getAudioManager();
        void play(const QString& src);
        void pause();
        void rewind();
        void mute();
        void quit();
        void resume();
        void replay();

    public slots:
        void playDone();

    private:
        AudioManager();
        ~AudioManager();

    public:
        SDL_UI_MODULE::AssetWindowBox* m_invoker = nullptr;
    private:
        static AudioManager* s_instance;
    };

} // SDL_UTILS

#endif //SHADERLAB_AUDIOMANAGER_H
