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

#define USER_TEMPORARY_SHADER_PATH "Resource/user/temporary"

#endif