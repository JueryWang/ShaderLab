#ifndef OVERALLWINDOW
#define OVERALLWINDOW

#include "MenuLayertop.h"
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

	QSize windowsz;
	static QString qssPath;
	static QString iconPath;
	QSize monitor_resolution;
	QSize normalSize = QSize(1000, 600);
	QPoint anchorPos;
};

#endif