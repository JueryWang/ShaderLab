#include "glmodule_EvSendFrame.h"
QEvent::Type EvSendFrame::eventType = (QEvent::Type)QEvent::registerEventType(QEvent::User + 1);

EvSendFrame::EvSendFrame():QEvent(Type(eventType))
{
}


