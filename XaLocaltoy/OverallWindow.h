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
private:
	QVBoxLayout* _owlayout;
	MenuLayertop* _menubar;

	QSize windowsz;
	static QString qssPath;
	static QString iconPath;
	QSize monitor_resolution;
};

#endif