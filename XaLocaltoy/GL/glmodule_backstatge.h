#ifndef GL_MODULE_BACKSTG_H
#define GL_MODULE_BACKSTG_H

#include "gl_defaultDfs.h"
#include <queue>
#include <QThread>

class XA_GLMODULE_BACKSTG : public QThread
{
public:
	XA_GLMODULE_BACKSTG(QObject* parent);
	~XA_GLMODULE_BACKSTG();

private:
	/*
	* multi Window Instance Rendering need to be reconstructed by vulkan
	  std::priority_queue<GL_windowBuffer*> hierarchy_layer;
	*/

};
#endif
