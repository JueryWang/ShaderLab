/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
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
	virtual void keyPressEvent(QKeyEvent* e);
	virtual void dragEnterEvent(QDragEnterEvent* event);
	virtual void dropEvent(QDropEvent* event);

public:
	QString label;
private:
	void setCustomStyle();
private:
	static QsciLexerCPP* _cpplexer;
	static QsciAPIs* _api;
};
#endif