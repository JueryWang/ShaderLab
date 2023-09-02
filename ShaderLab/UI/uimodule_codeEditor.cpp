#include "ui_defaultDfs.h"
#include "../Utilitys/uitilityDfs.h"
#include "uimodule_codeEditor.h"
#include "uimodule_editorpage.h"
#include <QFontDatabase>
#include <QFileDialog>
#include <QMimeData>
#include <QMimeDatabase>
#include <QDragEnterEvent>
#include <QTimer>
#include <QDropEvent>
#include <QTabBar>
#include <QMap>
#include <QFont>
#include <QToolBar>
#include <QLabel>
#include <QDir>
#include <QHBoxLayout>
#include <QDebug>
XA_UIMODULE_CodeEditor* XA_UIMODULE_CodeEditor::_codeEditor;
TabLabelEditor* XA_UIMODULE_CodeEditor::_tabLabelEditor;
int XA_UIMODULE_CodeEditor::editor_width = 0;
int XA_UIMODULE_CodeEditor::editor_height = 0;


XA_UIMODULE_CodeEditor::XA_UIMODULE_CodeEditor()
{

	this->resize(editor_width, editor_height);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setAcceptDrops(true);

	int fontId = QFontDatabase::addApplicationFont(FONTPATH(Cascadia.ttf));
	QStringList font_list = QFontDatabase::applicationFontFamilies(fontId);
	global_font_mp["Cascadia"] = font_list[0];
	
	_editor_font = new QFont();
	_editor_font->setPointSize(10);
	//this->setFont(*_editor_font);
	_editor_font->setFamily(global_font_mp["Cascadia"]);


	fontId = QFontDatabase::addApplicationFont(FONTPATH(Caviar_Dreams_Bold.ttf));
	font_list = QFontDatabase::applicationFontFamilies(fontId);
	global_font_mp["Caviar_Dreams_Bold"] = font_list[0];
	
	XA_UIMODULE_EditorPage::initFont(global_font_mp["Cascadia"], 10);
	XA_UIMODULE_EditorPage::initIntellisense();

	this->setContentsMargins(0, 0, 0, 0);
	this->setTabsClosable(true);
	this->setMovable(false);
	this->setDocumentMode(true);
	_editor_font->setFamily(global_font_mp["Caviar_Dreams_Bold"]);
	_editor_font->setPointSize(10);
	this->setFont(*_editor_font);
	connect(this, &QTabWidget::tabCloseRequested, this, &XA_UIMODULE_CodeEditor::on_closeTab);


	QToolButton* tb = new QToolButton(this);
	tb->setIcon(QIcon(ICOPATH(newscript.svg)));
	tb->setStyleSheet(R"(background-color:#6F7285;
	border-radius:9px;
    border-style: none;)");
	connect(tb, &QToolButton::clicked, this, &XA_UIMODULE_CodeEditor::on_addNewScript);
	tb->setFixedWidth(20);
	this->tabBar()->setTabButton(0, QTabBar::LeftSide, tb);
	tb->move(tb->pos() + QPoint(0, 9));
	connect(this->tabBar(), &QTabBar::tabBarClicked, this, &XA_UIMODULE_CodeEditor::on_clickTab);

	_tabLabelEditor = new TabLabelEditor(this);
	connect(_tabLabelEditor, &TabLabelEditor::labelChanged, this, &XA_UIMODULE_CodeEditor::on_pageLabelChanged);

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
		crt_idx = this->count() - 1;
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
			crt_idx = this->count() - 1;
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
	crt_idx = this->count() - 1;
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

void XA_UIMODULE_CodeEditor::on_addNewScript()
{
	qDebug() << "XA_UIMODULE_CodeEditor::on_addNewScript()";
}

void XA_UIMODULE_CodeEditor::on_clickTab(int tabIdx)
{
	_tabLabelEditor->setOriLabel(tabIdx,this->tabText(tabIdx));
}

void XA_UIMODULE_CodeEditor::on_pageLabelChanged(int tabIdx, const QString& newLabel)
{
	this->setTabText(tabIdx, newLabel);
}

TabLabelEditor::TabLabelEditor(XA_UIMODULE_CodeEditor* codeEditor):_code_editor(codeEditor)
{
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
	this->setAttribute(Qt::WA_TranslucentBackground);
	QWidget* ovWgt = new QWidget(this);
	QVBoxLayout* owlay = new QVBoxLayout(this);
	owlay->setContentsMargins(0, 0, 0, 0);
	QHBoxLayout* upper_hlay = new QHBoxLayout();
	upper_hlay->setContentsMargins(10, 2, 10, 2);
	upper_hlay->setSpacing(4);
	QLabel* tip1 = new QLabel(_STRING_WRAPPER("ÖØÃüÃû:"));
	tip1->setFixedWidth(50);
	tip1->setAlignment(Qt::AlignRight);
	tip1->setFont(QFont("Microsoft YaHei",10));
	upper_hlay->addWidget(tip1);
	_fromTab = new QLineEdit();
	_fromTab->setReadOnly(true);
	_fromTab->setStyleSheet(TABLABLE_EDITOR_STYLE);
	upper_hlay->addWidget(_fromTab);
	QHBoxLayout* lower_hlay = new QHBoxLayout();
	lower_hlay->setContentsMargins(10, 2, 10, 2);
	upper_hlay->setSpacing(4);
	QLabel* tip2 = new QLabel(_STRING_WRAPPER("Îª:"));
	tip2->setFixedWidth(50);
	tip2->setAlignment(Qt::AlignRight);
	tip2->setFont(QFont("Microsoft YaHei", 10));
	lower_hlay->addWidget(tip2);
	_toTab = new QLineEdit();
	_toTab->setStyleSheet(TABLABLE_EDITOR_STYLE);
	lower_hlay->addWidget(_toTab);
	owlay->addLayout(upper_hlay);
	owlay->addLayout(lower_hlay);
	ovWgt->setLayout(owlay);
	ovWgt->setStyleSheet(".QWidget{background-color:wheat;border-radius:10px;}");
	ovWgt->setFixedSize(440, 80);
	connect(_toTab, &QLineEdit::returnPressed, [=]() {
			emit labelChanged(tabIdx, _toTab->text()); 
			this->label = "";
			_toTab->setText(""); this->hide();
		});
	this->hide();
}

void TabLabelEditor::setOriLabel(int tabIdx,const QString& orilabel)
{
	_fromTab->setText(orilabel);
	this->tabIdx = tabIdx;
	this->label = orilabel;
	this->move(_code_editor->mapToGlobal(QPoint(0,0)) + 
		QPoint(_code_editor->width() / 4, _code_editor->height()/2));
	this->show();
}
