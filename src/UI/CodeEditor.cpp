#include "CodeEditor.h"
#include "MsgBox.h"
#include "UI_DefaultDfs.h"
#include "EditorPage.h"
#include "OverallWindow.h"
#include "AssetWindow.h"
#include "GL/GLRenderer.h"
#include <QFont>
#include <QFontDatabase>
#include <QFileDialog>
#include <QMimeData>
#include <QMimeDatabase>
#include <QAction>
#include <QDragEnterEvent>
#include <QTimer>
#include <QDropEvent>
#include <QMap>
#include <QToolBar>
#include <QLabel>
#include <QDir>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QStringListModel>
using namespace std;
using namespace SDL_GL_MODULE;

namespace SDL_UI_MODULE {

	CodeEditor* CodeEditor::s_codeEditor;
    QStringList CodeEditor::s_bufferLabels = {"Image","Buffer A","Buffer B","Buffer C","Buffer D","CubeMap A"};
    int dptTabIdxSrc = 0;
    int dptTabIdxTarget = 0;
    int rmvTabIdx = 0;
    QString dptScriptName;

    SDL_UI_MODULE::CodeEditor::CodeEditor() {
        resize(m_editor_width,m_editor_height);
        setAttribute(Qt::WA_TranslucentBackground);
        setAcceptDrops(true);

        int fontId = QFontDatabase::addApplicationFont(FONTPATH(Cascadia.ttf));
        QStringList font_list = QFontDatabase::applicationFontFamilies(fontId);
        global_font_mp["Cascadia"] = font_list[0];

        m_editorFont.setPointSize(10);
        m_editorFont.setFamily(global_font_mp["Cascadia"]);

        fontId = QFontDatabase::addApplicationFont(FONTPATH(Caviar_Dreams_Bold.ttf));
        font_list = QFontDatabase::applicationFontFamilies(fontId);
        global_font_mp["Caviar_Dreams_Bold"] = font_list[0];

        EditorPage::initFont(global_font_mp["Cascadia"],10);
        EditorPage::initIntellisense();

        this->setContentsMargins(0,0,0,0);
        setTabsClosable(true);
        setMovable(false);
        setDocumentMode(true);
        m_editorFont.setFamily(global_font_mp["Caviar_Dreams_Bold"]);
        m_editorFont.setPointSize(10);
        setFont(m_editorFont);
        connect(this,&QTabWidget::tabCloseRequested,this,&CodeEditor::on_closeTab);

        QToolButton* tb = new QToolButton(this);
        tb->setIcon(QIcon(ICOPATH(newscript.svg)));
        tb->setStyleSheet(EDITOR_TOOLBUTTON_STYLE);
        connect(tb,&QToolButton::clicked,this,&CodeEditor::on_addNewScript);
        tb->setFixedWidth(20);
        this->tabBar()->setTabButton(0,QTabBar::LeftSide,tb);
        this->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
        tb->move(tb->pos() + QPoint(0, 9));

        m_typeScriptView = new QListView(this);
        m_typeScriptView->setStyleSheet(TYPES_LISTVIEW_STYLE);
        m_scriptTypelist.append("Common");
        m_scriptTypelist.append("Buffer A");
        m_scriptTypelist.append("Buffer B");
        m_scriptTypelist.append("Buffer C");
        m_scriptTypelist.append("Buffer D");
        m_scriptTypelist.append("Cubemap");
        m_scriptTypeMp.insert("Image",SDL_GL_SCRIPT_TYPE::SCRIPT_IMAGE);
		m_scriptTypeMp.insert("Common", SDL_GL_SCRIPT_TYPE::SCRIPT_COMMON);
		m_scriptTypeMp.insert("Buffer A", SDL_GL_SCRIPT_TYPE::SCRIPT_BUFFERA);
		m_scriptTypeMp.insert("Buffer B", SDL_GL_SCRIPT_TYPE::SCRIPT_BUFFERB);
		m_scriptTypeMp.insert("Buffer C", SDL_GL_SCRIPT_TYPE::SCRIPT_BUFFERC);
		m_scriptTypeMp.insert("Buffer D", SDL_GL_SCRIPT_TYPE::SCRIPT_BUFFERD);
		m_scriptTypeMp.insert("Cubemap", SDL_GL_SCRIPT_TYPE::SCRIPT_CUBEMAP);
        m_typeScriptView->setModel(new QStringListModel(m_scriptTypelist));
        m_typeScriptView->setSpacing(2);
        m_typeScriptView->setSpacing(2);
        m_typeScriptView->move(tb->pos() + QPoint(10,9));
        m_typeScriptView->setFixedWidth(100);
        m_typeScriptView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_typeScriptView->hide();
        m_typeScriptView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_typeScriptView->installEventFilter(this);
        connect(m_typeScriptView,&QListView::clicked,this,&CodeEditor::on_clickTypeScript);

        m_tabLabelEditor = new TabLabelEditor(this);

        connect(this->tabBar(),&QTabBar::tabBarDoubleClicked,this,&CodeEditor::on_clickTab);
        connect(this,&CodeEditor::currentChanged,this,&CodeEditor::on_tabChanged);
        connect(m_tabLabelEditor,&TabLabelEditor::labelChanged,this,&CodeEditor::on_pageLabelChanged);

        this->set_new_tab(nullptr,SDL_GL_SCRIPT_TYPE::SCRIPT_IMAGE,false);
    }

