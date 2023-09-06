#include "uimodule_windowInfo.h"
#include "ui_defaultDfs.h"
#include "Utilitys/uitilityDfs.h"
#include "uimodule_glWidget.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPixmap>

XA_UIMODULE_WindowInfo::XA_UIMODULE_WindowInfo(QWidget* parent, const QSize& initsize, const QString& source)
	:_crtSoucesize(initsize),_crtWindowsize(initsize),_source(source)
{
	//this->setParent(parent);
	this->setStyleSheet(WINDOW_FONT_COLOR_STYLE);

	QHBoxLayout *hlay = new QHBoxLayout(this);
	crtSource_show = new QLabel(this);
	crtSource_info = new QLabel(this);
	crtSize_show = new QLabel(this);
	crtSize_info = new QLabel(this);
	lockbtn = new QPushButton(this);
	resetbtn = new QPushButton(this);
	recbtn = new QPushButton(this);
	recbtn->setIconSize(QSize(20, 20));
	soundbtn = new QPushButton(this);
	soundbtn->setIconSize(QSize(20, 20));
	QLabel *leftLine = new QLabel(this);
	QLabel *rightLine = new QLabel(this);

	QString source_show = QString("%1 %2x%3").arg(_source).arg(_crtSoucesize.width()).arg(_crtSoucesize.height());
	crtSource_show->setText(source_show);
	QString crtsize_info = _STRING_WRAPPER("当前大小: %1x%2").arg(_crtWindowsize.width()).arg(_crtWindowsize.height());
	crtSize_info->setText(crtsize_info);

	QPixmap vertical_pix = QPixmap(ICOPATH(vertical.svg));
	leftLine->setPixmap(vertical_pix);
	leftLine->setFixedWidth(16);
	rightLine->setPixmap(vertical_pix);
	rightLine->setFixedWidth(16);

	lockbtn->setIconSize(QSize(16, 16));
	lockbtn->setIcon(QIcon(ICOPATH(locksize.svg)));
	lockbtn->setText(_STRING_WRAPPER("锁定当前宽高"));
	lockbtn->setStyleSheet(WINDOWINFO_BTN_STYLE_1);

	resetbtn->setIconSize(QSize(16, 16));
	resetbtn->setIcon(QIcon(ICOPATH(reset.svg)));
	resetbtn->setText(_STRING_WRAPPER("重置大小"));
	resetbtn->setStyleSheet(WINDOWINFO_BTN_STYLE_1);

	recbtn->setFixedWidth(30);
	recbtn->setIcon(QIcon(ICOPATH(record.svg)));
	recbtn->setStyleSheet(WINDOWINFO_BTN_STYLE_2);
	connect(recbtn, &QPushButton::clicked, this, &XA_UIMODULE_WindowInfo::on_clcRecord);

	soundbtn->setFixedWidth(30);
	soundbtn->setIcon(QIcon(ICOPATH(soundon.svg)));
	soundbtn->setStyleSheet(WINDOWINFO_BTN_STYLE_2);
	connect(soundbtn, &QPushButton::clicked, this, &XA_UIMODULE_WindowInfo::on_clcSoundOff);

	hlay->addWidget(crtSource_show);
	hlay->addSpacing(2);
	hlay->addWidget(crtSource_info);
	hlay->addSpacing(10);
	hlay->addWidget(crtSize_show);
	hlay->addSpacing(2);
	hlay->addWidget(crtSize_info);
	hlay->addSpacing(10);
	hlay->addWidget(leftLine);
	hlay->addWidget(lockbtn);
	hlay->addSpacing(10);
	hlay->addWidget(resetbtn);
	hlay->addWidget(rightLine);
	hlay->addWidget(recbtn);
	hlay->addSpacing(10);
	hlay->addWidget(soundbtn);
	hlay->setContentsMargins(20, 0, 20, 0);
	this->setLayout(hlay);

}

void XA_UIMODULE_WindowInfo::updateInfo(const QSize& newsize)
{
	_crtWindowsize = newsize;
	QString crtsize_info = _STRING_WRAPPER("当前大小: %1x%2").arg(_crtWindowsize.width()).arg(_crtWindowsize.height());
	crtSize_info->setText(crtsize_info);
}

void XA_UIMODULE_WindowInfo::on_clcRecord()
{
	disconnect(recbtn, &QPushButton::clicked, this, &XA_UIMODULE_WindowInfo::on_clcRecord);
	recbtn->setIcon(QIcon(ICOPATH(recording.svg)));
	if (glWindow_Inst)
	{
		glWindow_Inst->__startRecord();
	}
	connect(recbtn, &QPushButton::clicked, this, &XA_UIMODULE_WindowInfo::on_clcEndRecord);

}

void XA_UIMODULE_WindowInfo::on_clcEndRecord()
{
	disconnect(recbtn, &QPushButton::clicked, this, &XA_UIMODULE_WindowInfo::on_clcEndRecord);
	recbtn->setIcon(QIcon(ICOPATH(record.svg)));
	if (glWindow_Inst)
	{
		glWindow_Inst->__endRecord();
	}
	connect(recbtn, &QPushButton::clicked, this, &XA_UIMODULE_WindowInfo::on_clcRecord);
}

void XA_UIMODULE_WindowInfo::on_clcSoundOn()
{
	disconnect(soundbtn, &QPushButton::clicked, this, &XA_UIMODULE_WindowInfo::on_clcSoundOn);
	soundbtn->setIcon(QIcon(ICOPATH(soundon.svg)));
	connect(soundbtn, &QPushButton::clicked, this, &XA_UIMODULE_WindowInfo::on_clcSoundOff);
}

void XA_UIMODULE_WindowInfo::on_clcSoundOff()
{
	disconnect(soundbtn, &QPushButton::clicked, this, &XA_UIMODULE_WindowInfo::on_clcSoundOff);
	soundbtn->setIcon(QIcon(ICOPATH(soundoff.svg)));
	connect(soundbtn, &QPushButton::clicked, this, &XA_UIMODULE_WindowInfo::on_clcSoundOn);
}

