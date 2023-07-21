#include "MenuLayertop.h"
#include "UI/uimodule_aboutmenu.h"
#include "UI/uimodule_filemenu.h"
#include "UI/uimodule_lookmenu.h"
#include "UI/uimodule_windowmenu.h"
#include "UI/uimodule_profilemenu.h"
#include "UI/uimodule_customIconStyle.h"
#include "Utilitys/uitilityDfs.h"
#include <QSizePolicy>
#include <QToolButton>

MenuLayertop::MenuLayertop(QMenuBar* parent /*= NULL*/)
{
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


	QToolButton* closeBtn = new QToolButton();
	closeBtn->setFixedSize(30, 30);
	closeBtn->setStyleSheet(".QToolButton{background-color:transparent;border:1px solid rgba(255,255,255,0);\
								qproperty-icon: url(Resources/icon/close.svg);qproperty-iconSize: 20px 20px;}\
				 .QToolButton:hover,pressed,selected{padding:0px 0px;background-color:rgba(226, 46, 39, 1.0)}");
	closeBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	connect(closeBtn, &QToolButton::triggered, this, &MenuLayertop::on_clcClose);
	
	QToolButton* fullBtn = new QToolButton();
	fullBtn->setFixedSize(30, 30);
	fullBtn->setStyleSheet(".QToolButton{background-color:transparent;border:1px solid rgba(255,255,255,0);\
							qproperty-icon: url(Resources/icon/fullscreen.svg);qproperty-iconSize: 20px 20px;}\
				 .QToolButton:hover,pressed,selected{padding:0px 0px;background-color:rgba(226, 46, 39, 1.0)}");
	fullBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	connect(closeBtn, &QToolButton::triggered, this, &MenuLayertop::on_clcFullScreen);

	QToolButton* minimizeBtn = new QToolButton();
	minimizeBtn->setFixedSize(30, 30);
	minimizeBtn->setStyleSheet(".QToolButton{background-color:transparent;border:1px solid rgba(255,255,255,0);\
							qproperty-icon: url(Resources/icon/minimize.svg);qproperty-iconSize: 20px 20px;}\
				 .QToolButton:hover,pressed,selected{padding:0px 0px;background-color:rgba(226, 46, 39, 1.0)}");
	minimizeBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	connect(minimizeBtn, &QToolButton::triggered, this, &MenuLayertop::on_clcMinimize);

	_layout->addWidget(_topMenus, 0, Qt::AlignLeft | Qt::AlignTop);
	_layout->addWidget(profile_menu, 1, Qt::AlignTop | Qt::AlignRight);
	_layout->addWidget(minimizeBtn, 0, Qt::AlignTop | Qt::AlignRight);
	_layout->addWidget(fullBtn, 0, Qt::AlignTop | Qt::AlignRight);
	_layout->addWidget(closeBtn, 0, Qt::AlignTop | Qt::AlignRight);
	_layout->setContentsMargins(6, 0, 0, 4);
	this->setLayout(_layout);
}

MenuLayertop::~MenuLayertop()
{
	delete _topMenus;
}

void MenuLayertop::on_clcClose()
{

}

void MenuLayertop::on_clcFullScreen()
{

}

void MenuLayertop::on_clcMinimize()
{

}
