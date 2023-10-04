#include "uimodule_AssetsWindow.h"
#include "ui_defaultDfs.h"
#include "Utilitys/uitilityDfs.h"
#include "GL/gl_defaultDfs.h"
#include "GL/glmodule_backstatge.h"
#include "GL/glmodule_render.h"
#include "Utilitys/AssetsManager/Audio/utils_audioPlayer.h"
#include "uimodule_radioVolume.h"
#include "uimodule_codeEditor.h"
#include <QFont>
#include <QFileDialog>
#include <QMouseEvent>
#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>
#include <QFileInfo>
#include <QPainter>
#include <QProcess>
#include <QAction>
#include <QStringListModel>
#include <QDebug>

XA_GLMODULE_RENDER* XA_UIMODULE_ASSET_BAR::_glReciver;
QMenu* addBufferMenu;
QMap<QString, QAction*> menuActionsMp;
void inline clearMenuActions();

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
	
	_audioToolbar = new QWidget(this);
	_audioToolbar->setStyleSheet(R"(.QWidget{
                        background-color:white;
                        border-bottom-left-radius:5px;
                        border-bottom-right-radius:5px;
                        })");
	_audioToolbar->setFixedHeight(20);
	_audioSetsLay = new QHBoxLayout();
	_audioSetsLay->setContentsMargins(0, 0, 0, 0);
	_audioSetsLay->setSpacing(0);

	//for audio
	_label = new QLabel();
	_label->setAlignment(Qt::AlignLeft);
	_label->setText(QString(" iChannel%1").arg(index));
	_label->setStyleSheet(R"(.QLabel{
                        background-color:white;
                        border-bottom-left-radius:5px;
                        border-bottom-right-radius:5px;
                        })");
	QFont label_font;
	label_font.setFamily(global_font_mp["OpenSans-Regular"]);
	label_font.setPointSize(10);
	_label->setFont(label_font);
	_label->setFixedHeight(20);
	
	pauseBtn = new QPushButton(_audioToolbar);
	pauseBtn->setAttribute(Qt::WA_TranslucentBackground, true); 
	pauseBtn->setStyleSheet(WINDOW_ASEETBAR_BTN_STYLE);
	pauseBtn->setIconSize(QSize(13, 13));
	pauseBtn->setFixedWidth(20);
	pauseBtn->setIcon(QIcon(ICOPATH(pause.svg)));
	connect(pauseBtn, &QPushButton::clicked, this, &XA_UIMODULE_ASSET_WINDOW::on_clcAudioPause);

	rewindBtn = new QPushButton(_audioToolbar);
	rewindBtn->setAttribute(Qt::WA_TranslucentBackground, true); 
	rewindBtn->setStyleSheet(WINDOW_ASEETBAR_BTN_STYLE);
	rewindBtn->setFixedWidth(20);
	rewindBtn->setIconSize(QSize(13, 13));
	rewindBtn->setIcon(QIcon(ICOPATH(rewind.svg)));
	connect(rewindBtn, &QPushButton::clicked, this, &XA_UIMODULE_ASSET_WINDOW::on_clcAudioRewind);

	volumeBtn = new QPushButton(_audioToolbar);
	volumeBtn->setAttribute(Qt::WA_TranslucentBackground, true);
	volumeBtn->setStyleSheet(WINDOW_ASEETBAR_BTN_STYLE);
	volumeBtn->setIconSize(QSize(18, 18));
	volumeBtn->setFixedWidth(20);
	volumeBtn->setIcon(QIcon(ICOPATH(audio-volume.svg)));
	connect(volumeBtn, &QPushButton::clicked, this, &XA_UIMODULE_ASSET_WINDOW::on_clcAudioVolume);

	settingBtn = new QPushButton(_audioToolbar);
	settingBtn->setAttribute(Qt::WA_TranslucentBackground, true);
	settingBtn->setStyleSheet(WINDOW_ASEETBAR_BTN_STYLE);
	settingBtn->setIconSize(QSize(13, 13));
	settingBtn->setFixedWidth(20);
	settingBtn->setIcon(QIcon(ICOPATH(setting.svg)));
	connect(settingBtn, &QPushButton::clicked, this, &XA_UIMODULE_ASSET_WINDOW::on_clcAudioSetting);
	
	_audioSetsLay->addWidget(pauseBtn, Qt::AlignRight);
	_audioSetsLay->addWidget(rewindBtn, Qt::AlignRight);
	_audioSetsLay->addWidget(volumeBtn, Qt::AlignRight);
	_audioSetsLay->addWidget(settingBtn, Qt::AlignRight);
	_audioToolbar->setLayout(_audioSetsLay);

	_vlay->addWidget(_window);
	_vlay->addWidget(_label);
	_vlay->setSpacing(0);
	this->setLayout(_vlay);
}

