/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef UI_MENU_LOOK_H
#define UI_MENU_LOOK_H

#include <QMenu>

class XA_UIModule_LOOKMenu : public QMenu
{
	Q_OBJECT
public:
	XA_UIModule_LOOKMenu(QString title, bool showIcon = false, const char* iconpath = nullptr);
	~XA_UIModule_LOOKMenu();

private://no need to add slots flag because there are no args between click signal and click event
	void on_clcRepo();
	void on_clcQueryFriend();
	void on_clcNewFriend();
	void on_clcQueryFriendReq();
	void on_clcGetHistory();//triggered only once
	void on_clcShowScrCap();
	void on_clcShowNews();
};

#endif // UI_MENU_LOOK
