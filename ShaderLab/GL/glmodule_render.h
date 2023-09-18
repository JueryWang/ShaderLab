/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/

#ifndef GL_MODULE_RENDER_H
#define GL_MODULE_RENDER_H

#include "gl_defaultDfs.h"
#include <QObject>
#include <QWidget>
#include <vector>
#include <string>
#include <atomic>
#include <shader.h>
#include <map>
#include <QDebug>

class XA_GL_TEXTURE_INFO;

class XA_GLMODULE_RENDER : public QObject
{
	friend class XA_GLMODULE_BACKSTG;
	Q_OBJECT

public:
	XA_GLMODULE_RENDER(const std::string& title,StorageType type = XA_GL_RGB,QObject* reciver = nullptr);
	~XA_GLMODULE_RENDER();
	void setReciver(QObject* reciver);
	void setTitle(const char* title);
	//void addShader(const char* vertexPath, const char* fragmentPath,const char* name);
	inline void flip(uint8_t** buf,int context_width,int context_height);
	void __exit();
	void __start();
	void __pause();
	void __restart();
	void __update();
	void __record();
	void __endrecord();
	static void __reset(const QSize& newSize);

private:
	void renderQuad(int context_width, int context_height);
	void loadTextures();
	void updateGLContex();
	struct Deleter {
		void operator()(GLFWwindow* w)
		{
			if (w != nullptr)
			{
				glfwTerminate();
				glfwDestroyWindow(w);
			}
		}
	};

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
	void setStatus(uint8_t bit_mask,bool open_bit);
	bool getFlag(XA_RENDER_FLAG_BIT_POS pos);
	XA_RENDERER_STATE rd_state = INACTIVE;

private:
	void* _windowbuf;
	std::unique_ptr <GLFWwindow,Deleter> _window;
	QObject* _reciver;
	StorageType _type;
	Shader* _shader = nullptr;
	std::string _title;
	std::string _vs_source;
	std::string _fs_source;
	/*
	* multi Window Instance need to be reconstructed by vulkan
		int _layer;//
	*/
	int windowBufSize;
	int activeShader = 0;

	float anchor_time;
	//use uint_8 flags to express 8 bools
	//0-3 reserved 4.record| 5. update |6 pause | 7 exit 
	std::atomic<uint8_t> state_bit = 0b00000000;
};

#endif