bool XA_UIMODULE_ASSET_WINDOW::eventFilter(QObject* obj, QEvent* event)
{
	switch (event->type())
	{
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
		if (mouseEvent->buttons() & Qt::LeftButton)
		{
			if (opened_asset && _window->cross_rect.contains(_window->mapFromGlobal(QCursor().pos())))//close
			{
				if (_window->asset_type == ASSET_WINDOW::IMAGE)
				{
					XA_GLMODULE_BACKSTG::getBackStage()->deleteTexture(_index);
					XA_UIMODULE_ASSET_BAR::_glReciver->__update();
				}
				if (_window->asset_type == ASSET_WINDOW::AUDIO)
				{
					XA_AUDIO_PLAYER::get_player()->quit();
					unsetupAudioSets();
					playingAudio = false;
				}

				opened_asset = false;
				_window->asset_type = ASSET_WINDOW::NONE;
				_window->repaint();
			}
			else
			{
				static QStringList pictureSuffixValidator = { "jpg","png" };
				static QStringList audioSuffixValidator = { "mp3","wav","ogg" };
				QString fileName = QFileDialog::getOpenFileName(this, _STRING_WRAPPER("打开文件"), "Resources",
					"Picuture (*.jpg *.png);; Audio (*.mp3 *.wav *.ogg)");
				if (fileName.size())
				{
					QFileInfo fileInfo(fileName);
					if (pictureSuffixValidator.contains(fileInfo.suffix().toLower()))
					{
						_window->asset_path = fileName;
						_window->asset_type = ASSET_WINDOW::IMAGE;
						_window->show_img = QImage(fileName).scaled(_window->width(), _window->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
						_window->update();
						sendAssets(ASSET_WINDOW::IMAGE);
					}
					if (audioSuffixValidator.contains(fileInfo.suffix()))
					{
						if (!opened_asset)
						{
							_window->asset_path = fileName;
							_window->asset_type = ASSET_WINDOW::AUDIO;
							setupAudioSets();
							_window->show_img = QImage(ICOPATH(music.png)).scaled(_window->width(), _window->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
							_window->update();
							sendAssets(ASSET_WINDOW::AUDIO);
							playingAudio = true;
						}
						else
						{
							XA_AUDIO_PLAYER::get_player()->quit();
							_window->asset_path = fileName;
							_window->asset_type = ASSET_WINDOW::AUDIO;
							sendAssets(ASSET_WINDOW::AUDIO);
							//Show MessageBox
						}
					}
					opened_asset = true;
				}
			}
			return true;
		}

		if (mouseEvent->buttons() & Qt::RightButton)
		{
			QStringList bufferUsed = XA_UIMODULE_CodeEditor::getEditor()->buffersInUse();
			clearMenuActions();
			for (auto& buffer : bufferUsed)
			{
				addBufferMenu->addAction(menuActionsMp[buffer]);
			}
			addBufferMenu->exec(QCursor().pos());
		}
	}
	default:
		break;
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
		new_task.first = XA_UIMODULE_ASSET_BAR::_glReciver;
		new_task.second.type = XA_GL_LOADTEXTURE;
		
		QByteArray s = _label->text().toLatin1();
		new_task.second.param.loadTexture_param.index = _index;
		s = _window->asset_path.toLatin1();
		strcpy(new_task.second.param.loadTexture_param.texture_path, s.data());
		XA_GLMODULE_BACKSTG::getBackStage()->addTask(new_task);
		break;
	}
	case ASSET_WINDOW::AUDIO:
	{
		XA_AUDIO_PLAYER::get_player()->play(_window->asset_path);
		XA_AUDIO_PLAYER::get_player()->_invoker = this;
		break;
	}
	default:
		break;
	}
}

void XA_UIMODULE_ASSET_WINDOW::setupAudioSets()
{
	_vlay->replaceWidget(_label,_audioToolbar);
	_label->setParent(NULL);
}

void XA_UIMODULE_ASSET_WINDOW::unsetupAudioSets()
{
	_vlay->replaceWidget(_audioToolbar,_label);
	_audioToolbar->setParent(NULL);
}

void XA_UIMODULE_ASSET_WINDOW::setAudioPlayDone()
{
	playingAudio = false;
	this->on_clcAudioPause();
}

