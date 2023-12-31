#include "glmodule_render.h"
#include "glmodule_backstatge.h"
#include "glmodule_EvSendFrame.h"
#include "Utilitys/AssetsManager/Video/utils_videoPlayer.h"
#include "UI/uimodule_editorpage.h"
#include "UI/uimodule_glWidget.h"
#include <QApplication>
#include <QThread>
#include <memory>
#include <QDebug>
using namespace std;

std::atomic<int> XA_GLMODULE_RENDER::SCR_WIDTH = 0;
std::atomic<int> XA_GLMODULE_RENDER::SCR_HEIGHT = 0;
int XA_GLMODULE_RENDER::resolution[2];
std::timed_mutex glLocker;
float deltaTime = 0.0f;
float lastTime = 0.0f;
int frame = 0;
unsigned int quadVAO = 0;
unsigned int quadVBO;

inline void flip(uint8_t** buf, int context_width, int context_height);

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
	//glfwWindowHint(GLFW_SAMPLES, 4);
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

void XA_GLMODULE_RENDER::setTitle(const char* title)
{
	_title = title;
}

void XA_GLMODULE_RENDER::initShader(XA_GL_SHADER_INFO* shader_info)
{
	if (!shader_info->inited)
	{
		for (auto ref_shader : shader_info->reference)
		{
			if (!ref_shader->inited) initShader(ref_shader);
			else return;
		}
		setShaderOrder(shader_info, shader_info->order);
		shader_info->inited = true;
		shader_info->program = Shader(shader_info->path_vertex, shader_info->path_fragment);
		for (int i = 0; i < 4; i++)
		{
			if (shader_info->textures[i].address != nullptr)
			{
				std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK> loadtextureTsk;
				loadtextureTsk.first = this;
				loadtextureTsk.second.type = XA_GL_LOADTEXTURE;
				loadtextureTsk.second.param.loadTexture_param.index = i;
				XA_GLMODULE_BACKSTG::getBackStage()->addTask(loadtextureTsk);
			}
		}
	}
}

bool shader_sortFunc(const XA_GL_SHADER_INFO& s1,XA_GL_SHADER_INFO& s2)
{
	return s1.order < s2.order;
}


void XA_GLMODULE_RENDER::addShader(const XA_GL_SHADER_INFO& shader_info)
{
	XA_GL_SHADER_INFO temp_info = shader_info;
	initShader(&temp_info);

	shaders.emplace_back(temp_info);
	sort(shaders.begin(), shaders.end(), shader_sortFunc);
	
	setStatus(RENDER_UPDATE, true);
	updateGLContex();
}

int XA_GLMODULE_RENDER::getWidth() const
{
	return SCR_WIDTH.load(memory_order_consume);
}

int XA_GLMODULE_RENDER::getHeight() const
{
	return SCR_HEIGHT.load(memory_order_consume);
}

void flip(uint8_t** buf, int context_width, int context_height)
{
	static uint8_t* tmp_rgb = (uint8_t*)malloc(XA_GLMODULE_RENDER::SCR_WIDTH * XA_GLMODULE_RENDER::SCR_HEIGHT * 3);
	
	int totalLength = context_width * context_height * 3;
	int oneLineLength = context_width * 3;
	memcpy(tmp_rgb, *buf, totalLength);
	memset(*buf, 0, sizeof(uint8_t) * totalLength);
	for (int i = 0; i < context_height; i++)
	{
		memcpy(*buf + oneLineLength * i, tmp_rgb + totalLength - oneLineLength * (i + 1), oneLineLength);
	}
}

void inline XA_GLMODULE_RENDER::setStatus(uint8_t bit_mask,bool open_bit)
{
	if (open_bit)
	{
		uint8_t temp = state_bit.load(memory_order_consume) | bit_mask;
		state_bit.store(temp, memory_order_release);
	}
	else
	{
		uint8_t temp = state_bit.load(memory_order_consume);
		temp = temp & (bit_mask ^ temp);
		state_bit.store(temp, memory_order_release);
	}

}

bool inline XA_GLMODULE_RENDER::getFlag(XA_RENDER_FLAG_BIT_POS pos)
{
	return state_bit.load(memory_order_consume) & (1 << pos);
}

void XA_GLMODULE_RENDER::__pause()
{
	setStatus(RENDER_PAUSE,true);
	anchor_time = glfwGetTime();
}

void XA_GLMODULE_RENDER::__start()
{
	setStatus( RENDER_PAUSE | RENDER_EXIT, false);
	glfwSetTime(anchor_time);
}

void XA_GLMODULE_RENDER::__restart()
{
	glfwSetTime(0.0f);
	setStatus(RENDER_PAUSE | RENDER_EXIT, false);
	frame = 0;
}

void XA_GLMODULE_RENDER::__update()
{
	setStatus(RENDER_UPDATE,true);
}

void XA_GLMODULE_RENDER::__record()
{
	XA_VIDEO_PLAYER::get_player()->setRecordParam(_title.c_str(),XA_GLMODULE_RENDER::SCR_WIDTH, XA_GLMODULE_RENDER::SCR_HEIGHT);
	setStatus(RENDER_RECORD, true);
}

void XA_GLMODULE_RENDER::__endrecord()
{
	setStatus(RENDER_RECORD, false);
	emit XA_GLMODULE_BACKSTG::getBackStage()->videoRecordDone();
}

void XA_GLMODULE_RENDER::__exit()
{
	setStatus(RENDER_EXIT, true);
}


