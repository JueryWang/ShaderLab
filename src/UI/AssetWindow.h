#ifndef WINDOWASSET_H
#define WINDOWASSET_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QList>
#include <QMenu>

namespace SDL_GL_MODULE
{
    class GLRenderer;
}

namespace SDL_UI_MODULE {
    class CodeEditor;
    class EditorPage;
    class AssetWindowBar;

    class AssetWindow : public QWidget{
    public :
        AssetWindow(const QSize& size);
    protected:
        virtual  void paintEvent(QPaintEvent* event);
    public:
        enum AssetType
        {
            NONE = 0,
            IMAGE,
            AUDIO,
            Mdoel,
        } m_assetType;

        QLabel m_crossIcon;
        QString m_assetPath;
        QImage m_showImage;
        QRect m_crossRect;
    };

    class AssetWindowBox : public QWidget
    {
        Q_OBJECT
    public:
        AssetWindowBox(int index);
        ~AssetWindowBox();
        void setAudioPlayDone();
        void setAttachedPage(EditorPage* page) { m_attachedPage = page; }
        const char* getAssetPath() { return diskpath; }

    protected:
        virtual bool eventFilter(QObject* obj,QEvent* event);
    private:
        void sendAssets(AssetWindow::AssetType type);
        void setupAudioSets();
        void unsetupAudioSets();

    public slots:
        void on_clcAudioPause();
        void on_clcAudioResume();
        void on_clcAudioRewind();
        void on_clcSetAudioVolume();
        void on_clcAudioSetting();
        void on_clcAddBufferA();
        void on_clcAddBufferB();
        void on_clcAddBufferC();
        void on_clcAddBufferD();
        void on_clcAddCubeMapA();

    private:
        QVBoxLayout* m_vlay;
        QHBoxLayout* m_audioSetLay;
        
        AssetWindow* m_window;
        EditorPage* m_attachedPage = nullptr;

        QWidget* m_audioToolBar;
        QLabel* m_label;
        QPushButton* m_pauseBtn;
        QPushButton* m_volumeBtn;
        QPushButton* m_rewindBtn;
        QPushButton* m_settingBtn;
        QSlider* m_auVolumeSlider;
        QMenu* m_addBufferMenu;
        QMap<QString, QAction*> m_menuActionsMp;

        int m_index;
        bool openedAsset = false;
        bool playingAudio = false;

        QImage showImage;
        const char* diskpath;
    };

    class WindowAssetBar : public  QScrollArea
    {
        friend class AssetWindowBox;
        Q_OBJECT
    public:
        WindowAssetBar(EditorPage* page);
        ~WindowAssetBar();
        int totalAsset();
        static void setAssetsReciver(SDL_GL_MODULE::GLRenderer* receiver);

    private:
        QList<AssetWindowBox*> m_assetWindowList;
        static SDL_GL_MODULE::GLRenderer* s_glReceiver;
    };
}

#endif