void XA_UIMODULE_ASSET_WINDOW::on_clcAudioPause()
{
	XA_AUDIO_PLAYER::get_player()->pause();
	disconnect(pauseBtn, &QPushButton::clicked, this, &XA_UIMODULE_ASSET_WINDOW::on_clcAudioPause);
	pauseBtn->setIcon(QIcon(ICOPATH(play.svg)));
	connect(pauseBtn, &QPushButton::clicked, this, &XA_UIMODULE_ASSET_WINDOW::on_clcAudioResume);
}

void XA_UIMODULE_ASSET_WINDOW::on_clcAudioResume()
{
	if (playingAudio == false)
	{
		XA_AUDIO_PLAYER::get_player()->replay();
	}
	XA_AUDIO_PLAYER::get_player()->resume();
	disconnect(pauseBtn, &QPushButton::clicked, this, &XA_UIMODULE_ASSET_WINDOW::on_clcAudioResume);
	pauseBtn->setIcon(QIcon(ICOPATH(pause.svg)));
	connect(pauseBtn, &QPushButton::clicked, this, &XA_UIMODULE_ASSET_WINDOW::on_clcAudioPause);
}

void XA_UIMODULE_ASSET_WINDOW::on_clcAudioRewind()
{
	XA_AUDIO_PLAYER::get_player()->rewind();
}

void XA_UIMODULE_ASSET_WINDOW::on_clcAudioVolume()
{
	XA_UIMODULE_RadioVolume* volumeSlider = new XA_UIMODULE_RadioVolume();
	QPoint show_pos = volumeBtn->mapToGlobal(QPoint(0, 0))- QPoint(0,volumeSlider->height());
	volumeSlider->move(show_pos);
	volumeSlider->show();
}

void XA_UIMODULE_ASSET_WINDOW::on_clcAudioSetting()
{

}

XA_UIMODULE_ASSET_BAR::XA_UIMODULE_ASSET_BAR(int width)
{
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	this->setFixedHeight(140);

	QWidget* wrapper = new QWidget();
	QHBoxLayout* hlay = new QHBoxLayout();
	hlay->setContentsMargins(10, 0, 10, 0);

	for (int i = 0; i < 4; i++)
	{
		XA_UIMODULE_ASSET_WINDOW* asset_win = new XA_UIMODULE_ASSET_WINDOW(i);
		asset_win_list.append(asset_win);
		asset_win->setFixedWidth(200);
		hlay->addWidget(asset_win);
	}
	wrapper->setLayout(hlay);
	this->setWidget(wrapper);

	
	addBufferMenu = new QMenu(this);
	addBufferMenu->setStyleSheet(TYPES_MENU_STYLE);
	addBufferMenu->setAttribute(Qt::WA_TranslucentBackground); //Menu背景透明
	addBufferMenu->setWindowFlags(addBufferMenu->windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);


	QStringList labels = XA_UIMODULE_CodeEditor::bufferLabels;
	typedef void (XA_UIMODULE_ASSET_BAR::* slot_func)(void);
	QList<slot_func> slot_functions = { &XA_UIMODULE_ASSET_BAR::act_addCommon,
										&XA_UIMODULE_ASSET_BAR::act_addBufferA,
										&XA_UIMODULE_ASSET_BAR::act_addBufferB,
										&XA_UIMODULE_ASSET_BAR::act_addBufferC,
										&XA_UIMODULE_ASSET_BAR::act_addBufferD,
										&XA_UIMODULE_ASSET_BAR::act_addCubeMap};
	int size = labels.size();
	for (int i = 0; i < size; i++)
	{
		QAction* action = new QAction(labels[i]);
		connect(action, &QAction::triggered, this, slot_functions[i]);
		menuActionsMp.insert(labels[i], action);
	}
}

void XA_UIMODULE_ASSET_BAR::setAssetsReciver(XA_GLMODULE_RENDER* reciver)
{
	XA_UIMODULE_ASSET_BAR::_glReciver = reciver;
}

void XA_UIMODULE_ASSET_BAR::act_addCommon()
{

}

void XA_UIMODULE_ASSET_BAR::act_addBufferA()
{

}

void XA_UIMODULE_ASSET_BAR::act_addBufferB()
{

}

void XA_UIMODULE_ASSET_BAR::act_addBufferC()
{

}

void XA_UIMODULE_ASSET_BAR::act_addBufferD()
{

}

void XA_UIMODULE_ASSET_BAR::act_addCubeMap()
{

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
		painter.drawImage(rect, show_img);
		painter.drawPixmap(cross_rect, cross.grab());
		painter.end();
		break;
	default:
		break;
	}
}

void inline clearMenuActions()
{
	for (auto& act : menuActionsMp.values())
	{
		addBufferMenu->removeAction(act);
	}
}