#ifndef SHADERLAB_H
#define SHADERLAB_H

#include <QObject>
#include "UI/OverallWindow.h"

class ShaderLab : public QObject
{
    Q_OBJECT
public:
    ShaderLab();
    ~ShaderLab();

    void run();
private:
    SDL_UI_MODULE::OverallWindow* window;
};
#endif