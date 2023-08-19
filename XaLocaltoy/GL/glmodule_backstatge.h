#ifndef GL_MODULE_BACKSTG_H
#define GL_MODULE_BACKSTG_H

#include "gl_defaultDfs.h"
#include <utility>
#include <vector>
#include <queue>
#include <QThread>

class XA_GLMODULE_RENDER;

class XA_GLMODULE_BACKSTG : public QThread
{
	friend class XA_GLMODULE_RENDER;
	Q_OBJECT
public:
	static XA_GLMODULE_BACKSTG* getBackStage();
	XA_GLMODULE_RENDER* requestRenderer();
	void addTask(std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK> newTask);
	void deleteTexture(int idx);

protected:
	void run() override;

private:
	XA_GLMODULE_BACKSTG();
	~XA_GLMODULE_BACKSTG();
	/*
	* multi Window Instance Rendering need to be reconstructed by vulkan
	  std::priority_queue<GL_windowBuffer*> hierarchy_layer;
	*/
private:
	static XA_GLMODULE_BACKSTG* _instance;

	std::vector<XA_GLMODULE_RENDER*> _renders;
	std::queue<std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK>> _task_queue; 

	QList<XA_GL_TEXTURE_INFO> _textures;

};
#endif
