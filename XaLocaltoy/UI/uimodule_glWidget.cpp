#include "uimodule_glWidget.h"
#include "../glmodule_EvSendFrame.h"
#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>
#include <QDebug>

uchar* XA_UIModule_GLWidget::_glwgt_pctbuffing;
int XA_UIModule_GLWidget::_glwgt_buffingsize;
bool first_resize = true;

XA_UIModule_GLWidget::XA_UIModule_GLWidget(const char* title, int width, int height):wgt_width(width),wgt_height(height)
{
	this->resize(width, height);
	
	XA_UIModule_GLWidget::_glwgt_pctbuffing = (uchar*)malloc(XA_GLMODULE_RENDER::SCR_WIDTH * XA_GLMODULE_RENDER::SCR_HEIGHT * 3 * sizeof(uchar));
	XA_UIModule_GLWidget::_glwgt_buffingsize = XA_GLMODULE_RENDER::SCR_WIDTH * XA_GLMODULE_RENDER::SCR_HEIGHT * 3 * sizeof(uchar);

	_glBackendRender = new XA_GLMODULE_RENDER(title,XA_GL_RGB,this);
	_renderThread = new QThread(this);
	_glBackendRender->moveToThread(_renderThread);
	//_glBackendRender->addShader("Shader/birthday_cake.vs", "Shader/birthday_cake.fs","birthdayCake");


	connect(this, &XA_UIModule_GLWidget::beginGLDraw, _glBackendRender, &XA_GLMODULE_RENDER::contextDraw);

	_renderThread->start();
	emit beginGLDraw();
	this->installEventFilter(this);
}

XA_UIModule_GLWidget::~XA_UIModule_GLWidget()
{
	_renderThread->terminate();
	_renderThread->quit();
	delete _renderThread;
	delete _glBackendRender;
}

void XA_UIModule_GLWidget::paintEvent(QPaintEvent* event)
{
	QPainterPath clipPath;
	clipPath.addRoundedRect(QRect(0, 0, wgt_width, wgt_height).adjusted(10, 10, -10, -10), 10, 10);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setClipPath(clipPath);
	painter.drawImage(QRect(0, 0, wgt_width, wgt_height), _picture);
	painter.end();
}

bool XA_UIModule_GLWidget::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == EvSendFrame::eventType)
	{
		EvSendFrame* ev = (EvSendFrame*)event;
		memcpy(_glwgt_pctbuffing,ev->_framedata, XA_UIModule_GLWidget::_glwgt_buffingsize);//copy to unique memory to avoid memory leak
		_picture = QImage(_glwgt_pctbuffing, XA_GLMODULE_RENDER::SCR_WIDTH, XA_GLMODULE_RENDER::SCR_HEIGHT, QImage::Format_RGB888);
		this->repaint();
	}

	if (event->type() == QEvent::Resize)
	{
		if (!first_resize)
		{
			QResizeEvent* ev = (QResizeEvent*)event;
			XA_GLMODULE_RENDER::setWindowSize(ev->size().width(), ev->size().height());
		}
		else
		{
			first_resize = false;
		}

	}
	return QWidget::eventFilter(obj, event);
}

