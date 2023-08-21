#include "uimodule_MsgBox.h"
#include "ui_defaultDfs.h"
#include <QMouseEvent>
#include <QPushButton>
#include <QIcon>

XA_UIModule_QUEST_BOX::XA_UIModule_QUEST_BOX(QWidget* parent, const QString& mainMsg, const QString& attachedMsg)
{
	_wgt = new QWidget(this);
	_wgt->setFixedSize(467, 117);
	_wgt->setProperty("class", "blackWidget");

	this->setAttribute(Qt::WA_TranslucentBackground, true);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog);
	this->setWindowModality(Qt::WindowModal);
	this->setVisible(true);
	_wgt->installEventFilter(this);

	_vlay = new QVBoxLayout();
	_hlay = new QHBoxLayout();
	_btnsLay = new QHBoxLayout();
	_icon = new QLabel();
	_icon->setFixedSize(64, 64);

	_hlay->addWidget(_icon);
	_hlay->addSpacing(20);
	QFont textFont("Microsoft YaHei", 10);
	QVBoxLayout* discripLayout = new QVBoxLayout();
	QLabel* maindisp = new QLabel();
	maindisp->setStyleSheet(".QLabel{"
		"color:#E6E6E6;"
		"}");
	maindisp->setFont(textFont);
	maindisp->setText(mainMsg);
	discripLayout->addWidget(maindisp);
	QLabel* attachedDisp = new QLabel();
	attachedDisp->setStyleSheet(".QLabel{"
		"color:#979797;"
		"}");
	attachedDisp->setText(attachedMsg);
	attachedDisp->setFont(textFont);
	discripLayout->addWidget(attachedDisp);
	discripLayout->addSpacing(50);
	_hlay->addLayout(discripLayout);
	_vlay->addLayout(_hlay);

	//Ô¤Áô3¸öButtonÎ»
	QPushButton* btnSave = new QPushButton();
	connect(btnSave, &QPushButton::clicked, this, &XA_UIModule_QUEST_BOX::on_clcSave);
	QPushButton* btnUnSave = new QPushButton();
	connect(btnUnSave, &QPushButton::clicked, this, &XA_UIModule_QUEST_BOX::on_clcUnsave);
	QPushButton* btnCancel = new QPushButton();
	connect(btnCancel, &QPushButton::clicked, this, &XA_UIModule_QUEST_BOX::on_clcCancel);

	QFont btnFont("Arial Black", 10, 75);
	btnSave->setStyleSheet(".QPushButton{"
		"background-color: rgba(71, 114, 179,200);border-radius: 5px;color:rgba(225, 225, 225,200);"
		"} .QPushButton:hover,pressed,selected{background-color:rgba(98, 139, 202,200);}");
	btnSave->setText("Save");
	btnSave->setFont(btnFont);
	btnUnSave->setStyleSheet(".QPushButton{"
		"background-color: rgba(101, 101, 101,200);border-radius: 5px;color:rgba(225, 225, 225,200);"
		"} .QPushButton:hover,pressed,selected{background-color:rgba(98, 139, 202,200);}");
	btnUnSave->setText("Don't Save");
	btnUnSave->setFont(btnFont);
	btnCancel->setStyleSheet(".QPushButton{"
		"background-color: rgba(101, 101, 101,200);border-radius: 5px;color:rgba(225, 225, 225,200);"
		"} .QPushButton:hover,pressed,selected{background-color:rgba(98, 139, 202,200);}");
	btnCancel->setText("Cancel");
	btnCancel->setFont(btnFont);
	_btnsLay->setSpacing(10);
	_btnsLay->addSpacing(80);
	_btnsLay->addWidget(btnSave);
	_btnsLay->addWidget(btnUnSave);
	_btnsLay->addWidget(btnCancel);

	_vlay->addLayout(_btnsLay);
	_wgt->setLayout(_vlay);
}

XA_UIModule_QUEST_BOX::~XA_UIModule_QUEST_BOX()
{
	delete _wgt;
}

XA_UIModule_QUEST_BOX* XA_UIModule_QUEST_BOX::question(QWidget* parent, const QString& title, const QString& text, const QSize& windowSz)
{
	static XA_UIModule_QUEST_BOX* questMsg = new XA_UIModule_QUEST_BOX(parent, title, text);
	QPixmap pix(ICOPATH(quest.svg));
	questMsg->_icon->setPixmap(pix);
	questMsg->move(parent->mapToGlobal(QPoint(windowSz.width() / 2, windowSz.height() / 2)));
	questMsg->show();
	questMsg->_wgt->activateWindow();
	questMsg->_wgt->setFocus();
	questMsg->setAttribute(Qt::WA_DeleteOnClose, true);
	return questMsg;
}

void XA_UIModule_QUEST_BOX::on_clcSave()
{
	sendChoose(0);
	this->hide();
}

void XA_UIModule_QUEST_BOX::on_clcUnsave()
{
	sendChoose(1);
	this->hide();
}

void XA_UIModule_QUEST_BOX::on_clcCancel()
{
	sendChoose(2);
	this->hide();
}

bool XA_UIModule_QUEST_BOX::eventFilter(QObject* obj, QEvent* event)
{
	switch (event->type())
	{
	case QEvent::MouseButtonPress:
	{   auto e = static_cast<QMouseEvent*>(event);
	if (e->button() == Qt::LeftButton)
	{
		_isDrag = true;

		_offsetPoint = e->globalPos() - this->frameGeometry().topLeft();
	}
	event->accept();
	return true;
	}
	case QEvent::MouseMove:
	{
		auto e = static_cast<QMouseEvent*>(event);
		if (_isDrag)
		{
			this->move(e->globalPos() - _offsetPoint);
		}
		event->accept();
		return true;
	}
	case QEvent::MouseButtonRelease:
	{
		auto e = static_cast<QMouseEvent*>(event);
		if (e->button() == Qt::LeftButton)
		{
			_isDrag = false;
		}
		return true;
	}
	case QEvent::FocusOut:
	{
		this->hide();
	}
	}
}

