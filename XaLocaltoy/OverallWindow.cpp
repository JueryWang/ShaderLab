#include "OverallWindow.h"
#include "Utilitys/uitilityDfs.h"
#include <QToolButton>
#include <windows.h>
#include <QFile>
#include <QScreen>
#include <QSpacerItem>
#include <QDesktopWidget>
#include <QApplication>
#include <QRegularExpression>
#include <QSplitter>
#include <iostream>

QString OverallWindow::qssPath;
QString OverallWindow::iconPath;

OverallWindow::OverallWindow() 
{

	init();
	_owlayout = new QVBoxLayout(this);
	_menubar = new MenuLayertop(this);
	_menubar->setFixedHeight(35);
	this->setContentsMargins(10, 0, 10, 0);
	this->setMinimumSize(1600, 900);
	this->resize(1600, 900);
	this->move(anchorPos);
	this->setStyleSheet("background-color:rgba(23,29,37,200)");
	_owlayout->addWidget(_menubar);
	_owlayout->setContentsMargins(0, 0, 0, 0);

	_glWindow = new XA_UIMODULE_GLWidget("default GL Widget",
		this->width()*GL_WIDGET_MAX_WIDTH_R, this->height()*GL_HEIGHT_MAX_HEIGHT_R);
	_glWindow->setMaximumHeight(_glWindow->height());
	XA_UIMODULE_CodeEditor::setEditorSize(this->width() * (1. - GL_WIDGET_MAX_WIDTH_R), this->height()*GL_HEIGHT_MAX_HEIGHT_R);
	XA_UIMODULE_CodeEditor* codeEditorInst = XA_UIMODULE_CodeEditor::getEditor();
	_windowInfo = new XA_UIMODULE_WindowInfo(this, QSize(_glWindow->width(), _glWindow->width()),_STRING_WRAPPER("Ä¬ÈÏÔ´"));
	_windowInfo->setFixedHeight(30);

	QWidget* video_controlpanel_wrapper = new QWidget(this);
	QVBoxLayout* video_controlpanel_layout = new QVBoxLayout();
	QSplitter* splitter_v1 = new QSplitter();
	splitter_v1->resize(_glWindow->width(), this->height());
	splitter_v1->setAttribute(Qt::WA_TranslucentBackground, true);
	splitter_v1->setOrientation(Qt::Vertical);
	splitter_v1->addWidget(_glWindow);
	video_controlpanel_layout->addWidget(_windowInfo, Qt::AlignLeft | Qt::AlignTop);
	QWidget* placeholder = new QWidget();
	video_controlpanel_layout->addWidget(placeholder);
	video_controlpanel_wrapper->setLayout(video_controlpanel_layout);
	splitter_v1->addWidget(video_controlpanel_wrapper);

	splitter_v1->setStretchFactor(0, GL_HEIGHT_MAX_HEIGHT_R*10);
	splitter_v1->setStretchFactor(1, (1. - GL_HEIGHT_MAX_HEIGHT_R)*10);
	QSplitter* splitter_v2 = new QSplitter();
	splitter_v2->resize(codeEditorInst->width(), this->height());
	splitter_v2->setAttribute(Qt::WA_TranslucentBackground, true);
	splitter_v2->setOrientation(Qt::Vertical);
	splitter_v2->addWidget(codeEditorInst);
	splitter_v2->addWidget(new QWidget);
	splitter_v2->setStretchFactor(0, GL_HEIGHT_MAX_HEIGHT_R * 10);
	splitter_v2->setStretchFactor(1, (1. - GL_HEIGHT_MAX_HEIGHT_R) * 10);

	QSplitter* splitter_h = new QSplitter();
	splitter_h->setAttribute(Qt::WA_TranslucentBackground, true);
	splitter_h->addWidget(splitter_v1);
	splitter_h->addWidget(splitter_v2);
	QWidget* spacing = new QWidget();
	_owlayout->addWidget(splitter_h);

	this->setWindowIcon(QIcon("Resources/icon/ShaderLabIco.png"));
	this->setWindowFlags(Qt::FramelessWindowHint);
}

OverallWindow::~OverallWindow()
{
	delete _menubar;
	delete _owlayout;
}

const QSize OverallWindow::getMonitorsz() const
{
	return this->monitor_resolution;
}

void OverallWindow::setFullScreen()
{
	this->resize(monitor_resolution);
	this->move(0, 0);
	QToolButton* tbn = _menubar->findChild<QToolButton*>("zoom Button");
	disconnect(tbn, &QToolButton::clicked, this, &OverallWindow::setFullScreen);
	tbn->setStyleSheet(".QToolButton{background-color:transparent;border:1px solid rgba(255,255,255,0);\
							qproperty-icon: url(Resources/icon/zoomOut.svg);qproperty-iconSize: 20px 20px;}\
				 .QToolButton:hover,pressed,selected{padding:0px 0px;background-color:rgba(61, 68, 80, 1.0)}");
	connect(tbn, &QToolButton::clicked, this, &OverallWindow::rollbackNormal);
}

void OverallWindow::setMinimum()
{
	this->showMinimized();
}


void OverallWindow::rollbackNormal()
{
	this->resize(normalSize);
	this->move(anchorPos);
	QToolButton* tbn = _menubar->findChild<QToolButton*>("zoom Button");
	disconnect(tbn, &QToolButton::clicked, this, &OverallWindow::rollbackNormal);
	tbn->setStyleSheet(".QToolButton{background-color:transparent;border:1px solid rgba(255,255,255,0);\
							qproperty-icon: url(Resources/icon/fullscreen.svg);qproperty-iconSize: 20px 20px;}\
				 .QToolButton:hover,pressed,selected{padding:0px 0px;background-color:rgba(61, 68, 80, 1.0)}");
	connect(tbn, &QToolButton::clicked, this, &OverallWindow::setFullScreen);
}

void OverallWindow::init()
{
	QString s = __FILE__;
	QRegularExpression re;
	re.setPattern("^(.*)\\\\");
	QRegularExpressionMatch match = re.match(s);
	if (match.hasMatch())
	{
		qssPath = re.match(s).captured(0)+"Resources\\qss\\";
		iconPath = re.match(s).captured(0) + "Resources\\icon\\";
		QFile file(qssPath+"default.qss");
		file.open(QIODevice::ReadOnly);
		QString stylesheet = QLatin1String(file.readAll());
		file.close();
		qApp->setStyleSheet(stylesheet);
	}
	QDesktopWidget desktop;
	QRect monitorRct = QApplication::desktop()->availableGeometry();
	monitor_resolution = QSize(monitorRct.width(), monitorRct.height());
	anchorPos = QPoint((monitor_resolution.width() - normalSize.width()) / 2, (monitor_resolution.height() - normalSize.height()) / 2);
	XA_GLMODULE_RENDER::setWindowSize(monitor_resolution.width() * GL_WIDGET_MAX_WIDTH_R, 
		monitor_resolution.height() * GL_HEIGHT_MAX_HEIGHT_R);
}


