/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/

#ifndef GL_MODULE_BACKSTG_H
#define GL_MODULE_BACKSTG_H

#include "gl_defaultDfs.h"
#include <utility>
#include <queue>
#include <array>
#include <QList>
#include <QThread>

class XA_GLMODULE_RENDER;

class XA_GLMODULE_BACKSTG : public QThread
{
	friend class XA_GLMODULE_RENDER;
	Q_OBJECT
public:
	static XA_GLMODULE_BACKSTG* getBackStage();
	void addTask(std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK> newTask);
	void deleteTexture(int idx);

signals:
	void shaderParsedone();
	void videoRecordDone();

protected:
	void run() override;

private:
	XA_GLMODULE_BACKSTG();
	~XA_GLMODULE_BACKSTG();
	void handleDrawFront(const std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK>& crt_task);
	void handleLoadTexture(const std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK>& crt_task);
	void handleCompileShader(const std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK>& crt_task);
	void handleAVRecord(const std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK>& crt_task);
	/*
	* multi Window Instance Rendering need to be reconstructed by vulkan
	  std::priority_queue<GL_windowBuffer*> hierarchy_layer;
	*/
private:
	static XA_GLMODULE_BACKSTG* _instance;

	std::queue<std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK>> _task_queue; 
	std::array<XA_GL_TEXTURE_INFO,10> _textures;
};
#endif
