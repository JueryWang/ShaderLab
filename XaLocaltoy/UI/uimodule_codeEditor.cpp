#include "ui_defaultDfs.h"
#include "uimodule_codeEditor.h"
#include "uimodule_editorpage.h"
#include <QFontDatabase>
#include <QFileDialog>
#include <QMimeData>
#include <QMimeDatabase>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMap>
#include <QFont>
#include <QDir>
#include <QDebug>
XA_UIMODULE_CodeEditor* XA_UIMODULE_CodeEditor::_codeEditor;
static QMap<QString, QString> registeredFonts;
int XA_UIMODULE_CodeEditor::editor_width = 0;
int XA_UIMODULE_CodeEditor::editor_height = 0;

XA_UIMODULE_CodeEditor::XA_UIMODULE_CodeEditor()
{

	this->resize(editor_width, editor_height);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setAcceptDrops(true);

	int fontId = QFontDatabase::addApplicationFont(FONTPATH(Cascadia.ttf));
	QStringList font_list = QFontDatabase::applicationFontFamilies(fontId);
	registeredFonts["Cascadia"] = font_list[0];
	
	_editor_font = new QFont();
	_editor_font->setPointSize(10);
	//this->setFont(*_editor_font);
	_editor_font->setFamily(registeredFonts["Cascadia"]);


	fontId = QFontDatabase::addApplicationFont(FONTPATH(Caviar_Dreams_Bold.ttf));
	font_list = QFontDatabase::applicationFontFamilies(fontId);
	registeredFonts["Caviar_Dreams_Bold"] = font_list[0];
	
	XA_UIMODULE_EditorPage::initFont(registeredFonts["Cascadia"], 10);
	XA_UIMODULE_EditorPage::initIntellisense();

	this->setContentsMargins(0, 0, 0, 0);
	this->setTabsClosable(true);
	this->setMovable(true);
	this->setDocumentMode(true);
	_editor_font->setFamily(registeredFonts["Caviar_Dreams_Bold"]);
	_editor_font->setPointSize(10);
	this->setFont(*_editor_font);
	connect(this, &QTabWidget::tabCloseRequested, this, &XA_UIMODULE_CodeEditor::on_closeTab);

	this->set_new_tab(nullptr, true);
}

XA_UIMODULE_CodeEditor::~XA_UIMODULE_CodeEditor()
{

}

void XA_UIMODULE_CodeEditor::setEditorSize(int width, int height)
{
	XA_UIMODULE_CodeEditor::editor_width = width;
	XA_UIMODULE_CodeEditor::editor_height = height;
}

XA_UIMODULE_CodeEditor* XA_UIMODULE_CodeEditor::getEditor()
{
	if (_codeEditor == nullptr)
	{
		_codeEditor = new XA_UIMODULE_CodeEditor();
	}
	return _codeEditor;
}

void XA_UIMODULE_CodeEditor::set_new_tab(const QString& path, bool is_new_file /*= false*/)
{
	QsciScintilla* editor = this->get_new_page();

	if (is_new_file)
	{
		this->addTab(editor, "untitled");
		this->setCurrentIndex(this->count() - 1);
		_current_file = std::make_unique<QFile>("untitled");
		return;
	}

	QFileInfo fileInfo(path);
	if (!fileInfo.isFile())
		return;

	int DirSeprator = path.lastIndexOf("/");
	QString file_name = path.right(path.length() - DirSeprator - 1);
	QMimeDatabase db;
	QMimeType mime = db.mimeTypeForFile(file_name);

	int tab_count = this->count();
	for (int i = 0; i < tab_count; i++)
	{
		if (this->tabText(i) == file_name)
		{
			this->setCurrentIndex(i);
			this->_current_file = std::make_unique<QFile>(path);
			return;
		}
	}

	this->addTab(editor, file_name);
	
	QFile* file = new QFile(path);
	if (!is_new_file)
	{
		if (!file->open(QIODevice::ReadOnly))
		{
			//Do some Log here
		}
		editor->setText(file->readAll());
		file->close();
	}
	this->_current_file = std::make_unique<QFile>(path);
	this->setCurrentIndex(this->count() - 1);
}

QsciScintilla* XA_UIMODULE_CodeEditor::get_new_page()
{
	_current_page = new XA_UIMODULE_EditorPage();

	return _current_page;
}

void XA_UIMODULE_CodeEditor::dragEnterEvent(QDragEnterEvent* event)
{
	static QStringList acceptedFileTypes = {"vert","tesc","tese","geom","frag","comp","mesh","task"};
	if (event->mimeData()->hasUrls() && event->mimeData()->urls().count() == 1)
	{
		QFileInfo file(event->mimeData()->urls().at(0).toLocalFile());
		if (acceptedFileTypes.contains(file.suffix().toLower()))
		{
			event->acceptProposedAction();
		}
	}
}

void XA_UIMODULE_CodeEditor::dropEvent(QDropEvent* event)
{
	QFileInfo droped_file(event->mimeData()->urls().at(0).toLocalFile());
	QString pre_file = _current_file->fileName();
	QString new_file = droped_file.absoluteFilePath();
	qDebug() << new_file;

	bool is_existed = false;
	
	int tab_count = this->count();
	for (int i = 0; i < tab_count; i++)
	{
		if (this->tabText(i) == new_file)
		{
			this->setCurrentIndex(i);
			is_existed = true;
			break;
		}
	}

	_current_file->setFileName(new_file);
	if (_current_file->open(QIODevice::ReadOnly))
	{
		if (is_existed)
		{
			QsciScintilla* editor = (QsciScintilla*)this->currentWidget();
			editor->setText(_current_file->readAll());
		}
		else
		{
			this->set_new_tab(new_file, false);
		}
	}
	else
	{
		_current_file->setFileName(pre_file);
	}
}

void XA_UIMODULE_CodeEditor::on_closeTab(int index)
{
	this->removeTab(index);
}

void XA_UIMODULE_CodeEditor::on_show_hide_Tab()
{

}

void XA_UIMODULE_CodeEditor::on_copy()
{
	QsciScintilla* editor = (QsciScintilla*)this->currentWidget();

	if (editor)
		editor->copy();
}

void XA_UIMODULE_CodeEditor::on_newfile()
{
	set_new_tab(nullptr, true);
}

void XA_UIMODULE_CodeEditor::on_savefile()
{
	if (_current_file == nullptr && this->count() > 0)
	{
		this->on_saveas();
		return;
	}

	QsciScintilla* editor = (QsciScintilla*)this->currentWidget();
	if (_current_file->open(QIODevice::WriteOnly))
	{
		_current_file->write(editor->text().toUtf8());
	}
	_current_file->close();
}

void XA_UIMODULE_CodeEditor::on_saveas()
{
	QsciScintilla* editor = (QsciScintilla*)this->currentWidget();
	if (editor == nullptr)
		return;

	//replace QDir::currentPath() with last user save path
	QString file_path = QFileDialog::getSaveFileName(this, "Save as", QDir::currentPath());
	if (!file_path.length())
		return;

	QFile* path = new QFile(file_path);
	if (path->open(QIODevice::WriteOnly))
	{
		path->write(editor->text().toUtf8());
	}
	path->close();
	this->setTabText(this->currentIndex(), path->fileName());
	_current_file.reset(path);
}

