#include "ui_defaultDfs.h"
#include "uimodule_windowInfo.h"
#include "../Utilitys/uitilityDfs.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPixmap>

XA_UIMODULE_WindowInfo::XA_UIMODULE_WindowInfo(QWidget* parent, const QSize& initsize, const QString& source)
	:_crtSoucesize(initsize),_crtWindowsize(initsize),_source(source)
{
	this->setParent(parent);
	this->setStyleSheet("QLabel{background:transparent;color:#eeeed4;}");

	QHBoxLayout *hlay = new QHBoxLayout(this);
	QLabel *crtSource_show = new QLabel(this);
	QLabel *crtSource_info = new QLabel(this);
	QLabel *crtSize_show = new QLabel(this);
	QLabel *crtSize_info = new QLabel(this);
	QLabel *leftLine = new QLabel(this);
	QPushButton *lockbtn = new QPushButton(this);
	QPushButton *resetbtn = new QPushButton(this);
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
	lockbtn->setStyleSheet(WINDOWINFO_BTN_STYLE);

	resetbtn->setIconSize(QSize(16, 16));
	resetbtn->setIcon(QIcon(ICOPATH(reset.svg)));
	resetbtn->setText(_STRING_WRAPPER("重置大小"));
	resetbtn->setStyleSheet(WINDOWINFO_BTN_STYLE);

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
	hlay->setContentsMargins(20, 0, 20, 0);
	this->setLayout(hlay);
}

