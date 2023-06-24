#ifndef XALOCALTOY
#define XALOCALTOY

#define DEBUG

#include <QObject>
#include "OverallWindow.h"
#ifdef DEBUG
#include <QDebug>
#endif // DEBUG


class XaLocaltoy : public QObject
{
	Q_OBJECT

public:
	XaLocaltoy();
	~XaLocaltoy();
private:
	OverallWindow* _window;
};

#endif