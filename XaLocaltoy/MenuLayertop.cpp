#include "MenuLayertop.h"
#include "UI/uimodule_customIconStyle.h"

MenuLayertop::MenuLayertop(QMenuBar* parent /*= NULL*/)
{
	CustomIconStyle* style = new CustomIconStyle();
	this->setStyle(style);
	QMenu *about_menu = new XA_UIModule_AboutMenu("About&Update",true,"Resources/icon/ShaderLabIco.png");

	about_menu->setObjectName("About&Update");
	about_menu->setProperty("class", "blackMenu");
	about_menu->setWindowFlags(about_menu->windowFlags() | Qt::FramelessWindowHint);
	about_menu->setAttribute(Qt::WA_TranslucentBackground);
	menu_list.append(about_menu);

	this->setFont(QFont("Microsoft YaHei", 12, QFont::Bold));
	this->addMenu(about_menu);
	this->addSeparator();
}
