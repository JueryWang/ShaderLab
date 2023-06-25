#include "XaLocaltoy.h"

XaLocaltoy::XaLocaltoy()
{
	_window = new OverallWindow();
}

XaLocaltoy::~XaLocaltoy()
{

}

void XaLocaltoy::run()
{
	_window->show();
}
