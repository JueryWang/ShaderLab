#ifndef GL_MODULE_WINBUF
#define GL_MODULE_WINBUF

#include <QObject>
#include <vector>
#include <map>
#include <string>
#include <atomic>
#include <shader.h>
#include "gl_defaultDfs.h"

class XA_GLMODULE_WINBUF : public QObject
{
	friend class XA_GLMODULE_BACKSTG;
	Q_OBJECT
public:
	XA_GLMODULE_WINBUF(int layer,const std::string& title,StorageType type = XA_GL_RGB);
	~XA_GLMODULE_WINBUF();
	void addShader(const char* vertexPath, const char* fragmentPath,const char* name);
	void restart();
	void start();
	void pause();
	static void setWindowSize(uint SCR_WIDTH,uint SCR_HEIGHT);
	inline void flip(uint8_t** buf);

public slots:
	void contextDraw();
	/*
	* multi Window Instance need to be reconstructed by vulkan
	bool operator<(const XA_GLMODULE_WINBUF& other)
	{
		return _layer < other._layer;
	}
	*/
private:
	void* _windowbuf;
	GLFWwindow* _window;
	StorageType _type;
	std::vector<Shader> _shaders;
	std::string _title;
	/*
	* multi Window Instance need to be reconstructed by vulkan
		int _layer;//
	*/
	int windowBufSize;
	int activeShader = 0;

	static int SCR_WIDTH;
	static int SCR_HEIGHT;
	static int resoultion[2];

	std::map<std::string, unsigned int> vao_mp;
	std::map<std::string, unsigned int> vbo_mp;

	bool writable = true;
	bool readable = true;
	
	std::atomic<bool> paused = false;
};

#endif
