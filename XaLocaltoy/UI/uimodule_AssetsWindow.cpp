#include "../Utilitys/uitilityDfs.h"
#include "uimodule_AssetsWindow.h"
#include "ui_defaultDfs.h"
#include <QFont>
#include <QFileDialog>
#include <QMouseEvent>
#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>
#include <QDebug>

XA_UIMODULE_ASSET_WINDOW::XA_UIMODULE_ASSET_WINDOW(int index)
{
	this->setAttribute(Qt::WA_Hover);
	this->installEventFilter(this);

	int fontId = QFontDatabase::addApplicationFont(FONTPATH(OpenSans-Regular.ttf));
	QStringList font_list = QFontDatabase::applicationFontFamilies(fontId);


	_vlay = new QVBoxLayout();
	_vlay->setContentsMargins(0, 0, 0, 0);

	_window = new QWidget();
	_window->setStyleSheet(R"(.QWidget{
                        background-color:black;
                        border:none;
                        border-top-left-radius:5px;
                        border-top-right-radius:5px;
                        })");
	_window->setFixedSize(200, 100);
	
	_label = new QLabel();
	_label->setAlignment(Qt::AlignLeft);
	_label->setText(QString(" iChannel%1").arg(index));
	QFont label_font;
	label_font.setFamily(font_list[0]);
	label_font.setPointSize(10);
	_label->setFont(label_font);
	_label->setFixedHeight(25);
	_label->setStyleSheet(R"(.QLabel{
                        background-color:white;
                        border-bottom-left-radius:5px;
                        border-bottom-right-radius:5px;
                        })");
	_vlay->addWidget(_window);
	_vlay->addWidget(_label);
	_vlay->setSpacing(0);
	this->setLayout(_vlay);
}



bool XA_UIMODULE_ASSET_WINDOW::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::HoverEnter) 
	{
		QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
		effect->setOffset(0, 0);
		effect->setColor("#D4D4D4");
		effect->setBlurRadius(15);
		this->setGraphicsEffect(effect);
		return true;
	}

	if (event->type() == QEvent::HoverLeave)
	{
		delete this->graphicsEffect();
		return true;
	}


	if (obj == this->_window && event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent* mouseEvent = (QMouseEvent*)event;
		if (mouseEvent->buttons() & Qt::LeftButton)
		{
			QString fileName = QFileDialog::getOpenFileName(this, _STRING_WRAPPER("打开文件"), "Resources",
				"Picuture (*.jpg *.png);; Audio (*.mp3 *.wav *.ogg)");
			return true;
		}
	}
	return QWidget::eventFilter(obj, event);
}

XA_UIMODULE_ASSET_BAR::XA_UIMODULE_ASSET_BAR(int width)
{
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	this->setFixedHeight(140);

	QWidget* wrapper = new QWidget();
	QHBoxLayout* hlay = new QHBoxLayout();
	hlay->setContentsMargins(10, 0, 10, 0);

	for (int i = 0; i < 10; i++)
	{
		XA_UIMODULE_ASSET_WINDOW* asset_win = new XA_UIMODULE_ASSET_WINDOW(i);
		asset_win_list.append(asset_win);
		asset_win->setFixedWidth(200);
		hlay->addWidget(asset_win);
	}
	wrapper->setLayout(hlay);
	this->setWidget(wrapper);
}