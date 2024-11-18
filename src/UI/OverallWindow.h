#ifndef OVERALLWINDOW_H
#define OVERALLWINDOW_H

#include "MenuLayertop.h"
#include <QVBoxLayout>
#include <QSplitter>

namespace SDL_UI_MODULE {
    class GLWidget;
    class WindowInfoPanel;
    class VarShowBoard;
    class WindowAssetBar;
    class CodeEditor;

    class OverallWindow : public QWidget {

        Q_OBJECT
    public:
        OverallWindow();
        ~OverallWindow();
        const QSize getMontorsz() const;
        void init();
        void loadQssTheme();
        void rollbackNormal();
        void setFullScreen();
        void setMinimum();
        GLWidget* getRenderWindow() { return m_glWindow; };

    public slots:
        void on_resetGLWidget(const QSize &size);
        void on_compileCode();
        void on_updateVarBoard();
        void on_changeAssetBar(WindowAssetBar *newBar);

    private:
        QVBoxLayout *m_ovlayout;
        MenuLayerTop *m_menubar;

        GLWidget *m_glWindow;
        WindowInfoPanel *m_windowInfoPanel;
        WindowAssetBar* m_assetBar;
        VarShowBoard *m_varShowBoard;

        QSplitter* m_splitter_v1;
        QSplitter* m_splitter_v2;
        QSplitter* m_splitter_h;

        QString m_qssPath;

        QSize m_monitorResolution;
        QSize m_normalSize;
        QPoint m_anchorPos;
    };
}
#endif // OVERALLWINDOW_H
