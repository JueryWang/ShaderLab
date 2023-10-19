/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/

#ifndef UI_CODE_EDITOR_H
#define UI_CODE_EDITOR_H

#include <utility>

#include <memory>
#include <queue>
#include <QTabWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QMenu>
#include <QListView>
#include <QLabel>
#include <QTabBar>
#include "GL/gl_defaultDfs.h"

class QFile;
class QFont;
class QStatusBar;
class QsciScintilla;
class XA_UIMODULE_EditorPage;
class XA_UIMODULE_ASSET_BAR;
class TabLabelEditor;
class OverallWindow;

class XA_UIMODULE_CodeEditor : public QTabWidget
{
	friend class XA_UIMODULE_EditorPage;
	Q_OBJECT

public:
	static XA_UIMODULE_CodeEditor* getEditor();
	void setEditorSize(int width, int height);
	void savefile();
	void setOverallWindow(OverallWindow* ovWindow);
	bool comboShader() const;
	const QString findPagebyName(const QString& name);
	QStringList buffersInUse();

private:
	QsciScintilla* get_new_page();
	void set_new_tab(const QString& path, XA_GL_SCRIPT_TYPE type = XA_GL_SCRIPT_NOTYPE,bool is_new_file = true);
	void appendTab(QsciScintilla* editor,const QString& tabLabel);
	void duplicateRenameScript();
	void closeBuildinTab();
	void saveas();

protected:
	virtual void dragEnterEvent(QDragEnterEvent* event);
	virtual void dropEvent(QDropEvent* event);
	virtual bool eventFilter(QObject* obj, QEvent* e);

signals:
	void setOvWindowAssetsBar(XA_UIMODULE_ASSET_BAR* newBar);

public slots:
	void on_closeTab(int index);
	void on_copy();
	void on_addNewScript();
	void on_clickTab(int tabIdx);
	void on_pageLabelChanged(int index,const QString& newLabel);
	void on_clickTypeScript(const QModelIndex &modelIdx);

private:
	XA_UIMODULE_CodeEditor();
	~XA_UIMODULE_CodeEditor();

public:
	static XA_UIMODULE_CodeEditor* _codeEditor;
	static QStringList bufferLabels;

	TabLabelEditor* _tabLabelEditor;
	QStringList scriptTypeslist;
	QMap<QString, XA_GL_SCRIPT_TYPE> scriptTypesMp;
	QListView* _typeScriptView;
	int editor_width;
	int editor_height;

	QFont* _editor_font;
	std::unique_ptr<QFile> _current_file;

	XA_UIMODULE_EditorPage* _current_page;
	std::deque<std::pair<bool,std::string>> saved_state;

	OverallWindow* _ovWindow = nullptr;
	QMap<XA_UIMODULE_EditorPage*, XA_UIMODULE_ASSET_BAR*> page_bar_map;
};

class TabLabelEditor : public QWidget
{
	Q_OBJECT
public:
	TabLabelEditor(XA_UIMODULE_CodeEditor *codeEditor);
	void setOriLabel(int tabIdx, const QString& orilabel);

signals:
	void labelChanged(int tabIdx, const QString& newLabel);

private:
	QString label;
	int tabIdx;
	QLineEdit* _fromTab;
	QLineEdit* _toTab;
	XA_UIMODULE_CodeEditor* _code_editor;
};

#endif
