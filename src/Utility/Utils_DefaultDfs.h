#ifndef UTILS_DEFAULTDFS_H
#define UTILS_DEFAULTDFS_H

#include <QProcess>
#include <stdlib.h>
#include <stdio.h>

namespace SDL_UTILS
{
    enum SDL_UTILS_TASK_TYPE
    {
        PlayAudio = 0,
        DecodeGif
    };

    enum AUDIO_STATE
    {
        OnPlay = 0,
        Pause,
        Resume,
        Mute,
        Rewind,
        Idle,
        Duplicated
    };

    struct PlayAudioTask_param
    {
        char audio_path[128];
    };

    struct DecodeGifTask_param
    {

    };

    union SDL_UTILS_TASK_param
    {
        PlayAudioTask_param playAudio_param;
        DecodeGifTask_param decodeGif_param;
    };

    struct SDL_UTILS_TASK
    {
        SDL_UTILS_TASK_TYPE type;
        SDL_UTILS_TASK_param param;
    };

    struct UTILS_AUDIO_INFO
    {
        int sampleRate = -1;
        int channel = -1;
        int sampleSize = -1;
        int duration = -1;
        int anchor = -1;//when audio stop, set the anchor 0<anchor<duration
        bool onplay = false;
    };

    static void UTILS_GENERATE_PCM(const UTILS_AUDIO_INFO& info,const QString& input,const QString& output)
    {
        QStringList argument;
        argument << "-y";
        argument << "-i" << input;
        argument << "-f" << "s16le";
        char c_str_temp[8] = {0};
        sprintf(c_str_temp,"%d",info.channel);
        argument<<"-ac"<<QString::fromLatin1(c_str_temp);
        memset(c_str_temp,0,8);
        sprintf(c_str_temp,"%d",info.sampleRate);
        argument << "-ar"<<QString::fromLatin1(c_str_temp);
        argument << "-acodec"<<"pcm_s16le";
        argument << output;
        QProcess::startDetached("ffmpeg",argument);
    }
}

#endif
