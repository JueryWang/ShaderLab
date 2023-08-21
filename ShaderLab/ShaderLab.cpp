#include "ShaderLab.h"

ShaderLab::ShaderLab()
{
	_window = new OverallWindow();
}

ShaderLab::~ShaderLab()
{

}

void ShaderLab::run()
{
	_window->show();
}
