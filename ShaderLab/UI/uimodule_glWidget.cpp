#include "uimodule_glWidget.h"
#include "uimodule_windowInfo.h"
#include "../glmodule_EvSendFrame.h"
#include "../GL/glmodule_render.h"
#include "../Utilitys/uitilityDfs.h"
#include "../OverallWindow.h"
#include <QApplication>
#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>
#include <QDebug>

uchar* XA_UIMODULE_GLWidget::_glwgt_pctbuffing;
int XA_UIMODULE_GLWidget::_glwgt_buffingsize;

bool first_resize = true;
float alpha_flick = 255;//alpha value for flicking recording Icon
float alpha_change_step = 5;


XA_UIMODULE_GLWidget::XA_UIMODULE_GLWidget(QWidget* parent, const std::string& title, int width, int height)
	:wgt_width(width),wgt_height(height),_title(title)
{
	this->resize(width, height);
	this->setParent(parent);
	default_size = QSize(width, height);

	_recIcon = QImage(ICOPATH(rec.png));

	_glBackendRender = new XA_GLMODULE_RENDER(title,XA_GL_RGB,this);
	XA_UIMODULE_GLWidget::_glwgt_pctbuffing = (uchar*)malloc(XA_GLMODULE_RENDER::SCR_WIDTH * XA_GLMODULE_RENDER::SCR_HEIGHT * 3 * sizeof(uchar));
	XA_UIMODULE_GLWidget::_glwgt_buffingsize = XA_GLMODULE_RENDER::SCR_WIDTH * XA_GLMODULE_RENDER::SCR_HEIGHT * 3 * sizeof(uchar);
	_renderThread = new QThread(this);
	_glBackendRender->moveToThread(_renderThread);
	  
	connect(this, &XA_UIMODULE_GLWidget::beginGLDraw, _glBackendRender, &XA_GLMODULE_RENDER::contextDraw);

	_renderThread->start();
	emit beginGLDraw();
	this->installEventFilter(this);
}

XA_UIMODULE_GLWidget::~XA_UIMODULE_GLWidget()
{
	_renderThread->terminate();
	_renderThread->quit();
	delete _renderThread;
	delete _glBackendRender;
}

XA_GLMODULE_RENDER* XA_UIMODULE_GLWidget::getRender()
{
	return this->_glBackendRender;
}

void XA_UIMODULE_GLWidget::setWindowInfoPanel(XA_UIMODULE_WindowInfo* inst)
{
	this->_infoPanel = inst;
	inst->glWindow_Inst = this;
	connect(_infoPanel->lockbtn, &QPushButton::clicked, this, &XA_UIMODULE_GLWidget::on_clickLockSize);
	connect(_infoPanel->resetbtn, &QPushButton::clicked, this, &XA_UIMODULE_GLWidget::on_clickResetSize);
}

void XA_UIMODULE_GLWidget::__setMinimumSize(const QSize& size)
{
	minimum_size = size;
	this->setMinimumSize(minimum_size);
}

void XA_UIMODULE_GLWidget::__setMaximumSize(const QSize& size)
{
	maximum_size = size;
	this->setMaximumSize(size);
}

void XA_UIMODULE_GLWidget::__reshow()
{
	emit beginGLDraw();
}

void XA_UIMODULE_GLWidget::__startRecord()
{
	_glBackendRender->__record();
	alpha_flick = 255;
	recording = true;
}

void XA_UIMODULE_GLWidget::__endRecord()
{
	_glBackendRender->__endrecord();
	recording = false;
}

void XA_UIMODULE_GLWidget::on_clickLockSize()
{
	size_locked = true;
	this->setFixedSize(wgt_width, wgt_height);
	_infoPanel->lockbtn->setText(_STRING_WRAPPER("解锁当前宽高"));
	_infoPanel->lockbtn->setIcon(QIcon(ICOPATH(unlock.svg)));
	disconnect(_infoPanel->lockbtn, &QPushButton::clicked, this, &XA_UIMODULE_GLWidget::on_clickLockSize);
	connect(_infoPanel->lockbtn, &QPushButton::clicked, this, &XA_UIMODULE_GLWidget::on_clickUnlockSize);
}

