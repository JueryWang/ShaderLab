#ifndef XA_LOCALTOY
#define XA_LOCALTOY

#include <QObject>
#include "OverallWindow.h"

class XaLocaltoy : public QObject
{
	Q_OBJECT

public:
	XaLocaltoy();
	~XaLocaltoy();

	void run();
private:
	OverallWindow* _window;
};

#endif