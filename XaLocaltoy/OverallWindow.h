#ifndef OVERALLWINDOW_H
#define OVERALLWINDOW_H

#include "MenuLayertop.h"
#include "UI/uimodule_glWidget.h"
#include "UI/uimodule_codeEditor.h"
#include "UI/uimodule_windowInfo.h"
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
	XA_UIMODULE_GLWidget* _glWindow;
	XA_UIMODULE_WindowInfo* _windowInfo;

	QSize windowsz;
	static QString qssPath;
	static QString iconPath;
	QSize monitor_resolution;
	QSize normalSize = QSize(1600, 900);
	QPoint anchorPos;
};

#endif