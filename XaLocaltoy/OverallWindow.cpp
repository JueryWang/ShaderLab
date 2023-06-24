#include "OverallWindow.h"
#include <QFile>
#include <QScreen>
#include <QGuiApplication>
#include <iostream>

OverallWindow::OverallWindow()
{
	QRect monitorRct;
	monitorRct = QGuiApplication::primaryScreen()->geometry();
	monitor_resolution = QSize(monitorRct.width(), monitorRct.height());
}

OverallWindow::~OverallWindow()
{

}

const QSize OverallWindow::getMonitorsz() const
{
	return this->monitor_resolution;
}

void OverallWindow::setQssStyle()
{
	QFile file()
}

