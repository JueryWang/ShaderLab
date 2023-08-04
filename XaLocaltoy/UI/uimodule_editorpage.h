#ifndef UI_EDITOR_PAGE_H
#define UI_EDITOR_PAGE_H

#include <Qsci/qsciscintilla.h>
#include <memory>

class QKeyEvent;
class QsciLexerCPP;
class QsciAPIs;
class QFile;

class XA_UIMODULE_EditorPage : public QsciScintilla
{
public:
	XA_UIMODULE_EditorPage();
	~XA_UIMODULE_EditorPage();
	static void initFont(const QString& font_name, int size);
	static void initIntellisense();
protected:
	void keyPressEvent(QKeyEvent* e) override;
private:
	static QsciLexerCPP* _cpplexer;
	static QsciAPIs* _api;
};
#endif