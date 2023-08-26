#include "uimodule_radioVolume.h"
#include "../Utilitys/utils_backendThread.h"
#include <QApplication>
#include <QEvent>
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>

XA_UIMODULE_RadioVolume::XA_UIMODULE_RadioVolume()
{
	setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
	
	this->setFixedSize(20, 75);
	this->setStyleSheet("QWidget{background-color:rgba(244, 228, 231, 155);border-radius:4px;}");

	_slider = new QSlider();
	_slider->setFixedSize(20, 70);
	_slider->setMaximum(100);
	_slider->setMinimum(0);
	_slider->setValue(0);
	_slider->setStyleSheet("QSlider{background:transparent;}"
		"QSlider::add-page:vertical{background-color:#FFFFFF;width:4px;border-radius:2px;}"
		"QSlider::sub-page:vertical{background-color:rgba(196, 196, 196, 0.5);width:4px;border-radius:2px;}"
		"QSlider::groove:vertical{background:transparent;width:4px;border-radius:2px;}"
		"QSlider::handle:vertical{height:20px;width:20px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
		"QSlider::handle:vertical:hover{height:20px;width:20px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
		"QSlider::handle:vertical:pressed{height:20px;width:20px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}");

	connect(_slider, &QSlider::valueChanged, this, &XA_UIMODULE_RadioVolume::onVolumeChanged);

	QVBoxLayout* vlay = new QVBoxLayout();
	vlay->setContentsMargins(0, 3, 0, 3);
	vlay->setAlignment(Qt::AlignCenter);
	vlay->setSpacing(0);
	vlay->addWidget(_slider);

	this->setLayout(vlay);
	raise();

	setValue(100);
}

XA_UIMODULE_RadioVolume::~XA_UIMODULE_RadioVolume()
{
	delete _slider;
}

void XA_UIMODULE_RadioVolume::setValue(int value)
{
	if (_slider != nullptr)
	{
		_slider->setValue(value);
	}
}

void XA_UIMODULE_RadioVolume::onVolumeChanged()
{
	if (_slider != nullptr)
	{
		int value = _slider->value();

		if (value > 0 && value < 100)
		{
			_slider->setStyleSheet("QSlider{background:transparent;}"
				"QSlider::add-page:vertical{background-color:#FFFFFF;width:4px;border-radius:2px;}"
				"QSlider::sub-page:vertical{background-color:rgba(196, 196, 196, 0.5);width:4px;border-radius:2px;}"
				"QSlider::groove:vertical{background:transparent;width:4px;border-radius:2px;}"
				"QSlider::handle:vertical{height:20px;width:21px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
				"QSlider::handle:vertical:hover{height:20px;width:21px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
				"QSlider::handle:vertical:pressed{height:20px;width:21px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}");
		}
		else
		{
			_slider->setStyleSheet("QSlider{background:transparent;}"
				"QSlider::add-page:vertical{background-color:#FFFFFF;width:4px;border-radius:2px;}"
				"QSlider::sub-page:vertical{background-color:rgba(0, 0, 0, 0.8);width:4px;border-radius:2px;}"
				"QSlider::groove:vertical{background:transparent;width:4px;border-radius:2px;}"
				"QSlider::handle:vertical{height:20px;width:21px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
				"QSlider::handle:vertical:hover{height:20px;width:21px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
				"QSlider::handle:vertical:pressed{height:20px;width:21px;margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}");
		}

		XA_UTILS_BACKEND::getUtilBackend()->setAuVolume(value / 100.0);
	}
}

void XA_UIMODULE_RadioVolume::paintEvent(QPaintEvent* event)
{
	QStyleOption option;
	
	option.init(this);

	QPainter p(this);

	style()->drawPrimitive(QStyle::PE_Widget, &option, &p, this);
}

bool XA_UIMODULE_RadioVolume::event(QEvent* e)
{
	if (e->type() == QEvent::ActivationChange)
	{
		if (QApplication::activeWindow() != this)
		{
			this->close();
		}
	}

	return QWidget::event(e);
}
