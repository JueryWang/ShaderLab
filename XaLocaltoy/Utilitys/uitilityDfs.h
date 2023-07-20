#ifndef UTILITY_SUPPORT
#define UTILITY_SUPPORT
#include <QString>

#ifdef  _WIN32
#define _STRING_WRAPPER(str) QStringLiteral(str)

#include <QUuid>
#endif

#ifndef UI_DEFAULT_DFS1
#include "../UI/ui_defaultDfs.h"
#endif // !UI_DEFAULT_DFS1

#endif