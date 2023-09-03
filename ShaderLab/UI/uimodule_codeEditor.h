#ifndef UI_CODE_EDITOR_H
#define UI_CODE_EDITOR_H

#include <deque>
#include <utility>
#include <memory>
#include <QTabWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QMenu>
#include <QLabel>
#include <QTabBar>

class QFile;
class QFont;
class QStatusBar;
class QsciScintilla;
class XA_UIMODULE_EditorPage;
class TabLabelEditor;

class XA_UIMODULE_CodeEditor : public QTabWidget
{
	friend class XA_UIMODULE_EditorPage;
	Q_OBJECT
public:
	static void setEditorSize(int width, int height);
	static XA_UIMODULE_CodeEditor* getEditor();
	void savefile();
private:
	void set_new_tab(const QString& path, bool is_new_file = false);
	QsciScintilla* get_new_page();
	void saveas();

protected:
	virtual void dragEnterEvent(QDragEnterEvent* event);
	virtual void dropEvent(QDropEvent* event);

public slots:
	void on_closeTab(int index);
	void on_copy();
	void on_newfile();
	void on_addNewScript();
	void on_clickTab(int tabIdx);
	void on_pageLabelChanged(int index,const QString& newLabel);
	void on_showClearMenu(const QPoint& pos);
	void on_clearRightTab();
	void on_excludeTab();

private:
	XA_UIMODULE_CodeEditor();
	~XA_UIMODULE_CodeEditor();


public:
	static XA_UIMODULE_CodeEditor* _codeEditor;
	static TabLabelEditor* _tabLabelEditor;
	static QMenu* _custmMenu;
	static int editor_width;
	static int editor_height;

	QFont* _editor_font;
	std::unique_ptr<QFile> _current_file;

	XA_UIMODULE_EditorPage* _current_page;
	std::deque<std::pair<bool,std::string>> saved_state;
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
