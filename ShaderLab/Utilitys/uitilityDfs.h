/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef UTILITY_SUPPORT
#define UTILITY_SUPPORT

#include <QString>
#ifdef  Q_OS_WIN
#define _STRING_WRAPPER(str) QStringLiteral(str)
#endif

#ifndef UI_DEFAULT_DFS1
#include "../UI/ui_defaultDfs.h"
#endif // !UI_DEFAULT_DFS1

static enum RT_CONTEXT_STATUS
{
	//last two bit flag - login save
	Archive_updated = 0x00000001U,
	Logged=0x00000002U,
} xa_context_flag;

static unsigned int xa_context = 0x00000000U;

#define USER_TEMPORARY_SHADER_PATH "Resources/user/temporary"
#define USER_TEMPORARY_AUDIO_PATH "Resources/user/temporary"
#define GL_DEFAULT_RECORD_VIDEO_PATH "Resources/user/video"

#endif