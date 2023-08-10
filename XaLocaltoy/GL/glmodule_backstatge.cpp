#include  "glmodule_backstatge.h"
#include "glmodule_render.h"

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
	while (!_task_queue.empty())
	{
		std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK> crt_task = _task_queue.front();
		switch (crt_task.second.type)
		{
			case XA_GL_DRAWFRONT:
				crt_task.first->setReciver(crt_task.second.parm.drawFront_param.target_reciver);
				connect(this, &XA_GLMODULE_BACKSTG::beginGLDraw, crt_task.first, &XA_GLMODULE_RENDER::contextDraw);
				crt_task.first->rd_state = ACTIVE_DRAW_FRONT;
				break;
			case XA_GL_LOADTEXTURE:

			default:
				break;
		}
		_task_queue.pop();
	}
}