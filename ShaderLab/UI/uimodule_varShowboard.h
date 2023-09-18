/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef UI_VAR_SHOWBOARD_H
#define UI_VAR_SHOWBOARD_H

#include <QWidget>
#include <QMap>
#include <QVariant>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QGridLayout>
#include "ClickableLabel.h"


class XA_UIMODULE_ShowBoard : public QWidget
{
	friend class OverallWindow;
	Q_OBJECT
public:
	XA_UIMODULE_ShowBoard(int width);
	~XA_UIMODULE_ShowBoard();
	void setWidth(int width);
	void setVariantMap(const QMap<QString, QVariant>& var_mp);
private:
	void clearLayout(QLayout* layout);

public slots:
	void on_ShowVariables();
	void on_unShowVariables();
private:
	ClickableLabel* _icon;
	QScrollArea* _scrollarea;
	QWidget* _showboard;
	QGridLayout* var_grid;
	int width;
	
	QMap<QString, QVariant> _var_mp;
	QPropertyAnimation* _animation;

};

#endif