#include "glmodule_windbufManager.h"

using namespace std;
int XA_GLMODULE_WINBUF::SCR_WIDTH = 0;
int XA_GLMODULE_WINBUF::SCR_HEIGHT = 0;
int XA_GLMODULE_WINBUF::resoultion[2] = {SCR_WIDTH,SCR_HEIGHT};

float deltaTime = 0.0f;
float lastTime = 0.0f;
unsigned int quadVAO = 0;
unsigned int quadVBO;
static void renderQuad(int SCR_WIDTH, int SCR_HEIGHT);

XA_GLMODULE_WINBUF::XA_GLMODULE_WINBUF(int layer, const std::string& title, StorageType type) :_title(title),_type(type)
{
	if ((XA_GLMODULE_WINBUF::SCR_WIDTH == 0) || (XA_GLMODULE_WINBUF::SCR_HEIGHT == 0))
	{
		//Do some Log Record
	}
	switch (type)
	{
		case XA_GL_RGB:
			_windowbuf = malloc(3 * sizeof(char) * SCR_WIDTH * SCR_HEIGHT);
			windowBufSize = 3 * SCR_WIDTH*SCR_HEIGHT;
			break;
		case XA_GL_RGBA:
			_windowbuf = malloc(4 * sizeof(char) * SCR_WIDTH * SCR_HEIGHT);
			windowBufSize = 4 * SCR_WIDTH * SCR_HEIGHT;
			break;
		default:
			_windowbuf = malloc(3 * sizeof(char) * SCR_WIDTH * SCR_HEIGHT);
			windowBufSize = 3 * SCR_WIDTH * SCR_HEIGHT;
			break;
	}

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	static char windowTitle[32];
	sprintf_s(windowTitle,_title.c_str(),NULL,NULL);
	_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, windowTitle, NULL, NULL);
	if (_window == NULL) {
		//Do some Log Record
		glfwTerminate();
	}
	
	//only render one context at the same time
	glfwMakeContextCurrent(_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		//Do some Log Record
	}
}

XA_GLMODULE_WINBUF::~XA_GLMODULE_WINBUF()
{
	glfwTerminate();
	delete _windowbuf;
}

void XA_GLMODULE_WINBUF::addShader(const char* vertexPath, const char* fragmentPath, const char* name)
{
	Shader render(vertexPath, fragmentPath,nullptr,name);
	_shaders.emplace_back(render);
}

void XA_GLMODULE_WINBUF::restart()
{

}

void XA_GLMODULE_WINBUF::start()
{
	this->start();
}

void XA_GLMODULE_WINBUF::pause()
{
	paused.store(false);
}

void XA_GLMODULE_WINBUF::setWindowSize(uint SCR_WIDTH, uint SCR_HEIGHT)
{
	XA_GLMODULE_WINBUF::SCR_WIDTH = SCR_WIDTH;
	XA_GLMODULE_WINBUF::SCR_HEIGHT = SCR_HEIGHT;
	resoultion[0] = SCR_WIDTH;
	resoultion[1] = SCR_HEIGHT;
}

void XA_GLMODULE_WINBUF::flip(uint8_t** buf)
{
	int totalLength = 0;
	int oneLineLength = 0;
	static uint8_t* tmp_rgb = (uint8_t*)malloc(SCR_WIDTH * SCR_HEIGHT * 3);
	static uint8_t* tmp_rgba = (uint8_t*)malloc(SCR_WIDTH * SCR_HEIGHT * 4);

	switch (_type)
	{
	case XA_GL_RGB:
		totalLength = SCR_WIDTH * SCR_HEIGHT * 3;
		oneLineLength = SCR_WIDTH * 3;
		memcpy(tmp_rgb, *buf, totalLength);
		memset(*buf, 0, sizeof(uint8_t) * totalLength);
		for (int i = 0; i < SCR_HEIGHT; i++)
		{
			memcpy(*buf + oneLineLength * i, tmp_rgb + totalLength - oneLineLength * (i + 1), oneLineLength);
		}
		break;
	case XA_GL_RGBA:
		totalLength = SCR_WIDTH * SCR_HEIGHT * 4;
		oneLineLength = SCR_WIDTH * 4;
		memcpy(tmp_rgb, *buf, totalLength);
		memset(*buf, 0, sizeof(uint8_t) * totalLength);
		for (int i = 0; i < SCR_HEIGHT; i++)
		{
			memcpy(*buf + oneLineLength * i, tmp_rgb + totalLength - oneLineLength * (i + 1), oneLineLength);
		}
		break;
	default:
		break;
	}
}

void XA_GLMODULE_WINBUF::contextDraw()
{
	bool tmp;

	_shaders[activeShader].use();
	int activeRenderID = _shaders[activeShader].ID;
	glUniform1iv(glGetUniformLocation(activeRenderID,"iResolution"),1,&resoultion[0]);

	while (tmp = paused.load(memory_order_acquire))
	{
		float time = static_cast<float>(glfwGetTime());
		deltaTime = time - lastTime;
		lastTime = time;

		switch (_type)
		{
			case XA_GL_RGB:
				glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, _windowbuf);
			case XA_GL_RGBA:
				glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, _windowbuf);
		}
		
		glUniform1f(glGetUniformLocation(activeRenderID, "runtime_data.iTime"), time);
		renderQuad(SCR_WIDTH, SCR_HEIGHT);

		glfwSwapBuffers(_window);
		glfwSwapInterval(1);
		glfwPollEvents();
	}
}

static void renderQuad(int SCR_WIDTH, int SCR_HEIGHT)
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