#ifndef UTIL_DEFAULTDFS_H
#define UTIL_DEFAULTDFS_H
#include <QString>
#include <iostream>
#include <stdio.h>

namespace parser
{

	const static QString PARSER_PREFIX_VERTEX = \
		"#version 420 core\n"
		"layout(location = 0) in vec3 position; \n"
		"layout(location = 1) in vec2 texlCoord;\n"
		"                                       \n"
		"out vec2 fragCoord;                    \n"
		"                                       \n"
		"void main()                            \n"
		"{                                      \n"
		"    fragCoord = texlCoord;             \n"
		"    gl_Position = vec4(position,1.0);  \n"
		"}";

	const static QString PARSER_PREFIX_FRAGMENT =
		"#version 420 core                      \n"
		"                                       \n"
		"out vec4 fragColor;                    \n"
		"in vec2  fragCoord;                    \n"
		"                                       \n"
		"struct runtime_param                   \n"
		"{                                      \n"
		"   float iTime;                        \n"
		"};                                     \n"
		"uniform runtime_param runtime_data;    \n"
		"uniform ivec2 iResolution;             \n"
		"                                       \n"
		"#define iTime runtime_data.iTime        \n";

#ifdef Q_OS_WIN

	static QString _UTIL_GET_VALADITOR_RES(const char* cmd)
	{
		FILE* pipe = _popen(cmd, "r");
		if (!pipe) return "";

		static char buffer[1024];
		QString result;
		while (!feof(pipe))
		{
			if (fgets(buffer, 1024, pipe) != NULL)
				result += buffer;
		}
		_pclose(pipe);

		return result;
	}
#endif
}

#endif // UTIL_DEFAULTDFS_H