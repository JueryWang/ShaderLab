#include "ShaderLab.h"

using namespace SDL_UI_MODULE;

ShaderLab::ShaderLab()
{
    window = new OverallWindow();
}

ShaderLab::~ShaderLab()
{
    delete window;
}

void ShaderLab::run()
{
    window->show();
    window->setFocus();
}
