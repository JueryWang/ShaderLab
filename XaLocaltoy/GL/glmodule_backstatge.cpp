#include  "glmodule_backstatge.h"
#include "glmodule_render.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

XA_GLMODULE_BACKSTG* XA_GLMODULE_BACKSTG::_instance=nullptr;
using namespace std;

XA_GLMODULE_BACKSTG::XA_GLMODULE_BACKSTG()
{
	_renders.resize(MAX_GL_RENDERER);
	for (int i = 0; i < MAX_GL_RENDERER; i++)
	{
		XA_GLMODULE_RENDER* render = new XA_GLMODULE_RENDER("BACKSTG RENDERER "+i);
		_renders.emplace_back(render);
	}
}

XA_GLMODULE_BACKSTG::~XA_GLMODULE_BACKSTG()
{
	for (int i = 0; i < MAX_GL_RENDERER; i++)
	{
		delete _renders[i];
	}
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

XA_GLMODULE_RENDER* XA_GLMODULE_BACKSTG::requestRenderer()
{
	for (int i = 0; i < MAX_GL_RENDERER; i++)
	{
		if (_renders[i]->rd_state == INACTIVE)
			return _renders[i];
	}
	return nullptr;
}

void XA_GLMODULE_BACKSTG::run()
{
	while (1)
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
						crt_task.first->setReciver(crt_task.second.param.drawFront_param.target_reciver);
						crt_task.first->__start();
						crt_task.first->rd_state = ACTIVE_DRAW_FRONT;
						break;
					}
					case XA_GL_LOADTEXTURE:
					{
						unsigned int textureID;
						glGenTextures(1, &textureID);

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

							glBindTexture(GL_TEXTURE_2D, textureID);
							glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
							glGenerateMipmap(GL_TEXTURE_2D);

							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
						}
						crt_task.first->_textures[crt_task.second.param.loadTexture_param.texture_name] = textureID;
						break;
					}
					case XA_GL_COMPILE_SHADER:
					{
						if (crt_task.first->_shader != nullptr)
						{
							glDeleteProgram(crt_task.first->_shader->ID);
						}
						crt_task.first->rd_state = INACTIVE;
						crt_task.first->__exit();
						crt_task.first->_vs_source = crt_task.second.param.compileTask_parm.vs_path;
						crt_task.first->_fs_source = crt_task.second.param.compileTask_parm.fs_path;
						break;
					}
					default:
						break;
				}
				_task_queue.pop();
			}
			taskLocker.unlock();
		}
		QThread::msleep(10);
	}
}