#ifndef UI_CODE_EDITOR_H
#define UI_CODE_EDITOR_H

#include <memory>
#include <QTabWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QLabel>
#include <QTabBar>

class QFile;
class QFont;
class QStatusBar;
class QsciScintilla;
class XA_UIMODULE_EditorPage;

class TabLabelEditor : public QWidget
{
	Q_OBJECT
public:
	TabLabelEditor();
	void setOriLabel(int tabIdx,const QString& orilabel);
signals:
	void labelChanged(int tabIdx,const QString& newLabel);

private:
	QString label;
	int tabIdx;
	QLineEdit* _fromTab;
	QLineEdit* _toTab;
};

class XA_UIMODULE_CodeEditor : public QTabWidget
{
	friend class XA_UIMODULE_EditorPage;
	Q_OBJECT
public:
	static void setEditorSize(int width, int height);
	static XA_UIMODULE_CodeEditor* getEditor();
private:
	void set_new_tab(const QString& path, bool is_new_file = false);
	QsciScintilla* get_new_page();

protected:
	virtual void dragEnterEvent(QDragEnterEvent* event);
	virtual void dropEvent(QDropEvent* event);

public slots:
	void on_closeTab(int index);
	void on_show_hide_Tab();
	void on_copy();
	void on_newfile();
	void on_savefile();
	void on_saveas();
	void on_addNewScript();
	void on_clickTab(int tabIdx);
	void on_pageLabelChanged(int index,const QString& newLabel);
private:
	XA_UIMODULE_CodeEditor();
	~XA_UIMODULE_CodeEditor();


public:
	static XA_UIMODULE_CodeEditor* _codeEditor;
	static TabLabelEditor* _tabLabelEditor;
	static int editor_width;
	static int editor_height;
	int crt_idx = -1;

	QFont* _editor_font;
	std::unique_ptr<QFile> _current_file;

	XA_UIMODULE_EditorPage* _current_page;
};

#endif
