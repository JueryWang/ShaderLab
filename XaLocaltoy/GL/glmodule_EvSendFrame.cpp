#include "glmodule_EvSendFrame.h"

QEvent::Type EvSendFrame::eventType = (QEvent::Type)QEvent::registerEventType(QEvent::User + 1);

EvSendFrame::EvSendFrame(void* frameData,int size):QEvent(Type(eventType))
{
	_framedata = (uchar*)malloc(size);
	memcpy(_framedata, frameData, size);
}

