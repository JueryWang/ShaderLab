#include "glmodule_backstatge.h"
#include "gl_defaultDfs.h"
#include "glmodule_render.h"
#include "../UI/uimodule_glWidget.h"
#include <QMutex>
#include <QDebug>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

XA_GLMODULE_BACKSTG* XA_GLMODULE_BACKSTG::_instance=nullptr;
static QMutex taskLocker;
using namespace std;

XA_GLMODULE_BACKSTG::XA_GLMODULE_BACKSTG()
{
}

XA_GLMODULE_BACKSTG::~XA_GLMODULE_BACKSTG()
{
}

XA_GLMODULE_BACKSTG* XA_GLMODULE_BACKSTG::getBackStage()
{
	if (_instance == nullptr)
	{
		_instance = new XA_GLMODULE_BACKSTG();
		_instance->start(QThread::NormalPriority);
	}
	return _instance;
}

void XA_GLMODULE_BACKSTG::addTask(std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK> newTask)
{
	taskLocker.lock();
	_task_queue.push(newTask);
	taskLocker.unlock();
}

void XA_GLMODULE_BACKSTG::deleteTexture(int idx)
{
	auto texIter = _textures.begin();

	for (; texIter != _textures.end(); texIter++)
	{
		if (((*texIter).index == idx) && ((*texIter).address != nullptr))
		{
			(*texIter).index = -1;
			delete (*texIter).address;
		}
		(*texIter).status = TEXTURE_ST_DEPRECATED;
	}
	if (texIter == _textures.end())
	{
		//Do some Log here
	}
}

void XA_GLMODULE_BACKSTG::run()
{
	while (true)
	{
		if (taskLocker.try_lock())
		{
			while (!_task_queue.empty())
			{
				std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK> crt_task = _task_queue.front();
				switch (crt_task.second.type)
				{
					case XA_GL_DRAWFRONT:
					{
						handleDrawFront(crt_task);
						break;
					}
					case XA_GL_LOADTEXTURE:
					{
						handleLoadTexture(crt_task);
						break;
					}
					case XA_GL_COMPILE_SHADER:
					{
						handleCompileShader(crt_task);
						break;
					}
					default:
						break;
				}
				_task_queue.pop();
			}
			taskLocker.unlock();
		}
		QThread::msleep(100);
	}
}

void XA_GLMODULE_BACKSTG::handleDrawFront(const std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK>& crt_task)
{
	crt_task.first->setReciver(crt_task.second.param.drawFront_param.target_reciver);
	crt_task.first->__start();
	crt_task.first->rd_state = ACTIVE_DRAW_FRONT;
}

void XA_GLMODULE_BACKSTG::handleLoadTexture(const std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK>& crt_task)
{
	int width, height, nrComponents;
	unsigned char* data = stbi_load(crt_task.second.param.loadTexture_param.texture_path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		int index = crt_task.second.param.loadTexture_param.index;
		_textures[index].index = index;
		_textures[index].address = data;
		_textures[index].width = width;
		_textures[index].height = height;
		_textures[index].format = format;
		_textures[index].status = TEXTURE_ST_UNLOAD;
		crt_task.first->__update();
	}
}

void XA_GLMODULE_BACKSTG::handleCompileShader(const std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK>& crt_task)
{
	crt_task.first->__exit();
	crt_task.first->rd_state = INACTIVE;
	crt_task.first->_vs_source = crt_task.second.param.compileTask_parm.vs_path;
	crt_task.first->_fs_source = crt_task.second.param.compileTask_parm.fs_path;
	XA_UIMODULE_GLWidget* glwgt = (XA_UIMODULE_GLWidget*)crt_task.first->_reciver;
	glwgt->__reshow();
	crt_task.first->__start();
}
