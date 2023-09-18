/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef UI_PROFILE_MENU_H
#define UI_PROFILE_MENU_H
#include <QToolButton>

class XA_UIModule_ProfileMenu : public QToolButton
{
public:
	XA_UIModule_ProfileMenu(QString title);
	~XA_UIModule_ProfileMenu();

private://no need to add slots flag because there are no args between click signal and click event
	void on_clcProfile();
	void on_clcCheckDevice();
	void on_clcCheckShader();
	void on_clcSwitchAccount();
	void on_clcExitLoggin();
};
#endif
