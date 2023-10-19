/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef OVERALLWINDOW_H
#define OVERALLWINDOW_H

#include "MenuLayertop.h"
#include "UI/uimodule_glWidget.h"
#include "UI/uimodule_codeEditor.h"
#include "UI/uimodule_windowInfo.h"
#include "UI/uimodule_varShowboard.h"
#include "UI/uimodule_AssetsWindow.h"
#include <QVBoxLayout>

class QSplitter;

class OverallWindow :public QWidget
{
	Q_OBJECT
public:
	OverallWindow();
	~OverallWindow();
	const QSize getMonitorsz() const;
	void init();
	void rollbackNormal();
	void setFullScreen();
	void setMinimum();

public slots:
	void on_restGLWidget(const QSize& size);
	void on_compileCode();
	void on_updateVarBoard();
	void on_changeAssetsBar(XA_UIMODULE_ASSET_BAR* newBar);

private:
	QVBoxLayout* _owlayout;
	MenuLayertop* _menubar;
	XA_UIMODULE_GLWidget* _glWindow;
	XA_UIMODULE_WindowInfo* _windowInfo;
	XA_UIMODULE_ShowBoard* _varboard;

	QSplitter* splitter_v1;
	QSplitter* splitter_v2;
	QSplitter* splitter_h;

	QSize windowsz;
	static QString qssPath;
	static QString iconPath;
	QSize monitor_resolution;
	QSize normalSize = QSize(1600, 900);
	QPoint anchorPos;
};

#endif