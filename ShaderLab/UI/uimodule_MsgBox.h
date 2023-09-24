/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef UI_MSGBOX_H
#define UI_MSGBOX_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <functional>
#include <QLabel>

class XA_UIModule_QUEST_BOX : public QWidget
{
	Q_OBJECT
public:
	XA_UIModule_QUEST_BOX(QWidget* parent, const QString& mainMsg, const QString& attachedMsg);
	~XA_UIModule_QUEST_BOX();
	static XA_UIModule_QUEST_BOX* question(QWidget* parent, const QString& title, const QString& text,const QSize& windowSz,
		const QStringList& btnTexts,const std::vector<std::function<void(void)>> &callbacks);
	void setButtonTexts(const QStringList& texts);
	void setCallbacks(const std::vector<std::function<void(void)>>& callbacks);

private:
	void on_clcBtn1();
	void on_clcBtn2();
	void on_clcBtn3();
	bool eventFilter(QObject* obj, QEvent* event) override;

private:
	QWidget* _wgt;
	QLabel* _icon;

	QPushButton* btn1;
	QPushButton* btn2;
	QPushButton* btn3;

	std::function<void(void)> cb1;
	std::function<void(void)> cb2;
	std::function<void(void)> cb3;
	bool _isDrag = false;
	QPoint _offsetPoint = QPoint(0,0);
	int _exec_res = -1;
};
#endif // 
