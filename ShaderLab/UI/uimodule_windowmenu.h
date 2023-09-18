/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef UI_MENU_WINDOW_H
#define UI_MENU_WINDOW_H

#include <QMenu>

class XA_UIModule_WINDOWMenu : public QMenu
{
	Q_OBJECT
public:
	XA_UIModule_WINDOWMenu(QString title, bool showIcon = false, const char* iconpath = nullptr);
	~XA_UIModule_WINDOWMenu();

private://no need to add slots flag because there are no args between click signal and click event
	void on_clcNewWindow();
	void on_clcOpenTerminal();
	void on_clcCloseTerminal();
	void on_clcPreference();

	bool terminalOpened = false;
};


#endif

