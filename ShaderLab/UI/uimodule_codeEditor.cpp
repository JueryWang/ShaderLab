#include "ui_defaultDfs.h"
#include "../Utilitys/uitilityDfs.h"
#include "uimodule_codeEditor.h"
#include "uimodule_editorpage.h"
#include "uimodule_MsgBox.h"
#include <QFontDatabase>
#include <QFileDialog>
#include <QMimeData>
#include <QAction>
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
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QStringListModel>
#include <QDebug>
XA_UIMODULE_CodeEditor* XA_UIMODULE_CodeEditor::_codeEditor;
static const QStringList unChangableLabels = { "Common","Image","Buffer A","Buffer B","Buffer C","Cubemap" };
using namespace std;

int dptTabIdxSrc = 0;
int dptTabIdxTarget = 0;
int rmvTabIdx = 0;
QString dptScriptName;

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
	this->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
	tb->move(tb->pos() + QPoint(0, 9));

	_typeScriptView = new QListView(this);
	_typeScriptView->setStyleSheet(TYPES_LISTVIEW_STYLE);
	scriptTypeslist.append("Common");
	scriptTypeslist.append("Buffer A");
	scriptTypeslist.append("Buffer B");
	scriptTypeslist.append("Buffer C");
	scriptTypeslist.append("Buffer D");
	scriptTypeslist.append("Cubemap");
	scriptTypesMp.insert("Image", XA_GL_SCRIPT_IMAGE);
	scriptTypesMp.insert("Common", XA_GL_SCRIPT_COMMON);
	scriptTypesMp.insert("Buffer A", XA_GL_SCRIPT_BUFFERA);
	scriptTypesMp.insert("Buffer B", XA_GL_SCRIPT_BUFFERB);
	scriptTypesMp.insert("Buffer C", XA_GL_SCRIPT_BUFFERC);
	scriptTypesMp.insert("Buffer D", XA_GL_SCRIPT_BUFFERD);
	scriptTypesMp.insert("Buffer D", XA_GL_SCRIPT_CUBEMAP);
	_typeScriptView->setModel(new QStringListModel(scriptTypeslist));
	_typeScriptView->setSpacing(2);
	_typeScriptView->move(tb->pos()+ QPoint(10, 9));
	_typeScriptView->setFixedWidth(100);
	_typeScriptView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_typeScriptView->hide();
	_typeScriptView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_typeScriptView->installEventFilter(this);
	connect(_typeScriptView, &QListView::clicked, this, &XA_UIMODULE_CodeEditor::on_clickTypeScript);

	_tabLabelEditor = new TabLabelEditor(this);

	connect(this->tabBar(), &QTabBar::tabBarDoubleClicked, this, &XA_UIMODULE_CodeEditor::on_clickTab);
	connect(_tabLabelEditor, &TabLabelEditor::labelChanged, this, &XA_UIMODULE_CodeEditor::on_pageLabelChanged);

	this->set_new_tab(nullptr, XA_GL_SCRIPT_IMAGE,false);
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

void XA_UIMODULE_CodeEditor::set_new_tab(const QString& path, XA_GL_SCRIPT_TYPE type,bool is_new_file)
{
	QsciScintilla* editor = this->get_new_page();

	switch (type)
	{
	case XA_GL_SCRIPT_NOTYPE:
		break;
	case XA_GL_SCRIPT_COMMON:
	{
		this->appendTab(editor, "Common");
		return;
	}
	case XA_GL_SCRIPT_IMAGE:
	{
		this->appendTab(editor, "Image");
		return;
	}
	case XA_GL_SCRIPT_BUFFERA:
	{
		this->appendTab(editor, "Buffer A");
		return;
	}
	case XA_GL_SCRIPT_BUFFERB:
	{
		this->appendTab(editor, "Buffer B");
		return;
	}
	case XA_GL_SCRIPT_BUFFERC:
	{
		this->appendTab(editor, "Buffer C");
		return;
	}
	case XA_GL_SCRIPT_BUFFERD:
	{
		this->appendTab(editor, "Buffer D");
		return;
	}
	case XA_GL_SCRIPT_CUBEMAP:
	{
		this->appendTab(editor, "Cubemap");
		return;
	}
	default:
		break;
	}


	QFileInfo fileInfo(path);
	if (!fileInfo.isFile())
		return;

	QString file_name = fileInfo.baseName();
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
	this->appendTab(editor, file_name);
	
	if (!is_new_file)
	{
		QFile* file = new QFile(path);
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

void XA_UIMODULE_CodeEditor::appendTab(QsciScintilla* editor,const QString& tabLabel)
{
	this->addTab(editor, tabLabel);
	this->setCurrentIndex(this->count() - 1);
	_current_file = std::make_unique<QFile>(tabLabel);
	saved_state.push_back(std::make_pair<bool, string>(false, tabLabel.toStdString()));
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
			this->set_new_tab(new_file,XA_GL_SCRIPT_NOTYPE,false);
			saved_state.push_back(make_pair<bool, string>(true,new_file.toStdString()));
		}
	}
	else
	{
		_current_file->setFileName(pre_file);
	}
}

