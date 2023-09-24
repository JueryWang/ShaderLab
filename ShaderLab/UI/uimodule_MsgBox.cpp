#include "uimodule_MsgBox.h"
#include "ui_defaultDfs.h"
#include <QMouseEvent>
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

	QVBoxLayout* _vlay = new QVBoxLayout();
	QHBoxLayout* _hlay = new QHBoxLayout();
	QHBoxLayout* _btnsLay = new QHBoxLayout();
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
	maindisp->setFixedHeight(30);
	maindisp->setWordWrap(true);
	maindisp->setText(mainMsg);
	discripLayout->addWidget(maindisp);
	QLabel* attachedDisp = new QLabel();
	attachedDisp->setStyleSheet(".QLabel{"
		"color:#979797;"
		"}");
	attachedDisp->setFont(textFont);
	attachedDisp->setWordWrap(true);
	attachedDisp->setText(attachedMsg);
	discripLayout->addWidget(attachedDisp);
	discripLayout->addSpacing(50);
	_hlay->addLayout(discripLayout);
	_vlay->addLayout(_hlay);

	//Ô¤Áô3¸öButtonÎ»
	btn1 = new QPushButton();
	connect(btn1, &QPushButton::clicked, this, &XA_UIModule_QUEST_BOX::on_clcBtn1);
	btn2 = new QPushButton();
	connect(btn2, &QPushButton::clicked, this, &XA_UIModule_QUEST_BOX::on_clcBtn2);
	btn3 = new QPushButton();
	connect(btn3, &QPushButton::clicked, this, &XA_UIModule_QUEST_BOX::on_clcBtn3);

	QFont btnFont("Arial Black", 10, 75);
	btn1->setStyleSheet(".QPushButton{"
		"background-color: rgba(71, 114, 179,200);border-radius: 5px;color:rgba(225, 225, 225,200);"
		"} .QPushButton:hover,pressed,selected{background-color:rgba(98, 139, 202,200);}");
	btn1->setFont(btnFont);
	btn2->setStyleSheet(".QPushButton{"
		"background-color: rgba(101, 101, 101,200);border-radius: 5px;color:rgba(225, 225, 225,200);"
		"} .QPushButton:hover,pressed,selected{background-color:rgba(98, 139, 202,200);}");
	btn2->setFont(btnFont);
	btn3->setStyleSheet(".QPushButton{"
		"background-color: rgba(101, 101, 101,200);border-radius: 5px;color:rgba(225, 225, 225,200);"
		"} .QPushButton:hover,pressed,selected{background-color:rgba(98, 139, 202,200);}");
	btn3->setFont(btnFont);
	_btnsLay->setSpacing(10);
	_btnsLay->addSpacing(80);
	_btnsLay->addWidget(btn1);
	_btnsLay->addWidget(btn2);
	_btnsLay->addWidget(btn3);

	_vlay->addLayout(_btnsLay);
	_wgt->setLayout(_vlay);
}

XA_UIModule_QUEST_BOX::~XA_UIModule_QUEST_BOX()
{
	delete _wgt;
}

XA_UIModule_QUEST_BOX* XA_UIModule_QUEST_BOX::question(QWidget* parent, const QString& title, const QString& text, const QSize& windowSz,
	const QStringList& btnTexts, const std::vector<std::function<void(void)>>& callbacks)
{
	static XA_UIModule_QUEST_BOX* questMsg = new XA_UIModule_QUEST_BOX(parent, title, text);
	questMsg->setButtonTexts(btnTexts);
	questMsg->setCallbacks(callbacks);
	QPixmap pix(ICOPATH(quest.svg));
	questMsg->_icon->setPixmap(pix);
	questMsg->move(parent->mapToGlobal(QPoint(windowSz.width() / 2, windowSz.height() / 2)));
	questMsg->show();
	questMsg->_wgt->activateWindow();
	questMsg->_wgt->setFocus();
	questMsg->setAttribute(Qt::WA_DeleteOnClose, true);
	return questMsg;
}

void XA_UIModule_QUEST_BOX::setButtonTexts(const QStringList& texts)
{
	if (texts[0] != "") { btn1->setText(texts[0]); btn1->show(); }
	else btn1->hide();

	if (texts[1] != "") { btn2->setText(texts[1]); btn2->show(); }
	else btn2->hide();

	if (texts[2] != "") { btn3->setText(texts[2]); btn3->show(); }
	else btn3->hide();
}

void XA_UIModule_QUEST_BOX::setCallbacks(const std::vector<std::function<void(void)>>& callbacks)
{
	if (callbacks[0] != nullptr) cb1 = callbacks[0];
	if (callbacks[1] != nullptr) cb2 = callbacks[1];
	if (callbacks[2] != nullptr) cb3 = callbacks[2];
}

void XA_UIModule_QUEST_BOX::on_clcBtn1()
{
	if (cb1) 
		cb1();
	this->hide();
}

void XA_UIModule_QUEST_BOX::on_clcBtn2()
{
	if (cb2)
		cb2();
	this->hide();
}

void XA_UIModule_QUEST_BOX::on_clcBtn3()
{
	if (cb3)
		cb3();
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

