/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef UI_WINDOW_INFO_H
#define UI_WINDOW_INFO_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class XA_UIMODULE_GLWidget;
class XA_UIMODULE_WindowInfo : public QWidget
{
	friend class XA_UIMODULE_GLWidget;
	Q_OBJECT
public:
	XA_UIMODULE_WindowInfo(QWidget* parent,const QSize &initsize,const QString &source);
	void updateInfo(const QSize& newsize);
public slots:
	void on_clcRecord();
	void on_clcEndRecord();
	void on_clcSoundOn();
	void on_clcSoundOff();
private:
	QLabel* crtSource_show;
	QLabel* crtSource_info;
	QLabel* crtSize_show;
	QLabel* crtSize_info;
	QPushButton* lockbtn;
	QPushButton* resetbtn;
	QPushButton* recbtn;
	QPushButton* soundbtn;

	QSize _crtSoucesize;
	QSize _crtWindowsize;
	QString _source;

	XA_UIMODULE_GLWidget* glWindow_Inst = nullptr;
};

#endif