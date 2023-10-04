#include "glmodule_backstatge.h"
#include "gl_defaultDfs.h"
#include "glmodule_render.h"
#include "Utilitys/uitilityDfs.h"
#include "UI/uimodule_glWidget.h"
#include "UI/uimodule_codeEditor.h"
#include "Utilitys/Parser/utils_shaderParser.h"
#include "Utilitys/AssetsManager/Video/utils_videoPlayer.h"
#include <Qsci/qsciscintilla.h>
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
	_task_queue.push(newTask);
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
		glDeleteTextures(1, const_cast<GLuint *>(&(*texIter).textureID));
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
					case XA_GL_RECORD:
					{
						handleAVRecord(crt_task);
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

		XA_GL_TEXTURE_INFO tex_info;
		tex_info.index = crt_task.second.param.loadTexture_param.index;
		tex_info.address = data;
		tex_info.width = width;
		tex_info.height = height;
		tex_info.format = format;
		tex_info.status = TEXTURE_ST_UNLOAD;
		_textures.push_back(tex_info);

		crt_task.first->__update();
	}
}

void XA_GLMODULE_BACKSTG::handleCompileShader(const std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK>& crt_task)
{
	char vs_path[128];
	char fs_path[128];

	switch (crt_task.second.param.compileTask_parm.rule)
	{
		case parser::ShaderToy:
		{
			XA_UTILS_ShaderParser* parser = XA_UTILS_ShaderParser::getParser();
			XA_UIMODULE_CodeEditor* codeEditor = XA_UIMODULE_CodeEditor::getEditor();

			parser->setContextParserRule(parser::ShaderToy);

			if (codeEditor->comboShader())
			{
				parser->setComboParse(true);
				crt_task.first->setTitle("Image");
				for (auto it = codeEditor->scriptTypesMp.begin(); it != codeEditor->scriptTypesMp.end(); it++)
				{
					if (!codeEditor->scriptTypeslist.contains(it.key()))
					{
						QByteArray s = it.key().toLatin1();

						strcpy(vs_path, USER_TEMPORARY_SHADER_PATH);
						strcat(vs_path, "/");
						strcat(vs_path, s.data());
						strcat(vs_path, "_combo.vert");

						strcpy(fs_path, USER_TEMPORARY_SHADER_PATH);
						strcat(fs_path, "/");
						strcat(fs_path, s.data());
						strcat(fs_path, "_combo.frag");

						parser->setCurrentFileName(QString::fromLatin1(s.data()) + "_combo.vert", parser::VERTEX);
						parser->parse("", parser::VERTEX);

						parser->setCurrentFileName(QString::fromLatin1(s.data()) + "_combo.frag", parser::FRAGMENT); 
						parser->parse(codeEditor->findPagebyName(it.key()), parser::FRAGMENT);
					}
				}
				emit shaderParsedone();
			}
			else
			{
				parser->setComboParse(false);

				QsciScintilla* page = (QsciScintilla*)codeEditor->currentWidget();
				QFileInfo fileInfo(codeEditor->_current_file->fileName());
				QString filename = fileInfo.baseName();
				QByteArray s = filename.toLatin1();
				crt_task.first->setTitle(s.data());

				strcpy(vs_path, USER_TEMPORARY_SHADER_PATH);
				strcat(vs_path, "/");
				strcat(vs_path, s.data());
				strcat(vs_path, ".vert");

				strcpy(fs_path, USER_TEMPORARY_SHADER_PATH);
				strcat(fs_path, "/");
				strcat(fs_path, s.data());
				strcat(fs_path, ".frag");

				parser->setCurrentFileName(QString::fromLatin1(s.data()) + ".vert", parser::VERTEX);
				parser->parse("", parser::VERTEX);

				parser->setCurrentFileName(QString::fromLatin1(s.data()) + ".frag", parser::FRAGMENT);
				parser->parse(page->text(), parser::FRAGMENT);
				emit shaderParsedone();
			}

		crt_task.first->__exit();
		crt_task.first->_vs_source = vs_path;
		crt_task.first->_fs_source = fs_path;
		crt_task.first->rd_state = INACTIVE;
		XA_UIMODULE_GLWidget* glwgt = (XA_UIMODULE_GLWidget*)crt_task.first->_reciver;
		glwgt->__reshow();
		crt_task.first->__start();
		break;
	
		}
	}
}

void XA_GLMODULE_BACKSTG::handleAVRecord(const std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK>& crt_task)
{
	XA_VIDEO_PLAYER::get_player()->writeRecord(crt_task.second.param.recordTask_param.frameAddr);
}
