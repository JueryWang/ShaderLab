#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QThread>
#include <QTimer>
#include <QImage>

namespace SDL_GL_MODULE
{
    class GLRenderer;
}

namespace SDL_UI_MODULE {
    class WindowInfoPanel;

    class GLWidget : public QWidget{
        friend class OverallWindow;
        Q_OBJECT
    public:
        GLWidget(QWidget* parent,const std::string& title,int width,int height);
        ~GLWidget();
        SDL_GL_MODULE::GLRenderer* getRenderer();
        void setWindowInfoPanel(WindowInfoPanel* pannel);
        void __setMinimumSize(const QSize& size);
        void __setMaximumSize(const QSize& size);
        void __reshow();
        void __startRecord();
        void __endRecord();

    public slots:
        void on_clcLockSize();
        void on_clcUnlockSize();
        void on_clcResetSize();

    signals:
        void beginGLDraw();
        void resetGLWidget(const QSize &size);

    protected:
        void paintEvent(QPaintEvent* event) override;
        bool eventFilter(QObject* obj,QEvent* event) override;

    public:
        static uint8_t* sdl_glwgt_pctBuffing;
        static int sdl_glwgt_buffingSize;

    private:
        QThread* m_renderThread;
        SDL_GL_MODULE::GLRenderer* m_glBackendRender = nullptr;
        WindowInfoPanel* m_infoPanel = nullptr;
        QTimer* m_fpsTicker = nullptr;

        QImage m_picture;
        QImage m_recIcon;

        int m_wgt_width;
        int m_wgt_height;
        QSize dragged_size;
        QSize default_size;
        QSize minimum_size;
        QSize maximum_size;
        std::string m_title;
        bool size_locked = false;
        bool recording = false;
    };

} // SDL_UI_MODULE

#endif //SHADERLAB_GLWIDGET_H
