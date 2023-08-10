#include "glmodule_render.h"
#include "glmodule_EvSendFrame.h"
#include <QApplication>
#include <memory>
using namespace std;

std::atomic<int> XA_GLMODULE_RENDER::SCR_WIDTH = 0;
std::atomic<int> XA_GLMODULE_RENDER::SCR_HEIGHT = 0;
int XA_GLMODULE_RENDER::resolution[2];
static bool first_resize = true;

float deltaTime = 0.0f;
float lastTime = 0.0f;
unsigned int quadVAO = 0;
unsigned int quadVBO;

XA_GLMODULE_RENDER::XA_GLMODULE_RENDER(const std::string& title, StorageType type, QObject* reciver) :_title(title),_type(type)
{
	this->_reciver = reciver;
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
	delete _shader;
	delete _windowbuf;
}

void XA_GLMODULE_RENDER::setReciver(QObject* reciver)
{
	this->_reciver = reciver;
}

//void XA_GLMODULE_RENDER::addShader(const char* vertexPath, const char* fragmentPath, const char* name)
//{
//	Shader *render = new Shader(vertexPath,fragmentPath);
//	_shaders.push_back(render);
//}

int XA_GLMODULE_RENDER::getWidth() const
{
	return SCR_WIDTH.load(memory_order_consume);
}

int XA_GLMODULE_RENDER::getHeight() const
{
	return SCR_HEIGHT.load(memory_order_consume);
}

void XA_GLMODULE_RENDER::flip(uint8_t** buf, int context_width, int context_height)
{
	static uint8_t* tmp_rgb = (uint8_t*)malloc(SCR_WIDTH * SCR_HEIGHT * 3);
	
	int totalLength = context_width * context_height * 3;
	int oneLineLength = context_width * 3;
	memcpy(tmp_rgb, *buf, totalLength);
	memset(*buf, 0, sizeof(uint8_t) * totalLength);
	for (int i = 0; i < context_height; i++)
	{
		memcpy(*buf + oneLineLength * i, tmp_rgb + totalLength - oneLineLength * (i + 1), oneLineLength);
	}
}

void XA_GLMODULE_RENDER::pause()
{
	paused.store(true, memory_order_release);
}

void XA_GLMODULE_RENDER::start()
{

}

void XA_GLMODULE_RENDER::restart()
{

}

void XA_GLMODULE_RENDER::reset(const QSize& newSize)
{
	XA_GLMODULE_RENDER::SCR_WIDTH.store(newSize.width(),memory_order_release);
	XA_GLMODULE_RENDER::SCR_HEIGHT.store(newSize.height(), memory_order_release);
	XA_GLMODULE_RENDER::resolution[0] = newSize.width();
	XA_GLMODULE_RENDER::resolution[1] = newSize.height();
}

void XA_GLMODULE_RENDER::contextDraw()
{
	rd_state = ACTIVE_DRAW_FRONT;
	static char windowTitle[32];
	static int windowCount = 0;
	sprintf_s(windowTitle, _title.c_str(), NULL, NULL);

	GLFWwindow *window_inst = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, windowTitle, NULL, NULL);
	_window.reset(window_inst);
	if (window_inst == NULL) {
		//Do some Log Record
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(_window.release());
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		//Do some Log Record
		return;
	}
	_shader = new Shader("Shader/birthday_font.vert", "Shader/birthday_font.frag");
	_shader->use();
	glUniform2iv(glGetUniformLocation(_shader->ID, "iResolution"), 1, &resolution[0]);

	bool tmp;
	while (!paused.load(memory_order_acquire))
	{
		auto time = static_cast<float>(glfwGetTime());
		deltaTime = time - lastTime;
		lastTime = time;

		int context_width = SCR_WIDTH.load(memory_order_consume);
		int context_height = SCR_HEIGHT.load(memory_order_consume);

		glUniform1f(glGetUniformLocation(_shader->ID, "runtime_data.iTime"), time);
		renderQuad(context_width,context_height);
		

		switch (_type)
		{
			case XA_GL_RGB:
				glReadPixels(0, 0, context_width, context_height, GL_RGB, GL_UNSIGNED_BYTE, _windowbuf);
				break;
			case XA_GL_RGBA:
				glReadPixels(0, 0, context_width, context_height, GL_RGBA, GL_UNSIGNED_BYTE, _windowbuf);
				break;
			default:
				break;
		}
		flip(&((uint8_t*)_windowbuf),context_width,context_height);

		auto event = std::make_unique<EvSendFrame>(_windowbuf, windowBufSize);
		QApplication::postEvent(_reciver, event.release(), Qt::HighEventPriority);

		glfwSwapBuffers(window_inst);
		glfwSwapInterval(1);
		glfwPollEvents();
	}
}

void XA_GLMODULE_RENDER::renderQuad(int context_width, int context_height)
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, (float)context_height,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, (float)context_width,(float)context_height,
			 1.0f, -1.0f, 0.0f, (float)context_width, 0.0f,
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
