#ifndef GL_DEFAULT_DFS
#define GL_DEFAULT_DFS

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <QObject>
#include <chrono>
#include <mutex>

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
	XA_GL_LOADTEXTURE
};

struct DrawFrontTask_param
{
	QObject* target_reciver;
};

struct LoadTextureTask_param
{
	int index;
	char texture_path[64];
};

struct CompileTask_param
{
	char vs_path[64];
	char fs_path[64];
};

union GL_Task_param
{
	DrawFrontTask_param drawFront_param;
	LoadTextureTask_param loadTexture_param;
	CompileTask_param compileTask_parm;
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
	RENDER_UPDATE = 0b00000100,
	RENDER_PAUSE  = 0b00000010,
	RENDER_EXIT   = 0b00000001,
};

enum XA_RENDER_FLAG_BIT_POS
{
	BIT_UPDATE_POS  = 2,
	BIT_PAUSE_POS   = 1,
	BIT_EXIT_POS    = 0
};
#endif