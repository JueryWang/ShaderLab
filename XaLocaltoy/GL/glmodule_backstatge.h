#ifndef GL_MODULE_BACKSTG
#define GL_MODULE_BACKSTG

#include "gl_defaultDfs.h"
#include "glmodule_windbufManager.h"
#include <queue>
#include <QThread>

class XA_GLMODULE_BACKSTG : public QThread
{
	typedef XA_GLMODULE_WINBUF GL_windowBuffer;
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