    SDL_UI_MODULE::CodeEditor::~CodeEditor() {

    }

    SDL_UI_MODULE::CodeEditor *SDL_UI_MODULE::CodeEditor::getEditor() {
        if(s_codeEditor == nullptr)
        {
            s_codeEditor = new CodeEditor();
        }
        return s_codeEditor;
    }

    void SDL_UI_MODULE::CodeEditor::setEditorSize(int width, int height) {
        m_editor_width = width;
        m_editor_height = height;
    }

    void SDL_UI_MODULE::CodeEditor::saveFile() {
        if((!m_savedState[this->currentIndex()].first) && this->count() > 0)
        {
            this->saveas();
            return;
        }

        QsciScintilla* editor = (QsciScintilla*)this->currentWidget();
        m_currentFile.setFileName(QString::fromStdString(m_savedState[this->currentIndex()].second));
        if(m_currentFile.open(QIODevice::WriteOnly))
        {
            m_currentFile.write(editor->text().toUtf8());
        }
        m_currentFile.close();
    }

    void SDL_UI_MODULE::CodeEditor::setOverallWindow(SDL_UI_MODULE::OverallWindow *ovWindow) {
        this->m_ovWindow = ovWindow;
        connect(this,&CodeEditor::setOvWindowAssetsBar,this->m_ovWindow,&OverallWindow::on_changeAssetBar);
    }

    void CodeEditor::setRenderer(SDL_GL_MODULE::GLRenderer* renderer)
    {
        m_renderer = renderer;
    }

	const QString CodeEditor::getCurrentPageLabel()
	{
        return m_currentPage->title; 
	}

	bool SDL_UI_MODULE::CodeEditor::comboShader() const {
        return CodeEditor::getEditor()->buffersInUse().size() > 0;
    }

    const QString SDL_UI_MODULE::CodeEditor::findPageContentByName(const QString &name) {
        int tabCount = this->tabBar()->count();
        for(int i = 0;i<tabCount;i++)
        {
            if(this->tabBar()->tabText(i) == name)
            {
                QsciScintilla* sci = (QsciScintilla*)this->widget(i);
                return sci->text();
            }
        }
        return "";
    }

    SDL_UI_MODULE::WindowAssetBar *SDL_UI_MODULE::CodeEditor::findAssetsBarByLabel(const QString &s) {
        
        return findEditorPageByLabel(s)->getAttachedAssetBar();
    }

	EditorPage* SDL_UI_MODULE::CodeEditor::findEditorPageByLabel(const QString& s)
	{
        int tabCount = this->tabBar()->count();
        for (int i = 0; i < tabCount; i++)
        {
            if (this->tabBar()->tabText(i) == s)
            {
                return (EditorPage*)this->widget(i);
            }
        }
        return nullptr;

	}

    EditorPage* SDL_UI_MODULE::CodeEditor::findEditorPageByShader(std::shared_ptr<SDL_GL_MODULE::SDL_SHADER_INFO> shader)
    {
        for (auto& page : m_pages)
        {
            if (shader == page->GetShader())
                return page;
        }
        return nullptr;
    }

    QStringList SDL_UI_MODULE::CodeEditor::buffersInUse() {
        QStringList res;
        static QStringList validBuffers = {"Buffer A","Buffer B","Buffer C","Buffer D","Cubemap"};
        for(const QString& buffer : validBuffers)
        {
            if(!m_scriptTypelist.contains(buffer)) res.append(buffer);
        }

        return res;
    }

