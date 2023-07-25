#include "OverallWindow.h"
#include <QToolButton>
#include <QDebug>
#include <windows.h>
#include <QFile>
#include <QScreen>
#include <QDesktopWidget>
#include <QApplication>
#include <QRegularExpression>
#include <iostream>

QString OverallWindow::qssPath;
QString OverallWindow::iconPath;

OverallWindow::OverallWindow() 
{
	init();
	_owlayout = new QVBoxLayout(this);
	_menubar = new MenuLayertop(this);
	this->setMinimumSize(1000, 600);
	this->resize(1000, 600);
	this->setStyleSheet("background-color:rgba(23,29,37,200)");
	_menubar->setParent(this);
	_owlayout->addWidget(_menubar);
	_owlayout->setContentsMargins(0, 0, 0, 0);

	QDesktopWidget desktop;
	QRect monitorRct = QApplication::desktop()->availableGeometry();
	monitor_resolution = QSize(monitorRct.width(), monitorRct.height());
	anchorPos = QPoint((monitor_resolution.width()-normalSize.width())/2, (monitor_resolution.height() - normalSize.height())/2);

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
	SetConsoleOutputCP(CP_UTF8);
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
}


