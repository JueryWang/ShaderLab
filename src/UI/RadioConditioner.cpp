#include "RadioConditioner.h"
#include "Utility/Utils_SessionManager.h"
#include <QApplication>
#include <QEvent>
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>

namespace SDL_UI_MODULE
{
    RadioConditioner::RadioConditioner() {
        setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);

        this->setFixedSize(20,75);
        this->setStyleSheet("QWidget{background-color:rgba{244,228,231,155};border-radius:4px;}");

        m_slider = new QSlider();
        m_slider->setFixedSize(20,70);
        m_slider->setMaximum(100);
        m_slider->setMinimum(0);
        m_slider->setValue(0);
        m_slider->setStyleSheet("QSlider{background:transparent;}"
                                "QSlider::add-page:vertical{background-color:#FFFFFF;width:4px;border-radius:2px;}"
                                "QSlider::sub-page:vertical{background-color:rgba(196, 196, 196, 0.5);width:4px;border-radius:2px;}"
                                "QSlider::groove:vertical{background:transparent;width:4px;border-radius:2px;}"
                                "QSlider::handle:vertical{height:20px;width:20px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
                                "QSlider::handle:vertical:hover{height:20px;width:20px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
                                "QSlider::handle:vertical:pressed{height:20px;width:20px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}");

        connect(m_slider,&QSlider::valueChanged,this,&RadioConditioner::onVolumeChanged);

        QVBoxLayout* vlay = new QVBoxLayout();
        vlay->setContentsMargins(0,3,0,3);
        vlay->setAlignment(Qt::AlignCenter);
        vlay->setSpacing(0);
        vlay->addWidget(m_slider);

        this->setLayout(vlay);
        raise();

        setValue(80);
    }

    RadioConditioner::~RadioConditioner() {
        delete m_slider;
    }

    void RadioConditioner::setValue(int value) {
        if(m_slider != nullptr)
        {
            m_slider->setValue(value);
        }
    }

    void RadioConditioner::onVolumeChanged() {
        if(m_slider != nullptr)
        {
            int value = m_slider->value();

            if(value > 0 && value < 100)
            {
                m_slider->setStyleSheet("QSlider{background:transparent;}"
                                        "QSlider::add-page:vertical{background-color:#FFFFFF;width:4px;border-radius:2px;}"
                                        "QSlider::sub-page:vertical{background-color:rgba(196, 196, 196, 0.5);width:4px;border-radius:2px;}"
                                        "QSlider::groove:vertical{background:transparent;width:4px;border-radius:2px;}"
                                        "QSlider::handle:vertical{height:20px;width:21px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
                                        "QSlider::handle:vertical:hover{height:20px;width:21px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
                                        "QSlider::handle:vertical:pressed{height:20px;width:21px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}");
            }else
            {
                m_slider->setStyleSheet("QSlider{background:transparent;}"
                                        "QSlider::add-page:vertical{background-color:#FFFFFF;width:4px;border-radius:2px;}"
                                        "QSlider::sub-page:vertical{background-color:rgba(0, 0, 0, 0.8);width:4px;border-radius:2px;}"
                                        "QSlider::groove:vertical{background:transparent;width:4px;border-radius:2px;}"
                                        "QSlider::handle:vertical{height:20px;width:21px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
                                        "QSlider::handle:vertical:hover{height:20px;width:21px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
                                        "QSlider::handle:vertical:pressed{height:20px;width:21px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}");
            }

            SDL_UTILS::Utils_SessionManager::getInstance()->setAudioVolume(value/80.0);
        }
    }

    void RadioConditioner::paintEvent(QPaintEvent *event) {
        QStyleOption option;
        option.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget,&option,&p,this);
    }

    bool RadioConditioner::event(QEvent *e) {
        if(e->type() == QEvent::ActivationChange)
        {
            if(QApplication::activeWindow() != this)
            {
                this->close();
            }
        }

        return QWidget::event(e);
    }
}