    SDL_UI_MODULE::EditorPage *SDL_UI_MODULE::CodeEditor::get_new_page() {
        m_currentPage = new EditorPage();
        
        return m_currentPage;
    }

    void
    SDL_UI_MODULE::CodeEditor::set_new_tab(const QString &path, SDL_GL_MODULE::SDL_GL_SCRIPT_TYPE type,
                                           bool is_new_file) {
        EditorPage* page = this->get_new_page();

        switch (type) {
            case SDL_GL_MODULE::SCRIPT_NOTYPE:
                break;
            case SDL_GL_MODULE::SCRIPT_COMMON: {
                this->appendTab(page, "Common");
                page->GetShader()->type = SCRIPT_COMMON;
                return;
            }
            case SDL_GL_SCRIPT_TYPE::SCRIPT_IMAGE:
            {
                this->appendTab(page,"Image");
                page->GetShader()->type = SCRIPT_IMAGE;
                return;
            }
            case SDL_GL_SCRIPT_TYPE::SCRIPT_BUFFERA:
            {
                this->appendTab(page,"Buffer A");
                page->GetShader()->type = SCRIPT_BUFFERA;
                return;
            }
            case SDL_GL_SCRIPT_TYPE::SCRIPT_BUFFERB:
            {
                this->appendTab(page,"Buffer B");
                page->GetShader()->type = SCRIPT_BUFFERB;
                return;
            }
            case SDL_GL_SCRIPT_TYPE::SCRIPT_BUFFERC:
            {
                this->appendTab(page,"Buffer C");
                page->GetShader()->type = SCRIPT_BUFFERC;
                return;
            }
            case SDL_GL_SCRIPT_TYPE::SCRIPT_BUFFERD:
            {
                this->appendTab(page,"Buffer D");
                page->GetShader()->type = SCRIPT_BUFFERD;
                return;
            }
            case SDL_GL_SCRIPT_TYPE::SCRIPT_CUBEMAP:
            {
                this->appendTab(page,"Cubemap");
                page->GetShader()->type = SCRIPT_CUBEMAP;
                return;
            }
            default:
                break;
        }

        QFileInfo fileInfo(path);
        if(!fileInfo.isFile())
            return;

        QString file_name = fileInfo.baseName();
        QMimeDatabase db;
        QMimeType mime = db.mimeTypeForFile(file_name);

        int tab_count = this->count();
        for(int i = 0;i< tab_count;i++)
        {
            if(this->tabText(i) == file_name)
            {
                this->setCurrentIndex(i);
                this->m_currentFile.setFileName(path);
                return;
            }
        }
        this->appendTab(page,file_name);

        if(!is_new_file)
        {
            QFile file(path);
            if(!file.open(QIODevice::ReadOnly))
            {
                //DO some Log here
            }
            page->setText(file.readAll());
            file.close();
        }

        m_currentFile.setFileName(path);
        setCurrentIndex(this->count()-1);
    }

    void SDL_UI_MODULE::CodeEditor::appendTab(SDL_UI_MODULE::EditorPage *page, const QString &tabLabel) {
        m_currentPage = page;
        m_pages.push_back(page);
        m_currentPage->title = tabLabel;
        m_currentFile.setFileName(tabLabel);
        m_savedState.push_back(std::make_pair<bool,string>(false,tabLabel.toStdString()));

        this->addTab(page,tabLabel);
        this->setCurrentIndex(this->count() -1 );
    }

    void SDL_UI_MODULE::CodeEditor::duplicateRenameScript() {

    }

    void SDL_UI_MODULE::CodeEditor::closeBuildinTab() {
        m_savedState.erase(m_savedState.begin() + rmvTabIdx);
        m_scriptTypelist.insert(0,this->tabBar()->tabText(rmvTabIdx));
        m_typeScriptView->model()->insertRow(0);
        m_typeScriptView->model()->setData(m_typeScriptView->model()->index(0,0,QModelIndex()),this->tabBar()->tabText(rmvTabIdx));

        EditorPage* page = (EditorPage*)this->widget(rmvTabIdx);
        delete (EditorPage*)this->currentWidget();
        m_renderer->__update();

        this->removeTab(rmvTabIdx);
    }

