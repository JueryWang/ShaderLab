#ifndef XA_LOCALTOY
#define XA_LOCALTOY

#include <QObject>
#include "OverallWindow.h"

class ShaderLab : public QObject
{
	Q_OBJECT

public:
	ShaderLab();
	~ShaderLab();

	void run();
private:
	OverallWindow* _window;
};

#endif