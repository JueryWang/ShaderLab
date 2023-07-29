#include "glmodule_render.h"
#include "glmodule_EvSendFrame.h"
#include <QApplication>
#include <memory>
#include <QDebug>
using namespace std;

int XA_GLMODULE_RENDER::SCR_WIDTH = 0;
int XA_GLMODULE_RENDER::SCR_HEIGHT = 0;
int XA_GLMODULE_RENDER::resolution[2];

float deltaTime = 0.0f;
float lastTime = 0.0f;
unsigned int quadVAO = 0;
unsigned int quadVBO;

XA_GLMODULE_RENDER::XA_GLMODULE_RENDER(const std::string& title, StorageType type,QWidget* reciver) :_title(title),_type(type)
{
	this->_reciver = (QObject*)reciver;
	if (XA_GLMODULE_RENDER::SCR_WIDTH == 0 || XA_GLMODULE_RENDER::SCR_HEIGHT == 0)
	{
		//Do some Log operation
		return;
	}
	switch (type)
	{
		case XA_GL_RGB:
			windowBufSize = SCR_WIDTH * SCR_HEIGHT * 3 * sizeof(uchar);
			_windowbuf = malloc(windowBufSize);
			break;
		case XA_GL_RGBA:
			windowBufSize = SCR_WIDTH * SCR_HEIGHT * 3 * sizeof(uchar);
			break;
		default:
			break;
	}	


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
}

XA_GLMODULE_RENDER::~XA_GLMODULE_RENDER()
{
	glfwTerminate();
	delete _windowbuf;
}

//void XA_GLMODULE_RENDER::addShader(const char* vertexPath, const char* fragmentPath, const char* name)
//{
//	Shader *render = new Shader(vertexPath,fragmentPath);
//	_shaders.push_back(render);
//}

void XA_GLMODULE_RENDER::setWindowSize(int SCR_WIDTH, int SCR_HEIGHT)
{
	XA_GLMODULE_RENDER::SCR_WIDTH = SCR_WIDTH;
	XA_GLMODULE_RENDER::SCR_HEIGHT = SCR_HEIGHT;
	XA_GLMODULE_RENDER::resolution[0] = SCR_WIDTH;
	XA_GLMODULE_RENDER::resolution[1] = SCR_HEIGHT;
}

void XA_GLMODULE_RENDER::flip(uint8_t** buf)
{
	static uint8_t* tmp_rgb = (uint8_t*)malloc(SCR_WIDTH * SCR_HEIGHT * 3);
	int totalLength = SCR_WIDTH * SCR_HEIGHT * 3;
	int oneLineLength = SCR_WIDTH * 3;
	memcpy(tmp_rgb, *buf, totalLength);
	memset(*buf, 0, sizeof(uint8_t) * totalLength);
	for (int i = 0; i < SCR_HEIGHT; i++)
	{
		memcpy(*buf + oneLineLength * i, tmp_rgb + totalLength - oneLineLength * (i + 1), oneLineLength);
	}
}

void XA_GLMODULE_RENDER::pause()
{

}

void XA_GLMODULE_RENDER::start()
{

}

void XA_GLMODULE_RENDER::restart()
{

}

void XA_GLMODULE_RENDER::contextDraw()
{
	static char windowTitle[32];
	sprintf_s(windowTitle, _title.c_str(), NULL, NULL);
	_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, windowTitle, NULL, NULL);
	qDebug() << SCR_WIDTH << "," << SCR_HEIGHT;
	if (_window == NULL) {
		//Do some Log Record
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		//Do some Log Record
		return;
	}
	_shader = new Shader("Shader/birthday_cake.vs", "Shader/birthday_cake.fs");
	_shader->use();
	glUniform2iv(glGetUniformLocation(_shader->ID, "iResolution"), 1, &resolution[0]);

	bool tmp;
	while (tmp = !paused.load(memory_order_acquire))
	{
		auto time = static_cast<float>(glfwGetTime());
		deltaTime = time - lastTime;
		lastTime = time;
		glUniform1f(glGetUniformLocation(_shader->ID, "runtime_data.iTime"), time);
		renderQuad();
		
		switch (_type)
		{
			case XA_GL_RGB:
				glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, _windowbuf);
				break;
			case XA_GL_RGBA:
				glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, _windowbuf);
				break;
			default:
				break;
		}
		flip(&((uint8_t*)_windowbuf));

		auto event = std::make_unique<EvSendFrame>(_windowbuf, windowBufSize);
		QApplication::postEvent(_reciver, event.release(), Qt::HighEventPriority);

		glfwSwapBuffers(_window);
		glfwSwapInterval(1);
		glfwPollEvents();
	}
}

void XA_GLMODULE_RENDER::renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, (float)SCR_HEIGHT,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, (float)SCR_WIDTH,(float)SCR_HEIGHT,
			 1.0f, -1.0f, 0.0f, (float)SCR_WIDTH, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}