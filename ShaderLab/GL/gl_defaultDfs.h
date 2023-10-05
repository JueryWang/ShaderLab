/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef GL_DEFAULT_DFS
#define GL_DEFAULT_DFS

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <QObject>
#include "../Utilitys/Parser/parser_defaultDfs.h"
#include "../UI/uimodule_varShowboard.h"

#define MAX_GL_RENDERER 3
//set ratio to fit high dpi screen
#define GL_WIDGET_DEFAULT_WIDTH_R 0.5
#define GL_WIDGET_DEFAULT_HEIGHT_R 0.8

#define GL_WIDGET_MIN_WIDTH_R 0.2
#define GL_WIDGET_MIN_HEIGHT_R 0.2

#define GL_WIDGET_MAX_WIDTH_R 0.8
#define GL_WIDGET_MAX_HEIGHT_R 0.8

enum StorageType {
	XA_GL_RGB = GL_RGB,
	XA_GL_RGBA = GL_RGBA
};

enum XA_RENDERER_STATE
{
	ACTIVE_DRAW_FRONT = 0,
	ACTIVE_DRAW_BACK,
	INACTIVE
};

enum XA_GL_TASK_TYPE
{
	XA_GL_DRAWFRONT = 0,
	XA_GL_COMPILE_SHADER,
	XA_GL_LOADTEXTURE,
	XA_GL_RECORD,
};

enum XA_GL_SCRIPT_TYPE
{
	XA_GL_SCRIPT_NOTYPE,
	XA_GL_SCRIPT_COMMON,
	XA_GL_SCRIPT_IMAGE,
	XA_GL_SCRIPT_BUFFERA,
	XA_GL_SCRIPT_BUFFERB,
	XA_GL_SCRIPT_BUFFERC,
	XA_GL_SCRIPT_BUFFERD,
	XA_GL_SCRIPT_CUBEMAP
};

struct DrawFrontTask_param
{
	QObject* target_reciver;
};

struct LoadTextureTask_param
{
	int index;
	char texture_path[64];
	LoadTextureTask_param* next;//for batch commit
};

struct CompileTask_param
{
	parser::ParserRule rule;
	XA_UIMODULE_ShowBoard* varboard;
};

struct RecordTask_param
{
	uchar* frameAddr;
};

union GL_Task_param
{
	DrawFrontTask_param drawFront_param;
	LoadTextureTask_param loadTexture_param;
	CompileTask_param compileTask_parm;
	RecordTask_param recordTask_param;
};

struct XA_GL_TASK
{
	XA_GL_TASK_TYPE type;
	GL_Task_param param;
};

enum TEXTURE_STATUS
{
	TEXTURE_ST_UNLOAD = 0,
	TEXTURE_ST_LOADED,
	TEXTURE_ST_DEPRECATED
};

struct XA_GL_TEXTURE_INFO
{
	int index = -1;
	int width;
	int height;
	GLenum format;
	GLuint textureID;
	unsigned char* address;
	TEXTURE_STATUS status = TEXTURE_ST_DEPRECATED;
};

enum XA_RENDER_FLAG_BIT
{
	RENDER_RECORD = 0b00001000,
	RENDER_UPDATE = 0b00000100,
	RENDER_PAUSE  = 0b00000010,
	RENDER_EXIT   = 0b00000001,
};

enum XA_RENDER_FLAG_BIT_POS
{
	BIT_RECORD_POS	= 3,
	BIT_UPDATE_POS  = 2,
	BIT_PAUSE_POS   = 1,
	BIT_EXIT_POS    = 0
};

enum XA_GL_SHADER_TYPE
{
	XA_GL_MAIN_SHADER,
	XA_GL_BUFFER_BUFFER,
	XA_GL_COMBUTE_SHADER
};

struct XA_GL_SHADER_INFO
{
	Shader program;
	bool inited = false;
	//As the number rises,the priority of decrease,which means 0 should be first executed 
	int order = -1;
	char path_vertex[128];
	char path_fragment[128];
	std::list<XA_GL_SHADER_INFO*> reference;
	XA_GL_SCRIPT_TYPE type;
	XA_GL_TEXTURE_INFO textures[4];
};

void inline setShaderOrder(XA_GL_SHADER_INFO* shader_info,int &result)
{
	for (auto ref : shader_info->reference)
	{
		setShaderOrder(ref, result);
		if (ref->order+1 > result)
		{
			result = ref->order+1;
		}
	}
}
#endif