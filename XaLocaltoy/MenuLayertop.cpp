#include "MenuLayertop.h"
#include "Utilitys/uitilityDfs.h"
#include "UI/uimodule_aboutmenu.h"
#include "UI/uimodule_filemenu.h"
#include "UI/uimodule_lookmenu.h"
#include "UI/uimodule_windowmenu.h"
#include "UI/uimodule_profilemenu.h"
#include "UI/uimodule_customIconStyle.h"
#include "UI/uimodule_MsgBox.h"
#include "OverallWindow.h"
#include <QMouseEvent>
#include <QSizePolicy>
#include <QToolButton>
#include <QCoreApplication>

MenuLayertop::MenuLayertop(OverallWindow* parent /*= NULL*/)
{
	this->setParent(parent);
	_parentInst = parent;
	_topMenus = new QMenuBar(this);
	_layout = new QHBoxLayout();

	CustomIconStyle* style = new CustomIconStyle(24);
	this->setStyle(style);
	XA_UIModule_AboutMenu*about_menu = new XA_UIModule_AboutMenu("About&Update",true,ICOPATH(ShaderLabIco.png));
	XA_UIModule_FILEMenu* file_menu = new XA_UIModule_FILEMenu(_STRING_WRAPPER("文件"));
	XA_UIModule_LOOKMenu* look_menu = new XA_UIModule_LOOKMenu(_STRING_WRAPPER("查看"));
	XA_UIModule_WINDOWMenu* window_menu = new XA_UIModule_WINDOWMenu(_STRING_WRAPPER("窗口"));
	XA_UIModule_ProfileMenu* profile_menu = new XA_UIModule_ProfileMenu(_STRING_WRAPPER("Xaiver"));

	about_menu->setObjectName("ABOUT&UPDATE");
	about_menu->setMinimumWidth(135);

	file_menu->setObjectName("FILE");
	file_menu->setMinimumWidth(135);

	look_menu->setObjectName("LOOK");
	look_menu->setMinimumWidth(135);

	window_menu->setObjectName("WINDOW");
	window_menu->setMinimumWidth(135);

	profile_menu->setObjectName("PROFILE");
	profile_menu->setMinimumWidth(100);

	_topMenus->setFont(QFont("Microsoft YaHei", 10,QFont::Bold));
	_topMenus->addMenu(about_menu);
	_topMenus->addMenu(file_menu);
	_topMenus->addMenu(look_menu);
	_topMenus->addMenu(window_menu);
	_topMenus->addSeparator();
	_topMenus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


	QToolButton* closeBtn = new QToolButton(this);
	closeBtn->setFixedSize(30, 30);
	closeBtn->setStyleSheet(".QToolButton{background-color:transparent;border:1px solid rgba(255,255,255,0);\
								qproperty-icon: url(Resources/icon/close.svg);qproperty-iconSize: 20px 20px;}\
				 .QToolButton:hover,pressed,selected{padding:0px 0px;background-color:rgba(226, 46, 39, 1.0)}");
	closeBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	connect(closeBtn, &QToolButton::clicked, this, &MenuLayertop::on_clcClose);
	
	OverallWindow* owWindow = (OverallWindow*)parent;
	QToolButton* fullBtn = new QToolButton(this);
	fullBtn->setObjectName("zoom Button");
	fullBtn->setFixedSize(30, 30);
	fullBtn->setStyleSheet(".QToolButton{background-color:transparent;border:1px solid rgba(255,255,255,0);\
							qproperty-icon: url(Resources/icon/fullscreen.svg);qproperty-iconSize: 20px 20px;}\
				 .QToolButton:hover,pressed,selected{padding:0px 0px;background-color:rgba(61, 68, 80, 1.0)}");
	fullBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	connect(fullBtn, &QToolButton::clicked, owWindow, &OverallWindow::setFullScreen);

	QToolButton* minimizeBtn = new QToolButton(this);
	minimizeBtn->setFixedSize(30, 30);
	minimizeBtn->setStyleSheet(".QToolButton{background-color:transparent;border:1px solid rgba(255,255,255,0);\
							qproperty-icon: url(Resources/icon/minimize.svg);qproperty-iconSize: 20px 20px;}\
				 .QToolButton:hover,pressed,selected{padding:0px 0px;background-color:rgba(61, 68, 80, 1.0)}");
	minimizeBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	connect(minimizeBtn, &QToolButton::clicked, owWindow, &OverallWindow::setMinimum);

	_layout->addWidget(_topMenus, 0, Qt::AlignLeft | Qt::AlignTop);
	_layout->addWidget(profile_menu, 1, Qt::AlignTop | Qt::AlignRight);
	_layout->addWidget(minimizeBtn, 0, Qt::AlignTop | Qt::AlignRight);
	_layout->addWidget(fullBtn, 0, Qt::AlignTop | Qt::AlignRight);
	_layout->addWidget(closeBtn, 0, Qt::AlignTop | Qt::AlignRight);
	_layout->setContentsMargins(6, 0, 0, 4);
	this->setLayout(_layout);
	this->installEventFilter(this);
}

MenuLayertop::~MenuLayertop()
{
	delete _topMenus;
}

bool MenuLayertop::eventFilter(QObject* obj, QEvent* event)
{
	switch (event->type())
	{
	case QEvent::MouseButtonPress:
	{   auto e = static_cast<QMouseEvent*>(event);
	if (e->button() == Qt::LeftButton)
	{
		_isDrag = true;
		_offsetPoint = e->globalPos() - _parentInst->frameGeometry().topLeft();
	}
	event->accept();
	return true;
	}
	case QEvent::MouseMove:
	{
		auto e = static_cast<QMouseEvent*>(event);
		if (_isDrag)
		{
			_parentInst->move(e->globalPos() - _offsetPoint);
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
	}
}

void MenuLayertop::on_Exit_Msg(int val)
{
	switch (val)
	{
		case 0:
			//do Save operation
			QCoreApplication::exit();
			break;
		case 1:
			QCoreApplication::exit();
			break;
		case 2:
			break;
	}
}

void MenuLayertop::on_clcClose()
{
	OverallWindow* owWindow = (OverallWindow*)this->parent();
	if((xa_context & RT_CONTEXT_STATUS::Archive_updated) == 1)
	{
		QCoreApplication::exit();
	}
	else
	{
		XA_UIModule_QUEST_BOX* instance = XA_UIModule_QUEST_BOX::question(this, "Save changes before closing?", "untitled.sdl",owWindow->size());
		connect(instance, &XA_UIModule_QUEST_BOX::sendChoose, this, &MenuLayertop::on_Exit_Msg);
	}
}
