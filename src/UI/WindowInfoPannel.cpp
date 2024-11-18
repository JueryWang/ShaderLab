#include "WindowInfoPannel.h"
#include "UI_DefaultDfs.h"
#include "GLWidget.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPixmap>


namespace SDL_UI_MODULE
{
    SDL_UI_MODULE::WindowInfoPanel::WindowInfoPanel(QWidget *parent, const QSize &initSize, const QString &source) : m_SourceSize(initSize),m_WindowSize(initSize),m_sourceLabel(source){
        this->setParent(parent);
        this->setStyleSheet(WINDOW_FONT_COLOR_STYLE);

        QHBoxLayout *hlay = new QHBoxLayout(this);
        m_SourceShowText = new QLabel(this);
        m_SourceInfoText = new QLabel(this);
        m_SizeShowText = new QLabel(this);
        m_SizeInfoText = new QLabel(this);
        m_fpsInfoText = new QLabel(this);
        m_lockBtn = new QPushButton(this);
        m_resetBtn = new QPushButton(this);
        m_recBtn = new QPushButton(this);
        m_recBtn->setIconSize(QSize(20,20));
        m_soundBtn = new QPushButton(this);
        m_soundBtn->setIconSize(QSize(20,20));
        QLabel *leftLine = new QLabel(this);
        QLabel *rightLine = new QLabel(this);

        QString sourceText = QString("%1 %2x%3").arg(m_sourceLabel).arg(m_SourceSize.width()).arg(m_SourceSize.height());
        m_SourceShowText->setText(sourceText);
        QString crtSizeText = QString(tr("Current size: %1x%2")).arg(m_WindowSize.width()).arg(m_WindowSize.height());
        m_SizeInfoText->setText(crtSizeText);
        m_fpsInfoText->setText(tr("FPS: 0"));

        QPixmap vertical_pix = QPixmap(ICOPATH(vertical_pix.svg));
        leftLine->setPixmap(vertical_pix);
        leftLine->setFixedWidth(16);
        rightLine->setPixmap(vertical_pix);
        rightLine->setFixedWidth(16);

        m_lockBtn->setIconSize(QSize(16,16));
        m_lockBtn->setIcon(QIcon(ICOPATH(locksize.svg)));
        m_lockBtn->setText(tr("Lock"));
        m_lockBtn->setStyleSheet(WINDOWINFO_BTN_STYLE_1);

        m_resetBtn->setIconSize(QSize(16,16));
        m_resetBtn->setIcon(QIcon(ICOPATH(reset.svg)));
        m_resetBtn->setText(tr("Resize"));
        m_resetBtn->setStyleSheet(WINDOWINFO_BTN_STYLE_1);

        m_recBtn->setFixedWidth(30);
        m_recBtn->setIcon(QIcon(ICOPATH(record.svg)));
        m_recBtn->setStyleSheet(WINDOWINFO_BTN_STYLE_2);
        connect(m_recBtn,&QPushButton::clicked,this,&WindowInfoPanel::on_clcRecord);

        m_soundBtn->setFixedWidth(30);
        m_soundBtn->setIcon(QIcon(ICOPATH(soundon.svg)));
        m_soundBtn->setStyleSheet(WINDOWINFO_BTN_STYLE_2);
        connect(m_soundBtn, &QPushButton::clicked, this, &WindowInfoPanel::on_clcSoundOff);

        hlay->addWidget(m_SourceShowText);
        hlay->addSpacing(2);
        hlay->addWidget(m_SourceInfoText);
        hlay->addSpacing(10);
        hlay->addWidget(m_SizeShowText);
        hlay->addSpacing(2);
        hlay->addWidget(m_SizeInfoText);
        hlay->addSpacing(20);
        hlay->addWidget(m_fpsInfoText);
        hlay->addSpacing(10);
        hlay->addWidget(leftLine);
        hlay->addWidget(m_resetBtn);
        hlay->addWidget(m_lockBtn);
        hlay->addSpacing(10);
        hlay->addWidget(m_resetBtn);
        hlay->addWidget(rightLine);
        hlay->addWidget(m_recBtn);
        hlay->addSpacing(10);
        hlay->addWidget(m_soundBtn);
        hlay->setContentsMargins(20,0,20,0);

        setLayout(hlay);
    }

    SDL_UI_MODULE::WindowInfoPanel::~WindowInfoPanel()
    {
    }

    void SDL_UI_MODULE::WindowInfoPanel::updateInfo(const QSize &newSize) {
        m_WindowSize = newSize;
        QString sizeText = QString(tr("Current size %1x%2").arg(m_WindowSize.width()).arg(m_WindowSize.height()));
        m_SizeInfoText->setText(sizeText);
    }

	void WindowInfoPanel::updateFPS(int fps)
	{
        m_fpsInfoText->setText(QString(tr("FPS: %1").arg(fps)));
	}

    void SDL_UI_MODULE::WindowInfoPanel::on_clcRecord() {
        disconnect(m_recBtn,&QPushButton::clicked,this,&WindowInfoPanel::on_clcRecord);
        m_recBtn->setIcon(QIcon(ICOPATH(recording.svg)));
        if(glWindowInst)
        {
            glWindowInst->__startRecord();
        }
        connect(m_recBtn,&QPushButton::clicked,this,&WindowInfoPanel::on_clcEndRecord);
    }

    void SDL_UI_MODULE::WindowInfoPanel::on_clcEndRecord() {
        disconnect(m_recBtn,&QPushButton::clicked,this,&WindowInfoPanel::on_clcEndRecord);
        m_recBtn->setIcon(QIcon(ICOPATH(record.svg)));
        if(glWindowInst)
        {
            glWindowInst->__endRecord();
        }
        connect(m_recBtn,&QPushButton::clicked,this,&WindowInfoPanel::on_clcRecord);
    }

    void SDL_UI_MODULE::WindowInfoPanel::on_clcSoundOn() {
        disconnect(m_soundBtn,&QPushButton::clicked,this,&WindowInfoPanel::on_clcSoundOn);
        m_soundBtn->setIcon(QIcon(ICOPATH(soundon.svg)));
        connect(m_soundBtn,&QPushButton::clicked,this,&WindowInfoPanel::on_clcSoundOff);
    }

    void SDL_UI_MODULE::WindowInfoPanel::on_clcSoundOff() {
        disconnect(m_soundBtn,&QPushButton::clicked,this,&WindowInfoPanel::on_clcSoundOff);
        m_soundBtn->setIcon(QIcon(ICOPATH(soundoff.svg)));
        connect(m_soundBtn,&QPushButton::clicked,this,&WindowInfoPanel::on_clcSoundOn);
    }
}
