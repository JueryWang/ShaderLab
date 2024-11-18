#include "AssetWindow.h"
#include "UI_DefaultDfs.h"
#include "Utility/Utils_DefaultDfs.h"
#include "GL/GL_DefaultDfs.h"
#include "GL/GLSessionManager.h"
#include "GL/GLRenderer.h"
#include "Utility/AssetsManager/Audio/AudioManager.h"
#include "UI/RadioConditioner.h"
#include "EditorPage.h"
#include "CodeEditor.h"
#include <QFont>
#include <QFileDialog>
#include <QMouseEvent>
#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>
#include <QFileInfo>
#include <QPainter>
#include <QProcess>
#include <QStringListModel>
#include <QDebug>

using namespace SDL_GL_MODULE;
using namespace SDL_UTILS;

namespace SDL_UI_MODULE {
    SDL_GL_MODULE::GLRenderer* WindowAssetBar::s_glReceiver;

    AssetWindow::AssetWindow(const QSize &size){
        setStyleSheet(R"(.QWidget{
                        background-color:black;
                        border:none;
                        border-top-left-radius:5px;
                        border-top-right-radius:5px;
                        })");
        setFixedSize(size);
        m_assetType = NONE;

        QPixmap crossPix = QPixmap(ICOPATH(cross.svg));
        m_crossIcon.setFixedSize(QSize(20, 20));
        m_crossIcon.setPixmap(crossPix);
        m_crossIcon.setAttribute(Qt::WA_TranslucentBackground);
        m_crossRect = QRect(this->width() - 25, 5, 20, 20);
    }

    void AssetWindow::paintEvent(QPaintEvent *event) {
        static QRect rect = QRect(0,0,this->width(),this->height());
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing,true);

        switch (m_assetType) {
            case AssetType::IMAGE:
                painter.drawImage(rect,m_showImage);
                painter.drawPixmap(m_crossRect,m_crossIcon.grab());
                painter.end();
                break;
            case AssetType::AUDIO:
                painter.drawImage(rect,m_showImage);
                painter.drawPixmap(m_crossRect,m_crossIcon.grab());
                painter.end();
                break;
            default:
                break;
        }
    }

    AssetWindowBox::AssetWindowBox(int index) : m_index(index){
        setAttribute(Qt::WA_Hover);
        installEventFilter(this);

        int fontId = QFontDatabase::addApplicationFont(FONTPATH(OpenSans-Regular.ttf));
        QStringList font_list = QFontDatabase::applicationFontFamilies(fontId);
        global_font_mp["OpenSans-Regular"] = font_list[0];

        m_vlay = new QVBoxLayout(this);
        m_vlay->setContentsMargins(0,0,0,0);

        m_window = new AssetWindow(QSize(UI_COMPONENT_WIDTH(window_asset_width_factor), UI_COMPONENT_HEIGHT(window_asset_height_factor)));

        m_audioToolBar = new QWidget(this);
        m_audioToolBar->setStyleSheet(R"(.QWidget{
                        background-color:white;
                        border-bottom-left-radius:5px;
                        border-bottom-right-radius:5px;
                        })");
        m_audioToolBar->setFixedHeight(20);
        m_audioSetLay = new QHBoxLayout();
        m_audioSetLay->setContentsMargins(0,0,0,0);
        m_audioSetLay->setSpacing(0);

        m_label = new QLabel();
        m_label->setAlignment(Qt::AlignLeft);
        m_label->setText(QString(" iChannel%1").arg(index));
        m_label->setStyleSheet(R"(.QLabel{
                        background-color:white;
                        border-bottom-left-radius:5px;
                        border-bottom-right-radius:5px;
                        })");
        QFont label_font;
        label_font.setFamily(global_font_mp["OpenSans-Regular"]);
        label_font.setPointSize(10);
        m_label->setFont(label_font);
        m_label->setFixedHeight(20);

        m_pauseBtn = new QPushButton(m_audioToolBar);
        m_pauseBtn->setAttribute(Qt::WA_TranslucentBackground,true);
        m_pauseBtn->setStyleSheet(WINDOW_ASEETBAR_BTN_STYLE);
        m_pauseBtn->setIconSize(QSize(13,13));
        m_pauseBtn->setFixedWidth(20);
        m_pauseBtn->setIcon(QIcon(ICOPATH(pause.svg)));
        connect(m_pauseBtn,&QPushButton::clicked,this,&AssetWindowBox::on_clcAudioPause);

        m_rewindBtn = new QPushButton(m_audioToolBar);
        m_rewindBtn->setAttribute(Qt::WA_TranslucentBackground,true);
        m_rewindBtn->setStyleSheet(WINDOW_ASEETBAR_BTN_STYLE);
        m_rewindBtn->setFixedWidth(20);
        m_rewindBtn->setIconSize(QSize(13,13));
        m_rewindBtn->setIcon(QIcon(ICOPATH(rewind.svg)));
        connect(m_rewindBtn,&QPushButton::clicked,this,&AssetWindowBox::on_clcAudioRewind);

        m_volumeBtn = new QPushButton(m_audioToolBar);
        m_volumeBtn->setAttribute(Qt::WA_TranslucentBackground,true);
        m_volumeBtn->setStyleSheet(WINDOW_ASEETBAR_BTN_STYLE);
        m_volumeBtn->setIconSize(QSize(18,18));
        m_volumeBtn->setFixedWidth(20);
        m_volumeBtn->setIcon(QIcon(ICOPATH(audio-volume.svg)));
        connect(m_volumeBtn,&QPushButton::clicked,this,&AssetWindowBox::on_clcAudioResume);

        m_settingBtn = new QPushButton(m_audioToolBar);
        m_settingBtn->setAttribute(Qt::WA_TranslucentBackground,true);
        m_settingBtn->setStyleSheet(WINDOW_ASEETBAR_BTN_STYLE);
        m_settingBtn->setIconSize(QSize(13,13));
        m_settingBtn->setFixedWidth(20);
        m_settingBtn->setIcon(QIcon(ICOPATH(setting.svg)));
        connect(m_settingBtn,&QPushButton::clicked,this,&AssetWindowBox::on_clcAudioSetting);

        m_audioSetLay->addWidget(m_pauseBtn,Qt::AlignRight);
        m_audioSetLay->addWidget(m_rewindBtn,Qt::AlignRight);
        m_audioSetLay->addWidget(m_volumeBtn,Qt::AlignRight);
        m_audioSetLay->addWidget(m_settingBtn,Qt::AlignRight);
        m_audioToolBar->setLayout(m_audioSetLay);

        m_vlay->addWidget(m_window);
        m_vlay->addWidget(m_label);
        m_vlay->setSpacing(0);
        this->setLayout(m_vlay);

        typedef void (AssetWindowBox::* slot_func)(void);

		m_addBufferMenu = new QMenu(this);
		m_addBufferMenu->setStyleSheet(TYPES_MENU_STYLE);
		m_addBufferMenu->setAttribute(Qt::WA_TranslucentBackground);
		m_addBufferMenu->setWindowFlags(m_addBufferMenu->windowFlags() | Qt::FramelessWindowHint);

		QStringList bufferLabel = CodeEditor::s_bufferLabels;
		typedef void (AssetWindowBox::* slotfunc)(void);
		static QList<slotfunc> slot_functions =
		{
			&AssetWindowBox::on_clcAddBufferA,
			&AssetWindowBox::on_clcAddBufferB,
			&AssetWindowBox::on_clcAddBufferC,
			&AssetWindowBox::on_clcAddBufferD,
			&AssetWindowBox::on_clcAddCubeMapA
		};
		for (int i = 0; i < slot_functions.size(); i++)
		{
			QString clickLabel = QString(tr("Add %1")).arg(bufferLabel[i + 1]);
			QAction* action = new QAction(clickLabel,this);
			connect(action, &QAction::triggered, this, slot_functions[i]);
			m_menuActionsMp.insert(bufferLabel[i + 1], action);
		}

    }

    AssetWindowBox::~AssetWindowBox() {
        delete m_vlay;
        delete m_label;
        delete m_window;
    }

    bool AssetWindowBox::eventFilter(QObject *obj, QEvent *event) {
        switch (event->type()) {
		    case QEvent::HoverEnter:
		    {
				QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
				effect->setOffset(0, 0);
				effect->setColor("#D4D4D4");
				effect->setBlurRadius(15);
				this->setGraphicsEffect(effect);
				return true;
		    }
            case QEvent::HoverLeave:
            {
                delete this->graphicsEffect();
                return true;
            }
            case QEvent::MouseButtonPress:
            {
                QMouseEvent* mouseEvent = (QMouseEvent*)event;
                if(mouseEvent->buttons() & Qt::LeftButton)
                {
                    if(m_window->m_crossRect.contains(m_window->mapFromGlobal(QCursor().pos())))
                    {
                        if (openedAsset)
                        {
                            if(m_window->m_assetType == AssetWindow::IMAGE) {
                                GLSessionManager::getInstance()->deleteTexture(m_index);
                                WindowAssetBar::s_glReceiver->__update();
                            }
                            if(m_window->m_assetType == AssetWindow::AUDIO)
                            {
                                SDL_UTILS::AudioManager::getAudioManager()->quit();
                                unsetupAudioSets();
                                playingAudio = false;
                            }

                            openedAsset = false;
                        }
                        else //close Buffer
                        {
                            EditorPage* currentPage = CodeEditor::s_codeEditor->currentPage();
                            currentPage->GetShader().get()->reference[m_index].reset();                            
                        }
                        m_window->m_assetType = AssetWindow::NONE;
                        m_window->repaint();
                    }
                    else
                    {
                        static QStringList pictureSuffixValidator = {"jpg","png"};
                        static QStringList audioSuffixValidator = {"mp3","wav","ogg"};
                        QFileDialog::Options options = QFileDialog::Options(QFileDialog::DontUseNativeDialog);
                        QString fileName = QFileDialog::getOpenFileName(nullptr,tr("Open Asset"), tr("Resources"),
                                    "Picture (*jpg *.png);; Audio(*.mp3 *.wav *ogg)",nullptr,options);

                        if(fileName.size())
                        {
                            diskpath = fileName.toLatin1();
                            QFileInfo fileInfo(fileName);
                            if(pictureSuffixValidator.contains(fileInfo.suffix().toLower()))
                            {
                                m_window->m_assetPath = fileName;
                                m_window->m_assetType = AssetWindow::IMAGE;
                                m_window->m_showImage = QImage(fileName).scaled(m_window->width(),m_window->height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
                                sendAssets(AssetWindow::IMAGE);
                            }
                            if(audioSuffixValidator.contains(fileInfo.suffix()))
                            {
                                if(!openedAsset)
                                {
                                    m_window->m_assetPath = fileName;
                                    m_window->m_assetType = AssetWindow::AUDIO;
                                    setupAudioSets();
                                    m_window->m_showImage = QImage(ICOPATH(music.png)).scaled(m_window->width(),m_window->height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
                                    m_window->update();
                                    sendAssets(AssetWindow::AUDIO);
                                    playingAudio = true;
                                }
                                else
                                {
                                    AudioManager::getAudioManager()->quit();
                                    m_window->m_assetPath = fileName;
                                    m_window->m_assetType = AssetWindow::AUDIO;
                                    sendAssets(AssetWindow::AUDIO);
                                }
                            }
                            openedAsset = true;
                        }
                    }
                    return true;
                }

                if(mouseEvent->buttons() & Qt::RightButton)
                {
                    QStringList bufferUsed = CodeEditor::getEditor()->buffersInUse();
                    m_addBufferMenu->clear();
                    for(auto& buffer : bufferUsed)
                    {
                        m_addBufferMenu->addAction(m_menuActionsMp[buffer]);
                    }
                    m_addBufferMenu->exec(QCursor().pos());
                }
            }
            default:
                break;
        }
        return QWidget::eventFilter(obj, event);
    }

    void AssetWindowBox::setAudioPlayDone() {
        playingAudio = false;
        on_clcAudioPause();
    }

    void AssetWindowBox::sendAssets(AssetWindow::AssetType type) {
        std::pair<SDL_GL_MODULE::GLRenderer*,SDL_GL_TASK> newTask;

        switch (type)
        {
            case AssetWindow::IMAGE:
            {
                newTask.first = WindowAssetBar::s_glReceiver;
                newTask.second.type = SDL_GL_MODULE::GL_LOAD_TEXTURE;
                newTask.second.param.loadTextureTask.attachedShader = m_attachedPage->GetShader().get();

                QByteArray s = m_label->text().toLatin1();
                newTask.second.param.loadTextureTask.index = m_index;
                s = m_window->m_assetPath.toLatin1();
                strcpy(newTask.second.param.loadTextureTask.texture_path,s.data());
                GLSessionManager::getInstance()->addTask(newTask);
            }
            case AssetWindow::AUDIO:
            {
                AudioManager::getAudioManager()->m_invoker = this;
                AudioManager::getAudioManager()->play(m_window->m_assetPath);
            }
            default:
                break;
        }
    }

    void AssetWindowBox::setupAudioSets() {
        m_vlay->replaceWidget(m_label,m_audioToolBar);
        m_label->setParent(nullptr);
    }

    void AssetWindowBox::unsetupAudioSets() {
        m_vlay->replaceWidget(m_audioToolBar,m_label);
        m_audioToolBar->setParent(nullptr);
    }

    void AssetWindowBox::on_clcAudioPause() {
        AudioManager::getAudioManager()->pause();
        disconnect(m_pauseBtn,&QPushButton::clicked,this,&AssetWindowBox::on_clcAudioPause);
        m_pauseBtn->setIcon(QIcon(ICOPATH(play.svg)));
        connect(m_pauseBtn,&QPushButton::clicked,this,&AssetWindowBox::on_clcAudioResume);
    }

    void AssetWindowBox::on_clcAudioResume() {
        if(!playingAudio)
        {
            AudioManager::getAudioManager()->replay();
        }
        AudioManager::getAudioManager()->resume();
        disconnect(m_pauseBtn,&QPushButton::clicked,this,&AssetWindowBox::on_clcAudioResume);
        m_pauseBtn->setIcon(QIcon(ICOPATH(pause.svg)));
        connect(m_pauseBtn,&QPushButton::clicked,this,&AssetWindowBox::on_clcAudioPause);
    }

    void AssetWindowBox::on_clcAudioRewind() {
        AudioManager::getAudioManager()->rewind();
    }

    void AssetWindowBox::on_clcSetAudioVolume() {
        static RadioConditioner* volumeSlider = new RadioConditioner();
        QPointF show_pos = volumeSlider->mapToGlobal(QPointF(0,0)) - QPointF(0,volumeSlider->height());
        volumeSlider->move(show_pos.toPoint());
        volumeSlider->show();
    }

	void AssetWindowBox::on_clcAudioSetting() {

	}


	void AssetWindowBox::on_clcAddBufferA()
	{
        EditorPage* refPage = CodeEditor::getEditor()->findEditorPageByLabel(CodeEditor::s_bufferLabels[1]);
        m_attachedPage->GetShader()->reference[m_index] = refPage->GetShader();
        m_window->m_showImage = QImage(ICOPATH(bufferA.png));
        m_window->m_assetType = AssetWindow::IMAGE;
        m_window->update();
	}


	void AssetWindowBox::on_clcAddBufferB()
	{
		EditorPage* refPage = CodeEditor::getEditor()->findEditorPageByLabel(CodeEditor::s_bufferLabels[2]);
        m_attachedPage->GetShader()->reference[m_index] = refPage->GetShader();
        m_window->m_showImage = QImage(ICOPATH(bufferB.png));
        m_window->m_assetType = AssetWindow::IMAGE;
        m_window->update();
	}


	void AssetWindowBox::on_clcAddBufferC()
	{
		EditorPage* refPage = CodeEditor::getEditor()->findEditorPageByLabel(CodeEditor::s_bufferLabels[3]);
		m_attachedPage->GetShader()->reference[m_index] = refPage->GetShader();
        m_window->m_showImage = QImage(ICOPATH(bufferC.png));
        m_window->m_assetType = AssetWindow::IMAGE;
        m_window->update();
	}


	void AssetWindowBox::on_clcAddBufferD()
	{
		EditorPage* refPage = CodeEditor::getEditor()->findEditorPageByLabel(CodeEditor::s_bufferLabels[4]);
		m_attachedPage->GetShader()->reference[m_index] = refPage->GetShader();
        m_window->m_showImage = QImage(ICOPATH(bufferD.png));
        m_window->m_assetType = AssetWindow::IMAGE;
        m_window->update();
	}


	void AssetWindowBox::on_clcAddCubeMapA()
	{
		EditorPage* refPage = CodeEditor::getEditor()->findEditorPageByLabel(CodeEditor::s_bufferLabels[5]);
        refPage->GetShader()->type = SCRIPT_CUBEMAP;
		m_attachedPage->GetShader()->reference[m_index] = refPage->GetShader();
        m_window->m_showImage = QImage(ICOPATH(cubemapA.png));
        m_window->m_assetType = AssetWindow::IMAGE;
        m_window->update();
	}

	WindowAssetBar::WindowAssetBar(EditorPage* page) {
        this->setAttribute(Qt::WA_TranslucentBackground,true);
        this->setFixedHeight(140);

        QWidget* wrapper = new QWidget();
        QHBoxLayout* hlay = new QHBoxLayout();
        hlay->setContentsMargins(10,0,10,0);

        for(int i = 0;i<4;i++)
        {
            AssetWindowBox* assetWin = new AssetWindowBox(i);
            assetWin->setAttachedPage(page);
            assetWin->setParent(this);
            m_assetWindowList.append(assetWin);
            assetWin->setFixedWidth(UI_COMPONENT_WIDTH(window_asset_width_factor));
            hlay->addWidget(assetWin);
        }

        wrapper->setLayout(hlay);
        this->setWidget(wrapper);
    }


	WindowAssetBar::~WindowAssetBar() {
        for(int i = 0; i < 4;i++)
        {
            if(m_assetWindowList[i] != nullptr)
            {
                delete m_assetWindowList[i];
            }
        }
    }

    int WindowAssetBar::totalAsset()
    {
        return std::count_if(m_assetWindowList.begin(), m_assetWindowList.end(), [&](AssetWindowBox* windowbox) {return windowbox->getAssetPath() != nullptr; });
    }

    void WindowAssetBar::setAssetsReciver(SDL_GL_MODULE::GLRenderer *receiver) {
        WindowAssetBar::s_glReceiver = receiver;
    }
}