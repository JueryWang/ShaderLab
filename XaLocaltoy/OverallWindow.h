#ifndef OVERALLWINDOW_H
#define OVERALLWINDOW_H

#include "MenuLayertop.h"
#include "UI/uimodule_glWidget.h"
#include <QVBoxLayout>

class OverallWindow :public QWidget
{
	Q_OBJECT
public:
	OverallWindow();
	~OverallWindow();
	const QSize getMonitorsz() const;
	void init();
	void rollbackNormal();
	void setFullScreen();
	void setMinimum();
private:
	QVBoxLayout* _owlayout;
	MenuLayertop* _menubar;
	XA_UIModule_GLWidget* _glWindow;

	QSize windowsz;
	static QString qssPath;
	static QString iconPath;
	QSize monitor_resolution;
	QSize normalSize = QSize(1000, 600);
	QPoint anchorPos;
};

#endif