#ifndef WINDOWINFO_PANNEL_H
#define WINDOWINFO_PANNEL_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

namespace SDL_UI_MODULE
{
    class GLWidget;

    class WindowInfoPanel :public  QWidget{
        friend  class GLWidget;
        Q_OBJECT
    public :
        WindowInfoPanel(QWidget* parent,const QSize &initSize,const QString &source);
        ~WindowInfoPanel();
        void updateInfo(const QSize& newSize);
        void updateFPS(int fps);
    public slots:
        void on_clcRecord();
        void on_clcEndRecord();
        void on_clcSoundOn();
        void on_clcSoundOff();
    public:
        QLabel* m_SourceShowText;
        QLabel* m_SourceInfoText;
        QLabel* m_SizeShowText;
        QLabel* m_SizeInfoText;
        QLabel* m_fpsInfoText;
        QPushButton* m_lockBtn;
        QPushButton* m_resetBtn;
        QPushButton* m_recBtn;
        QPushButton* m_soundBtn;

        QSize m_SourceSize;
        QSize m_WindowSize;
        QString m_sourceLabel;

        GLWidget* glWindowInst = nullptr;
    };
}

#endif
