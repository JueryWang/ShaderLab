#ifndef GL_MODULE_EVSENDFRAME_H
#define GL_MODULE_EVSENDFRAME_H

#include <QEvent>

class EvSendFrame : public QEvent
{
public: 
	EvSendFrame(void* frameData,int size);
	~EvSendFrame();
	static Type eventType;

public:
	uchar* _framedata;
};

#endif
