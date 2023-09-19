/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef UTIL_DEFAULTDFS_H
#define UTIL_DEFAULTDFS_H
#include <QString>
#include <iostream>
#include <stdio.h>
#include <QProcess>

namespace parser
{
	enum ParserRule
	{
		ShaderToy = 0,
		DefaultGLSL
	};

	enum ShaderType
	{
		VERTEX = 0,
		TES_CONTROL,
		TES_EVAL,
		GEOMETRY,
		FRAGMENT
	};

	enum VariantType
	{
		INT = 0,
		FLOAT,
		VEC2,
		VEC3,
		VEC4,
	};

	enum ErrorCode
	{
		PARSE_ERROR = 0,
		NO_FILE_SET,
		NO_MAIN_FUNC,
		NO_DETECTED_VARS,
		NO_ERROR
	};

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
		"   int   iFrame;                       \n"
		"	vec2  iMouse;						\n"
		"};                                     \n"
		"uniform runtime_param runtime_data;    \n"
		"uniform ivec2 iResolution;             \n"
		"                                       \n"
		"float iTime = runtime_data.iTime;      \n"
		"vec2 iMouse = runtime_data.iMouse;		\n"
		"int iFrame = runtime_data.iFrame;      \n";

	static QString _UTILS_GET_VALADITOR_RES(const QString& file)
	{
		static QProcess process;
		process.setProgram("glslangValidator");
		QStringList argument;
#ifdef Q_OS_WIN
		argument << "/c" << file;
#elif Q_OS_UNIX
		argument << "-c" << file;
#endif
		process.setArguments(argument);
		process.start();
		process.waitForStarted();
		process.waitForFinished();
		QString outputInfo = QString::fromLocal8Bit(process.readAllStandardOutput());
		return outputInfo;
	}
}

#endif // UTIL_DEFAULTDFS_H