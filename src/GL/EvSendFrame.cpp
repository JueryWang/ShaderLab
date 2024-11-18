#include "EvSendFrame.h"

namespace SDL_GL_MODULE
{
    QEvent::Type SDL_GL_MODULE::EvSendFrame::eventType = (QEvent::Type)QEvent::registerEventType(QEvent::User + 1);

    SDL_GL_MODULE::EvSendFrame::EvSendFrame() : QEvent(Type(eventType)){
    }
}