    void SDL_UI_MODULE::CodeEditor::saveas() {
        QsciScintilla* editor = (QsciScintilla*)this->currentWidget();
        if(editor == nullptr)
            return;

        QString file_path = QFileDialog::getSaveFileName(this,"Save as",QDir::currentPath());
        if(!file_path.length())
            return;

        m_currentFile.setFileName(file_path);
        if(m_currentFile.open(QIODevice::WriteOnly))
        {
            m_currentFile.write(editor->text().toUtf8());
        }
        m_currentFile.close();
        this->setTabText(this->currentIndex(),m_currentFile.fileName());
        m_savedState[this->currentIndex()].first = true;
        m_savedState[this->currentIndex()].second = file_path.toStdString();
    }

    void SDL_UI_MODULE::CodeEditor::dragEnterEvent(QDragEnterEvent *event) {
        static QStringList acceptedFileTypes = {"vert","tesc","tese","geom","frag","comp","mesh","task"};
        if(event->mimeData()->hasUrls() && event->mimeData()->urls().count() == 1)
        {
            QFileInfo file(event->mimeData()->urls().at(0).toLocalFile());
            if(acceptedFileTypes.contains(file.suffix().toLower()))
            {
                event->acceptProposedAction();
            }
        }
    }

    void SDL_UI_MODULE::CodeEditor::dropEvent(QDropEvent *event) {
        QFileInfo dropped_file(event->mimeData()->urls().at(0).toLocalFile());
        QString pre_file = m_currentFile.fileName();
        QString new_file = dropped_file.absoluteFilePath();
        bool is_existed = false;

        int tab_count = this->count();
        for(int i = 0; i < tab_count;i++)
        {
            if(this->tabText(i) == new_file)
            {
                this->setCurrentIndex(i);
                is_existed = true;
                break;
            }
        }

        m_currentFile.setFileName(new_file);
        if(m_currentFile.open(QIODevice::ReadOnly))
        {
            if(is_existed)
            {
                QsciScintilla* editor = (QsciScintilla*)this->currentWidget();
                editor->setText(m_currentFile.readAll());
            }else
            {
                this->set_new_tab(new_file,SDL_GL_SCRIPT_TYPE::SCRIPT_NOTYPE,false);
                m_savedState.push_back(make_pair<bool,string>(true,new_file.toStdString()));
            }
        }
    }

    bool SDL_UI_MODULE::CodeEditor::eventFilter(QObject *obj, QEvent *e) {
        if(obj == m_typeScriptView)
        {
            QListView* view = (QListView*)obj;
            if(e->type() == QEvent::Leave)
            {
                view->hide();
            }
        }
        return QObject::eventFilter(obj, e);
    }

    void SDL_UI_MODULE::CodeEditor::on_copy() {
        QsciScintilla* editor = (QsciScintilla*)this->currentWidget();

        if(editor)
            editor->copy();
    }

    void SDL_UI_MODULE::CodeEditor::on_closeTab(int index) {

        if(m_scriptTypeMp.keys().contains(this->tabText(index)))
        {
            if (this->tabText(index) != s_bufferLabels[0])
            {
                rmvTabIdx = index;
                QStringList BtnText = {"","Confirm","Cancel"};
                std::vector<std::function<void(void)>> callbacks;

                callbacks.push_back(nullptr);
                callbacks.push_back([this]
                {
                    this->closeBuildinTab();
                });
                callbacks.push_back(nullptr);
 
                MsgBox::question(this,QString("Do you want to delete the pass?"),QString("%1.glsl").arg(this->tabText(index)),this->size(),BtnText,callbacks);
            }
            else
            {
				QStringList BtnText = { "","Confirm","Cancel" };
				std::vector<std::function<void(void)>> callbacks;

                callbacks.push_back(nullptr);
                callbacks.push_back(nullptr);
                callbacks.push_back(nullptr);

               MsgBox::warning(this, QString("mainImage cann't be deleted"), QString("%1.glsl").arg(this->tabText(index)), this->size(), BtnText, callbacks);
            }
        }
    }

    void SDL_UI_MODULE::CodeEditor::on_tabChanged(int index) {
        m_currentPage = (EditorPage *)this->currentWidget();

        emit setOvWindowAssetsBar(m_currentPage->getAttachedAssetBar());
    }

    void SDL_UI_MODULE::CodeEditor::on_addNewScript() {
        if(m_scriptTypelist.size())
            m_typeScriptView->show();
        else
            set_new_tab("untitled");
    }

    void SDL_UI_MODULE::CodeEditor::on_clickTab(int tabIdx) {
        if(!s_bufferLabels.contains(this->tabBar()->tabText(tabIdx)))
            m_tabLabelEditor->setOriLabel(tabIdx,this->tabText(tabIdx));
    }

