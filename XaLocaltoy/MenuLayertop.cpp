#include "MenuLayertop.h"
#include "UI/uimodule_aboutmenu.h"
#include "UI/uimodule_filemenu.h"
#include "UI/uimodule_lookmenu.h"
#include "UI/uimodule_customIconStyle.h"
#include "Utilitys/uitilityDfs.h"

MenuLayertop::MenuLayertop(QMenuBar* parent /*= NULL*/)
{
	CustomIconStyle* style = new CustomIconStyle(24);
	this->setStyle(style);
	QMenu *about_menu = new XA_UIModule_AboutMenu("About&Update",true,ICOPATH(ShaderLabIco.png));
	QMenu* file_menu = new XA_UIModule_FILEMenu(_STRING_WRAPPER("文件"));
	QMenu* look_menu = new XA_UIModule_LOOKMenu(_STRING_WRAPPER("查看"));

	about_menu->setObjectName("About&Update");
	about_menu->setProperty("class", "blackMenu");
	about_menu->setWindowFlags(about_menu->windowFlags() | Qt::FramelessWindowHint);
	about_menu->setAttribute(Qt::WA_TranslucentBackground);
	

	file_menu->setObjectName("FILE");
	file_menu->setProperty("class", "blackMenu");
	file_menu->setWindowFlags(file_menu->windowFlags() | Qt::FramelessWindowHint);
	file_menu->setAttribute(Qt::WA_TranslucentBackground);

	look_menu->setObjectName("LOOK");
	look_menu->setMinimumWidth(135);
	look_menu->setProperty("class", "blackMenu");
	look_menu->setWindowFlags(look_menu->windowFlags() | Qt::FramelessWindowHint);
	look_menu->setAttribute(Qt::WA_TranslucentBackground);

	this->setFont(QFont("Microsoft YaHei", 10,QFont::Bold));
	this->addMenu(about_menu);
	this->addMenu(file_menu);
	this->addMenu(look_menu);
	this->addSeparator();
}
