#include "uimodule_AssetsWindow.h"
#include "ui_defaultDfs.h"
#include "../Utilitys/uitilityDfs.h"
#include "../GL/gl_defaultDfs.h"
#include "../GL/glmodule_backstatge.h"
#include "../GL/glmodule_render.h"
#include <QFont>
#include <QFileDialog>
#include <QMouseEvent>
#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>
#include <QFileInfo>
#include <QPainter>
#include <QDebug>

XA_GLMODULE_RENDER* XA_UIMODULE_ASSET_BAR::_reciver;

XA_UIMODULE_ASSET_WINDOW::XA_UIMODULE_ASSET_WINDOW(int index):_index(index)
{
	this->setAttribute(Qt::WA_Hover);
	this->installEventFilter(this);

	int fontId = QFontDatabase::addApplicationFont(FONTPATH(OpenSans-Regular.ttf));
	QStringList font_list = QFontDatabase::applicationFontFamilies(fontId);
	global_font_mp["OpenSans-Regular"] = font_list[0];


	_vlay = new QVBoxLayout();
	_vlay->setContentsMargins(0, 0, 0, 0);

	_window = new ASSET_WINDOW(QSize(200, 100));
	
	_label = new QLabel();
	_label->setAlignment(Qt::AlignLeft);
	_label->setText(QString(" iChannel%1").arg(index));
	QFont label_font;
	label_font.setFamily(global_font_mp["OpenSans-Regular"]);
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

	if (event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent* mouseEvent = (QMouseEvent*)event;
		if (mouseEvent->buttons() && Qt::LeftButton)
		{
			if (opened_asset && _window->cross_rect.contains(_window->mapFromGlobal(QCursor().pos())))
			{
				if (_window->asset_type == ASSET_WINDOW::IMAGE)
				{
					XA_GLMODULE_BACKSTG::getBackStage()->deleteTexture(_index);
					XA_UIMODULE_ASSET_BAR::_reciver->__update();
				}
				opened_asset = false;
				_window->asset_type = ASSET_WINDOW::NONE;
				_window->repaint();
			}
			else
			{
				static QStringList pictureSuffixValidator = { "jpg","png" };
				static QStringList audioSuffixValidator = {"mp3","wav","ogg"};
				QString fileName = QFileDialog::getOpenFileName(this, _STRING_WRAPPER("打开文件"), "Resources",
					"Picuture (*.jpg *.png);; Audio (*.mp3 *.wav *.ogg)");
				if (fileName.size())
				{
					QFileInfo fileInfo(fileName);
					if (pictureSuffixValidator.contains(fileInfo.suffix().toLower()))
					{
						_window->asset_path = fileName;
						_window->asset_type = ASSET_WINDOW::IMAGE;
						_window->show_img = QImage(fileName).scaled(_window->width(),_window->height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
						_window->update();
						sendAssets(ASSET_WINDOW::IMAGE);
					}
					if (audioSuffixValidator.contains(fileInfo.suffix()))
					{
						_window->asset_path = fileName;
						_window->asset_type = ASSET_WINDOW::AUDIO;
					}
					opened_asset = true;
			}
			}
			return true;
		}
	}
	return QWidget::eventFilter(obj, event);
}

void XA_UIMODULE_ASSET_WINDOW::sendAssets(ASSET_WINDOW::AssetType type)
{
	std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK> new_task;

	switch (type)
	{
	case ASSET_WINDOW::IMAGE:
	{	
		new_task.first = XA_UIMODULE_ASSET_BAR::_reciver;
		new_task.second.type = XA_GL_LOADTEXTURE;
		
		QByteArray s = _label->text().toLatin1();
		new_task.second.param.loadTexture_param.index = _index;
		s = _window->asset_path.toLatin1();
		strcpy(new_task.second.param.loadTexture_param.texture_path, s.data());
		XA_GLMODULE_BACKSTG::getBackStage()->addTask(new_task);
		break;
	}
	case ASSET_WINDOW::AUDIO:
		break;
	default:
		break;
	}
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

void XA_UIMODULE_ASSET_BAR::setAssetsReciver(XA_GLMODULE_RENDER* reciver)
{
	XA_UIMODULE_ASSET_BAR::_reciver = reciver;
}

ASSET_WINDOW::ASSET_WINDOW(const QSize& size)
{
	this->setStyleSheet(R"(.QWidget{
                        background-color:black;
                        border:none;
                        border-top-left-radius:5px;
                        border-top-right-radius:5px;
                        })");
	this->setFixedSize(size);
	this->asset_type = NONE;

	QPixmap crossPix = QPixmap(ICOPATH(cross.svg));
	cross.setFixedSize(QSize(20, 20));
	cross.setPixmap(crossPix);
	cross.setAttribute(Qt::WA_TranslucentBackground);
	cross_rect = QRect(this->width() - 25, 5, 20, 20);
}

void ASSET_WINDOW::paintEvent(QPaintEvent* event)
{
	static QRect rect = QRect(0, 0, this->width(), this->height());
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	
	switch (asset_type)
	{
	case ASSET_WINDOW::IMAGE:
		painter.drawImage(rect, show_img);
		painter.drawPixmap(cross_rect, cross.grab());
		painter.end();
		break;
	case ASSET_WINDOW::AUDIO:
		break;
	default:
		break;
	}
}