bool XA_UIMODULE_CodeEditor::eventFilter(QObject* obj, QEvent* e)
{
	if (obj == _typeScriptView)
	{
		QListView* view = (QListView*)obj;
		if (e->type() == QEvent::Leave)
		{
			view->hide();
		}
	}
	return QTabWidget::eventFilter(obj, e);
}

void XA_UIMODULE_CodeEditor::on_closeTab(int index)
{
	if (scriptTypesMp.keys().contains(this->tabText(index)))
	{
		rmvTabIdx = index;
		QStringList BtnTexts = { "","Comfirm","Cancel" };
		std::vector<std::function<void(void)>> callbacks;

		callbacks.push_back(nullptr);
		callbacks.push_back([&]
			{
				this->closeBuildinTab();
			});
		callbacks.push_back(nullptr);

		XA_UIModule_QUEST_BOX* quest = XA_UIModule_QUEST_BOX::question(this, QString("Do you want to delete the pass? "), QString("%1.glsl").arg(this->tabText(index)), this->size(),
			BtnTexts, callbacks);
		return;
	}
	this->removeTab(index);
}

void XA_UIMODULE_CodeEditor::on_copy()
{
	QsciScintilla* editor = (QsciScintilla*)this->currentWidget();

	if (editor)
		editor->copy();
}

void XA_UIMODULE_CodeEditor::savefile()
{
	if ((!saved_state[this->currentIndex()].first) && this->count() > 0)
	{
		this->saveas();
		return;
	}

	QsciScintilla* editor = (QsciScintilla*)this->currentWidget();
	_current_file->setFileName(QString::fromStdString(saved_state[this->currentIndex()].second));
	if (_current_file->open(QIODevice::WriteOnly))
	{
		_current_file->write(editor->text().toUtf8());
	}
	_current_file->close();
}

void XA_UIMODULE_CodeEditor::duplicateRenameScript()
{
	saved_state.erase(saved_state.begin() + dptTabIdxTarget);
	this->setTabText(dptTabIdxTarget, dptScriptName);
	this->removeTab(dptTabIdxSrc);
}


void XA_UIMODULE_CodeEditor::closeBuildinTab()
{
	saved_state.erase(saved_state.begin() + rmvTabIdx);
	scriptTypeslist.insert(0, this->tabBar()->tabText(rmvTabIdx));
	_typeScriptView->model()->insertRow(0);
	_typeScriptView->model()->setData(_typeScriptView->model()->index(0, 0, QModelIndex()), this->tabBar()->tabText(rmvTabIdx));
	this->removeTab(rmvTabIdx);
}

void XA_UIMODULE_CodeEditor::saveas()
{
	QsciScintilla* editor = (QsciScintilla*)this->currentWidget();
	if (editor == nullptr)
		return;

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
	saved_state[this->currentIndex()].first = true;
	saved_state[this->currentIndex()].second = file_path.toStdString();
}

void XA_UIMODULE_CodeEditor::on_addNewScript()
{
	if (scriptTypeslist.size())
		_typeScriptView->show();
	else
		this->set_new_tab("untitled");
}

void XA_UIMODULE_CodeEditor::on_clickTab(int tabIdx)
{
	if(!unChangableLabels.contains(this->tabBar()->tabText(tabIdx)))
		_tabLabelEditor->setOriLabel(tabIdx,this->tabText(tabIdx));
}

void XA_UIMODULE_CodeEditor::on_pageLabelChanged(int tabIdx, const QString& newLabel)
{
	int tabcount = this->tabBar()->count();
	for (int i = 0; i < tabcount; i++)
	{
		if(i ==  tabIdx)
			continue;
		if (this->tabBar()->tabText(i) == newLabel)
		{
			QStringList BtnTexts = { "","Comfirm","Cancel" };
			std::vector<std::function<void(void)>> callbacks;

			dptTabIdxSrc = i;
			dptTabIdxTarget = tabIdx;
			dptScriptName = newLabel;

			callbacks.push_back(nullptr);
			callbacks.push_back([&]
				{
					//No param should be passed
					this->duplicateRenameScript();
				});
			callbacks.push_back(nullptr);
			XA_UIModule_QUEST_BOX* quest = XA_UIModule_QUEST_BOX::question(this, QString("Another shader named %1"
				"Do you really want to rename this shader?").arg(newLabel), QString("%1.glsl").arg(newLabel), this->size(),
				BtnTexts,callbacks);
			return;
		}
	}
	this->setTabText(tabIdx, newLabel);
	saved_state[tabIdx].first = false;
	saved_state[tabIdx].second = newLabel.toStdString();
}


void XA_UIMODULE_CodeEditor::on_clickTypeScript(const QModelIndex& modelIdx)
{
	int row = modelIdx.row();
	_typeScriptView->model()->removeRow(row);
	set_new_tab(scriptTypeslist[row],scriptTypesMp[scriptTypeslist[row]]);
	scriptTypeslist.removeAt(row);
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
	if(this->label.size())
		this->show();
}
