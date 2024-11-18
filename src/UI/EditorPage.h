#ifndef EDITORPAGE_H
#define EDITORPAGE_H
#include <Qsci/qsciapis.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciscintilla.h>
#include <GL/GL_DefaultDfs.h>
#include <memory>

namespace SDL_UI_MODULE {

    class WindowAssetBar;

    class EditorPage : public QsciScintilla{
    public:
        EditorPage();
        ~EditorPage();
        static void initFont(const QString& font_name,int size);
        static void initIntellisense();
        std::shared_ptr<SDL_GL_MODULE::SDL_SHADER_INFO>& GetShader() { return m_shader; };
        WindowAssetBar* getAttachedAssetBar() { return m_attachedAssetBar; }

    protected:
        virtual void keyPressEvent(QKeyEvent* e);
        virtual void dragEnterEvent(QDragEnterEvent* event);
        virtual void dropEvent(QDropEvent* event);

    public:
        QString title;

    private:
        void setCustomStyle();

    private:
        static QsciLexerCPP* s_cpplexer;
        static QsciAPIs* s_api;
        std::shared_ptr<SDL_GL_MODULE::SDL_SHADER_INFO> m_shader;
        WindowAssetBar* m_attachedAssetBar;

    };

}

#endif
