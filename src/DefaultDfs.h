#ifndef UTILS_DEFAULT_DFS_H
#define UTILS_DEFAULT_DFS_H

#include "UI/UI_DefaultDfs.h"

static enum SDL_CONTEXT_STATUS
{
    ArchiveUpdated = 0x00000001U,
    Logged = 0x00000002U,
} sdl_runtime_context_flag;

#define USER_TEMPORARY_SHADER_PATH "Resources/user/temporary"
#define USER_TEMPORARY_AUDIO_PATH "Resources/user/temporary"
#define USER_RECORD_VIDEO_PATH "Resources/user/video"

#endif
