#include "OverallWindow.h"
#include <QFile>
#include <QScreen>
#include <QGuiApplication>
#include <QApplication>
#include <QRegularExpression>
#include <iostream>

QString OverallWindow::qssPath;
QString OverallWindow::iconPath;

OverallWindow::OverallWindow() 
{
	init();
	_owlayout = new QVBoxLayout(this);
	_menubar = new MenuLayertop();
	_owlayout->setMenuBar(_menubar->GetMenus());
	this->setMinimumSize(300, 200);
	this->setStyleSheet("background-color:rgba(23,29,37,200)");

	QRect monitorRct;
	monitorRct = QGuiApplication::primaryScreen()->geometry();
	monitor_resolution = QSize(monitorRct.width(), monitorRct.height());
	this->setWindowIcon(QIcon("Resources/icon/ShaderLabIco.png"));
	this->setWindowTitle("ShaderLab v1.0");
}

OverallWindow::~OverallWindow()
{
}

const QSize OverallWindow::getMonitorsz() const
{
	return this->monitor_resolution;
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
}

