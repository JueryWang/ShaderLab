/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/

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