void XA_UIMODULE_GLWidget::on_clickUnlockSize()
{
	size_locked = false;
	this->resize(wgt_width, wgt_height);
	_infoPanel->lockbtn->setText(_STRING_WRAPPER("锁定当前宽高"));
	_infoPanel->lockbtn->setIcon(QIcon(ICOPATH(locksize.svg)));
	disconnect(_infoPanel->lockbtn, &QPushButton::clicked, this, &XA_UIMODULE_GLWidget::on_clickUnlockSize);
	connect(_infoPanel->lockbtn, &QPushButton::clicked, this, &XA_UIMODULE_GLWidget::on_clickLockSize);
	this->setMaximumSize(maximum_size);
	this->setMinimumSize(minimum_size);
}

void XA_UIMODULE_GLWidget::on_clickResetSize()
{
	size_locked = false;
	this->setMaximumSize(maximum_size);
	this->setMinimumSize(minimum_size);

	QResizeEvent *ev = new QResizeEvent(default_size,QSize(wgt_width,wgt_height));
	QApplication::postEvent(this, ev);
	emit resetGLWidget(default_size);

	wgt_width = default_size.width(); wgt_height = default_size.height();

	_infoPanel->lockbtn->setText(_STRING_WRAPPER("锁定当前宽高"));
	_infoPanel->lockbtn->setIcon(QIcon(ICOPATH(locksize.svg)));
	disconnect(_infoPanel->lockbtn, &QPushButton::clicked, this, &XA_UIMODULE_GLWidget::on_clickLockSize);
	disconnect(_infoPanel->lockbtn, &QPushButton::clicked, this, &XA_UIMODULE_GLWidget::on_clickUnlockSize);
	connect(_infoPanel->lockbtn, &QPushButton::clicked, this, &XA_UIMODULE_GLWidget::on_clickLockSize);
}

void XA_UIMODULE_GLWidget::paintEvent(QPaintEvent* event)
{
	QPainterPath clipPath;
	clipPath.addRoundedRect(QRect(0, 0, wgt_width, wgt_height).adjusted(10, 10, -10, -10), 10, 10);
	QPainter painter(this);
	
	painter.setClipPath(clipPath);
	painter.drawImage(QRect(0, 0, wgt_width, wgt_height), _picture);

	if (recording)
	{
		for (int col = 0; col < 48; col++)
		{
			for (int row = 0; row < 48; row++)
			{
				QColor pixelColor = _recIcon.pixelColor(row, col);
				if(pixelColor.alpha() != 0)
					pixelColor.setAlpha(alpha_flick);
				_recIcon.setPixelColor(row, col, pixelColor);
			}
		}
		QRect recIconRegion = QRect(wgt_width - 70, 10, 48, 48);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.drawImage(recIconRegion, _recIcon);
		if ((alpha_flick + alpha_change_step) < 100|| (alpha_flick + alpha_change_step) > 255)
		{
			alpha_change_step = -alpha_change_step;
		}
		alpha_flick = alpha_flick + alpha_change_step;
	}
	painter.end();
}

bool XA_UIMODULE_GLWidget::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == EvSendFrame::eventType)
	{
		_picture = QImage(_glwgt_pctbuffing, _glBackendRender->getWidth(), _glBackendRender->getHeight(), QImage::Format_RGB888);
		this->repaint();
	}

	if (event->type() == QEvent::Resize)
	{
		if (!first_resize && !size_locked)
		{
			QResizeEvent* ev = (QResizeEvent*)event;
			dragged_size = ev->size();
			if(_infoPanel != nullptr)
				_infoPanel->updateInfo(dragged_size);
			wgt_width = dragged_size.width();
			wgt_height = dragged_size.height();
		}
		else
		{
			first_resize = false;
		}

	}
	return QWidget::eventFilter(obj, event);
}

