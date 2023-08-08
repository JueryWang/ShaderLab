#ifndef UI_CODE_EDITOR_H
#define UI_CODE_EDITOR_H

#include <memory>
#include <QMap>
#include <QTabWidget>

class QFile;
class QFont;
class QStatusBar;
class QsciScintilla;
class XA_UIMODULE_EditorPage;

class XA_UIMODULE_CodeEditor : public QTabWidget
{
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

private:
	XA_UIMODULE_CodeEditor();
	~XA_UIMODULE_CodeEditor();


public:
	static XA_UIMODULE_CodeEditor* _codeEditor;
	static int editor_width;
	static int editor_height;

	QFont* _editor_font;
	std::unique_ptr<QFile> _current_file;

	XA_UIMODULE_EditorPage* _current_page;
};


#endif
