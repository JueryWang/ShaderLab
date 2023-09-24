/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef  MENULAYERTOP_H
#define  MENULAYERTOP_H
#include <QMenuBar>
#include <QHBoxLayout>

class OverallWindow;
class MenuLayertop : public QWidget
{
	friend class OverallWindow;
	Q_OBJECT
public: 
	MenuLayertop(OverallWindow* parent = NULL);
	~MenuLayertop();

protected:
	bool eventFilter(QObject* obj, QEvent* event) override;

private:
	void on_clcClose();
private:
	bool _isDrag = false;
	QPoint _offsetPoint = QPoint(0, 0);

	QMenuBar* _topMenus;
	QHBoxLayout* _layout;

	OverallWindow* _parentInst;
};

#endif
