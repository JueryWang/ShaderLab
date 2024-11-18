#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <utility>
#include <queue>
#include <memory>
#include <QTableWidget>
#include <QToolButton>
#include <QMenu>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QFile>
#include <QTabBar>
#include <QFont>
#include <QStatusBar>
#include "GL/GL_DefaultDfs.h"

class QScintilla;

namespace SDL_GL_MODULE
{
    class GLRenderer;
}

namespace SDL_UI_MODULE
{
    class EditorPage;
    class TabLabelEditor;
    class WindowAssetBar;
    class OverallWindow;

    class CodeEditor : public QTabWidget{
        friend class EditorPage;
        Q_OBJECT

    public:
        static CodeEditor* getEditor();
        void setEditorSize(int width,int height);
        void saveFile();
        void setOverallWindow(OverallWindow* ovWindow);
        void setRenderer(SDL_GL_MODULE::GLRenderer* renderer);
        bool comboShader() const;
        const QString findPageContentByName(const QString& name);
        WindowAssetBar* findAssetsBarByLabel(const QString& s);
        EditorPage* findEditorPageByLabel(const QString& s);
        EditorPage* findEditorPageByShader(std::shared_ptr<SDL_GL_MODULE::SDL_SHADER_INFO> shader);
        const QString getCurrentPageLabel();
        EditorPage* currentPage() { return m_currentPage; }
        std::vector< EditorPage*> getPages() { return m_pages; }
        QStringList buffersInUse();

    private:
        EditorPage* get_new_page();
        void set_new_tab(const QString& path,SDL_GL_MODULE::SDL_GL_SCRIPT_TYPE type = SDL_GL_MODULE::SCRIPT_NOTYPE,bool is_new_file = true);
        void appendTab(EditorPage* page,const QString& tabLabel);
        void duplicateRenameScript();
        void closeBuildinTab();
        void saveas();

    protected:
        virtual void dragEnterEvent(QDragEnterEvent* event);
        virtual void dropEvent(QDropEvent* event);
        virtual bool eventFilter(QObject* obj, QEvent* e);

    signals:
        void setOvWindowAssetsBar(WindowAssetBar* newBar);

    public slots:
        void on_copy();
        void on_closeTab(int index);
        void on_tabChanged(int index);
        void on_addNewScript();
        void on_clickTab(int tabIdx);
        void on_pageLabelChanged(int tabIdx,const QString& newLabel);
        void on_clickTypeScript(const QModelIndex &modelIdx);

    private:
        CodeEditor();
        ~CodeEditor();

    public:
        static CodeEditor* s_codeEditor;
        static QStringList s_bufferLabels;
        static QString s_title;

        TabLabelEditor* m_tabLabelEditor;
        QStringList m_scriptTypelist;
        QMap<QString,SDL_GL_MODULE::SDL_GL_SCRIPT_TYPE> m_scriptTypeMp;
        QListView* m_typeScriptView;
        int m_editor_width;
        int m_editor_height;

        QFont m_editorFont;
        QFile m_currentFile;

        EditorPage* m_currentPage;
        std::vector<EditorPage*> m_pages;
        std::deque<std::pair<bool,std::string>> m_savedState;

        OverallWindow* m_ovWindow;
        SDL_GL_MODULE::GLRenderer* m_renderer;
    };

    class TabLabelEditor : public QWidget
    {
        Q_OBJECT
    public:
        TabLabelEditor(CodeEditor *codeEditor);
        void setOriLabel(int tabIdx,const QString& oriLabel);

    signals:
        void labelChanged(int tabIdx,const QString& newLabel);

    private:
        QString m_label;
        int m_tabIdx;
        QLineEdit* m_fromTab;
        QLineEdit* m_toTab;
        CodeEditor* m_codeEditor;
    };
}

#endif //SHADERLAB_CODEEDITOR_H
