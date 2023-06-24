#include "XaLocaltoy.h"

XaLocaltoy::XaLocaltoy()
{
	_window = new OverallWindow();
	qDebug() << _window->getMonitorsz();
}

XaLocaltoy::~XaLocaltoy()
{

}
