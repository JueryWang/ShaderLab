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
		"										\n"
		"layout (binding = 0) uniform sampler2D iChannel0;\n"
		"layout (binding = 1) uniform sampler2D iChannel1;\n"
		"layout (binding = 2) uniform sampler2D iChannel2;\n"
		"layout (binding = 3) uniform sampler2D iChannel3;\n"
		"layout (binding = 4) uniform sampler2D iChannel4;\n"
		"layout (binding = 5) uniform sampler2D iChannel5;\n"
		"layout (binding = 6) uniform sampler2D iChannel6;\n"
		"layout (binding = 7) uniform sampler2D iChannel7;\n"
		"layout (binding = 8) uniform sampler2D iChannel8;\n"
		"layout (binding = 9) uniform sampler2D iChannel9;\n"
		"                                       \n"
		"struct runtime_param                   \n"
		"{                                      \n"
		"   float iTime;                        \n"
		"	ivec4 iMouse;						\n"
		"};                                     \n"
		"uniform runtime_param runtime_data;    \n"
		"uniform ivec2 iResolution;             \n"
		"                                       \n"
		"#define iTime runtime_data.iTime       \n"
		"#define iMouse runtime_data.iMouse		\n";

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