#ifndef GL_DEFAULT_DFS
#define GL_DEFAULT_DFS

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

#endif