    void SDL_UI_MODULE::CodeEditor::on_pageLabelChanged(int tabIdx, const QString &newLabel) {
        int tabcount = this->tabBar()->count();
        for(int i = 0;i < tabcount;i++){
            if(i == tabIdx)
                continue;
            if(this->tabBar()->tabText(i) == newLabel)
            {
                QStringList BtnTexts = {"","Confirm","Cancel"};
                std::vector<std::function<void(void)>> callbacks;

                dptTabIdxSrc = i;
                dptTabIdxTarget = tabIdx;
                dptScriptName = newLabel;

                callbacks.push_back(nullptr);
                callbacks.push_back([&]
                    {
                        this->duplicateRenameScript();
                    });
                callbacks.push_back(nullptr);
                MsgBox* quest = MsgBox::question(this,QString("Another shader named %1"
                "Do you really want to rename this shader?").arg(newLabel),QString("%1.glsl").arg(newLabel),this->size(),BtnTexts,callbacks);
                return;
            }
        }
        this->setTabText(tabIdx,newLabel);
        m_savedState[tabIdx].first = false;
        m_savedState[tabIdx].second = newLabel.toStdString();
    }

    void SDL_UI_MODULE::CodeEditor::on_clickTypeScript(const QModelIndex &modelIdx) {
        int row = modelIdx.row();
        m_typeScriptView->model()->removeRow(row);
        set_new_tab(m_scriptTypelist[row],m_scriptTypeMp[m_scriptTypelist[row]]);
        m_scriptTypelist.removeAt(row);
    }

    SDL_UI_MODULE::TabLabelEditor::TabLabelEditor(SDL_UI_MODULE::CodeEditor *codeEditor) : m_codeEditor(codeEditor) {
        setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
        setAttribute(Qt::WA_TranslucentBackground);
        QWidget* ovWidget = new QWidget(this);
        QVBoxLayout* owlay = new QVBoxLayout(this);
        owlay->setContentsMargins(0,0,0,0);
        QHBoxLayout* upper_hlay = new QHBoxLayout();
        upper_hlay->setContentsMargins(10,2,10,2);
        upper_hlay->setSpacing(4);
        QLabel* tip1 = new QLabel(tr("Refactor:"));
        tip1->setFixedWidth(UI_COMPONENT_WIDTH(tablabel_tip_width_factor));
        tip1->setAlignment(Qt::AlignRight);
        tip1->setFont(QFont("Microsoft YaHei",10));
        upper_hlay->addWidget(tip1);
        m_fromTab = new QLineEdit();
        m_fromTab->setReadOnly(true);
        m_fromTab->setStyleSheet(TABLABLE_EDITOR_STYLE);
        upper_hlay->addWidget(m_fromTab);
        QHBoxLayout* lower_hlay = new QHBoxLayout();
        lower_hlay->setContentsMargins(10,2,10,2);
        upper_hlay->setSpacing(4);
        QLabel* tip2 = new QLabel(tr("to:"));
        tip2->setFixedWidth(UI_COMPONENT_WIDTH(tablabel_tip_width_factor));
        tip2->setAlignment(Qt::AlignRight);
        tip2->setFont(QFont("Microsoft YaHei",10));
        m_toTab = new QLineEdit();
        m_toTab->setStyleSheet(TABLABLE_EDITOR_STYLE);
        lower_hlay->addWidget(m_toTab);
        owlay->addLayout(upper_hlay);
        owlay->addLayout(lower_hlay);
        ovWidget->setLayout(owlay);
        ovWidget->setStyleSheet(".QWidget{background-color:wheat;border-radius:10px;}");
        ovWidget->setFixedSize(UI_COMPONENT_WIDTH(editorTab_refactor_width_factor), UI_COMPONENT_WIDTH(editorTab_refactor_height_factor));
        connect(m_toTab,&QLineEdit::returnPressed,[this]()
        {
            emit labelChanged(m_tabIdx,m_toTab->text());
            this->m_label = "";
            m_toTab->setText("");this->hide();
        });
        this->hide();
    }

    void SDL_UI_MODULE::TabLabelEditor::setOriLabel(int tabIdx, const QString &oriLabel) {
        m_fromTab->setText(oriLabel);
        m_tabIdx = tabIdx;
        m_label = oriLabel;
        this->move(m_codeEditor->mapToGlobal(QPoint(0,0)+QPoint(m_codeEditor->width()/4,m_codeEditor->height()/2)));
        if(m_label.size())
            show();
    }
}