#ifndef OVERALLWINDOW
#define OVERALLWINDOW

#include "MenuLayertop.h"

class OverallWindow :public QWidget
{
	Q_OBJECT
public:
	OverallWindow();
	~OverallWindow();
	const QSize getMonitorsz() const;
	void setQssStyle();
private:
	MenuLayertop* layer_periphery;
	QSize windowsz;
	QSize monitor_resolution;
};

#endif