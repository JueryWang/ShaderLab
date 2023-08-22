#ifndef UTILS_SUPPORT
#define UTILS_SUPPORT
#include <QIODevice>

enum XA_UTILS_TASK_TYPE
{
	XA_UTIL_PLAYAUDIO = 0,
};

struct PlayAudioTask_param
{
	char audio_path[64];
	QIODevice* device;
};

union UTILS_TASK_param
{
	PlayAudioTask_param playAudio_pram;
};

struct XA_UTILS_TASK
{
	XA_UTILS_TASK_TYPE type;
	UTILS_TASK_param param;
};

#endif
