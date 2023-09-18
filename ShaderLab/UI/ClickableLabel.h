/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/

#ifndef UI_CLICKABLE_LABEL_H
#define UI_CLICKABLE_LABEL_H

#include <QLabel>

class ClickableLabel : public QLabel
{
	Q_OBJECT
public:
	ClickableLabel(const QString& path = "", int size = 16);
signals:
	void clicked();
protected:
	void mousePressEvent(QMouseEvent* event) override;
};

#endif
