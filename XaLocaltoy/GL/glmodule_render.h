#ifndef GL_MODULE_RENDER_H
#define GL_MODULE_RENDER_H

#include <QObject>
#include <QWidget>
#include <vector>
#include <map>
#include <string>
#include <atomic>
#include <shader.h>
#include "gl_defaultDfs.h"

class XA_GLMODULE_RENDER : public QObject
{
	friend class XA_GLMODULE_BACKSTG;
	Q_OBJECT

public:
	XA_GLMODULE_RENDER(const std::string& title,StorageType type = XA_GL_RGB,QWidget* reciver = nullptr);
	~XA_GLMODULE_RENDER();
	//void addShader(const char* vertexPath, const char* fragmentPath,const char* name);
	static void setWindowSize(int SCR_WIDTH,int SCR_HEIGHT);
	inline void flip(uint8_t** buf,int context_width,int context_height);
	void pause();
	void start();
	void restart();

private:
	void renderQuad(int context_width, int context_height);

public slots:
	void contextDraw();
	/*
	* multi Window Instance need to be reconstructed by vulkan
	bool operator<(const XA_GLMODULE_RENDER& other)
	{
		return _layer < other._layer;
	}
	*/
public:
	int getWidth() const;
	int getHeight() const;
	static std::atomic<int> SCR_WIDTH;
	static std::atomic<int> SCR_HEIGHT;
	static int resolution[2];
	static std::atomic<bool> window_changed;
private:
	void* _windowbuf;
	GLFWwindow* _window;
	QObject* _reciver;
	StorageType _type;
	Shader* _shader;
	std::string _title;
	/*
	* multi Window Instance need to be reconstructed by vulkan
		int _layer;//
	*/
	int windowBufSize;
	int activeShader = 0;


	std::map<std::string, unsigned int> vao_mp;
	std::map<std::string, unsigned int> vbo_mp;
	
	std::atomic<bool> paused = false;
};

#endif
