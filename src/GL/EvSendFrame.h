#ifndef EVSENDFRAME_H
#define EVSENDFRAME_H

#include <QEvent>

namespace SDL_GL_MODULE
{
    class EvSendFrame : public QEvent   //Just for Inform
    {
    public:
        EvSendFrame();
        static Type eventType;
    public:
    };
}
#endif