void XA_GLMODULE_RENDER::__reset(const QSize& newSize)
{
	XA_GLMODULE_RENDER::SCR_WIDTH.store(newSize.width(),memory_order_release);
	XA_GLMODULE_RENDER::SCR_HEIGHT.store(newSize.height(), memory_order_release);
	XA_GLMODULE_RENDER::resolution[0] = newSize.width();
	XA_GLMODULE_RENDER::resolution[1] = newSize.height();
}

inline void RGBA2RBG(uchar** src, uchar** dst)
{

}

void XA_GLMODULE_RENDER::contextDraw()
{
	rd_state = ACTIVE_DRAW_FRONT;
	static char windowTitle[32];
	static int windowCount = 0;
	sprintf_s(windowTitle, _title.c_str(), NULL, NULL);

	static GLFWwindow *window_inst = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, windowTitle, NULL, NULL);
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

	if (_shader != nullptr)
	{
		glDeleteProgram(_shader->ID);
		delete _shader;
	}
	_shader = new Shader(this->_vs_source.c_str(),this->_fs_source.c_str());
	_shader->use();
	
	loadTextures();

	glUniform2iv(glGetUniformLocation(_shader->ID, "iResolution"), 1, &resolution[0]);
	//glEnable(GL_SAMPLE_SHADING);
	//glMinSampleShading(0.7f);

	while (!getFlag(BIT_EXIT_POS))
	{
		if (getFlag(BIT_PAUSE_POS))
		{
			QThread::msleep(10);
			continue;
		}
		if (getFlag(BIT_UPDATE_POS))
		{
			updateGLContex();
			setStatus(RENDER_UPDATE, false);
		}
		auto time = static_cast<float>(glfwGetTime());
		deltaTime = time - lastTime;
		lastTime = time;

		int context_width = SCR_WIDTH.load(memory_order_consume);
		int context_height = SCR_HEIGHT.load(memory_order_consume);

		glUniform1f(glGetUniformLocation(_shader->ID, "runtime_data.iTime"), time);
		glUniform1f(glGetUniformLocation(_shader->ID, "runtime_data.iFrame"), frame);
		renderQuad(context_width,context_height);
		frame++;

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
		flip(&((uint8_t*)_windowbuf), context_width, context_height);

		memcpy(XA_UIMODULE_GLWidget::_glwgt_pctbuffing, _windowbuf, windowBufSize);//copy to unique memory to avoid memory leak
		
		auto event = std::make_unique<EvSendFrame>();
		QApplication::postEvent(_reciver, event.release(), Qt::HighEventPriority);

		if (getFlag(BIT_RECORD_POS))
		{
			std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK> recordTask;
			recordTask.first = this;
			recordTask.second.type = XA_GL_RECORD;
			int frame_size = context_width * context_height * 3 * sizeof(uchar);
			uchar* newFrame = (uchar*)malloc(frame_size);
			switch (_type)
			{
				case XA_GL_RGB:
					memcpy(newFrame, _windowbuf, frame_size);
					recordTask.second.param.recordTask_param.frameAddr = newFrame;
					break;
				case XA_GL_RGBA:
					//ignore alpha bit
					RGBA2RBG(&newFrame, &((uchar*)_windowbuf));
					recordTask.second.param.recordTask_param.frameAddr = newFrame;
					break;
			}
			XA_GLMODULE_BACKSTG::getBackStage()->addTask(recordTask);
		}

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

void XA_GLMODULE_RENDER::loadTextures()
{
	std::vector<int> removeIdxs;
	int pos = 0;
	for (XA_GL_TEXTURE_INFO& texture_info : XA_GLMODULE_BACKSTG::getBackStage()->_textures)
	{
		if (texture_info.status == TEXTURE_ST_UNLOAD)
		{
			glActiveTexture(GL_TEXTURE0 + texture_info.index);
			unsigned int textureID;
			glGenTextures(1, &textureID);
			texture_info.textureID = textureID;
		
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, texture_info.format, texture_info.width, 
			texture_info.height, 0, texture_info.format, GL_UNSIGNED_BYTE, texture_info.address);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			texture_info.status = TEXTURE_ST_LOADED;
		}
		if(texture_info.status == TEXTURE_ST_DEPRECATED)
		{
			glDeleteTextures(1, &texture_info.textureID);
			removeIdxs.push_back(pos);
		}
		pos++;
	}

	//Remove DEPRECATED textures
	auto begin = XA_GLMODULE_BACKSTG::getBackStage()->_textures.begin();
	for (int remove_idx : removeIdxs)
	{
		XA_GLMODULE_BACKSTG::getBackStage()->_textures.erase(begin+ remove_idx);
	}
}


void XA_GLMODULE_RENDER::updateGLContex()
{
	loadTextures();
	loadShaders();
}

void XA_GLMODULE_RENDER::loadShaders()
{
	for (XA_GL_SHADER_INFO shader : shaders)
	{
		shader.program.use();
		glUniform2iv(glGetUniformLocation(shader.program.ID, "iResolution"), 1, &resolution[0]);
		if (shader.type == XA_GL_SCRIPT_BUFFERA || shader.type == XA_GL_SCRIPT_BUFFERB || shader.type == XA_GL_SCRIPT_BUFFERC || shader.type == XA_GL_SCRIPT_BUFFERD)
		{
			glGenFramebuffers(1, &shader.fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, shader.fbo);
			glGenTextures(1, &shader.fbo_Tex);
			glBindTexture(GL_TEXTURE_2D, shader.fbo_Tex);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, shader.fbo_Tex, 0);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
		}
	}
}