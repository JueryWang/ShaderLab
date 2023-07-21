#include "MenuLayertop.h"
#include "UI/uimodule_aboutmenu.h"
#include "UI/uimodule_filemenu.h"
#include "UI/uimodule_lookmenu.h"
#include "UI/uimodule_windowmenu.h"
#include "UI/uimodule_profilemenu.h"
#include "UI/uimodule_customIconStyle.h"
#include "Utilitys/uitilityDfs.h"
#include <QSizePolicy>

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
	XA_UIModule_ProfileMenu* profile_menu = new XA_UIModule_ProfileMenu(_STRING_WRAPPER("个人"));

	about_menu->setObjectName("About&Update");
	about_menu->setMinimumWidth(135);
	about_menu->setProperty("class", "blackMenu");
	about_menu->setWindowFlags(about_menu->windowFlags() | Qt::FramelessWindowHint);
	about_menu->setAttribute(Qt::WA_TranslucentBackground);
	

	file_menu->setObjectName("FILE");
	file_menu->setMinimumWidth(135);
	file_menu->setProperty("class", "blackMenu");
	file_menu->setWindowFlags(file_menu->windowFlags() | Qt::FramelessWindowHint);
	file_menu->setAttribute(Qt::WA_TranslucentBackground);

	look_menu->setObjectName("LOOK");
	look_menu->setMinimumWidth(135);
	look_menu->setProperty("class", "blackMenu");
	look_menu->setWindowFlags(look_menu->windowFlags() | Qt::FramelessWindowHint);
	look_menu->setAttribute(Qt::WA_TranslucentBackground);

	window_menu->setObjectName("WINDOW");
	window_menu->setMinimumWidth(135);
	window_menu->setProperty("class", "blackMenu");
	window_menu->setWindowFlags(window_menu->windowFlags() | Qt::FramelessWindowHint);
	window_menu->setAttribute(Qt::WA_TranslucentBackground);

	_topMenus->setFont(QFont("Microsoft YaHei", 10,QFont::Bold));
	_topMenus->addMenu(about_menu);
	_topMenus->addMenu(file_menu);
	_topMenus->addMenu(look_menu);
	_topMenus->addMenu(window_menu);
	_topMenus->addSeparator();
	_topMenus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	_layout->addWidget(_topMenus, 0, Qt::AlignLeft | Qt::AlignTop);
	_layout->addWidget(profile_menu, 0, Qt::AlignTop | Qt::AlignRight);
}

MenuLayertop::~MenuLayertop()
{
	delete _topMenus;
}

QMenuBar* MenuLayertop::GetMenus()
{
	return this->_topMenus;
}
