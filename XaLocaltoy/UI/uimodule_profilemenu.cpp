#include "uimodule_profilemenu.h"
#include "uimodule_customIconStyle.h"
#include "../Utilitys/uitilityDfs.h"
#include <QMenu>
#include <QAction>

XA_UIModule_ProfileMenu::XA_UIModule_ProfileMenu(QString title)
{

	CustomIconStyle* style = new CustomIconStyle(18);

	QMenu* profileMenu = new QMenu(_STRING_WRAPPER("个人"));
	profileMenu->setMinimumWidth(135);
	profileMenu->setStyle(style);
	profileMenu->setProperty("class", "blackMenu");
	QAction* myprofileAct = new QAction(_STRING_WRAPPER("个人资料"));
	myprofileAct->setIcon(QIcon(ICOPATH(profile.svg)));
	connect(myprofileAct, &QAction::triggered, this, &XA_UIModule_ProfileMenu::on_clcProfile);
	profileMenu->addAction(myprofileAct);
	QAction* myDvcAct = new QAction(_STRING_WRAPPER("我的设备"));
	myDvcAct->setIcon(QIcon(ICOPATH(mydevice.svg)));
	connect(myDvcAct, &QAction::triggered, this, &XA_UIModule_ProfileMenu::on_clcProfile);
	profileMenu->addAction(myDvcAct);
	QAction* myShaderAct = new QAction(_STRING_WRAPPER("我的Shader"));
	myShaderAct->setIcon(QIcon(ICOPATH(unrealengine.svg)));
	connect(myShaderAct, &QAction::triggered, this, &XA_UIModule_ProfileMenu::on_clcCheckShader);
	profileMenu->addAction(myShaderAct);
	QAction* switchAct = new QAction(_STRING_WRAPPER("更换账户"));
	connect(switchAct, &QAction::triggered, this, &XA_UIModule_ProfileMenu::on_clcSwitchAccount);
	profileMenu->addAction(switchAct);
	QAction* exitAct = new QAction(_STRING_WRAPPER("退出账户"));
	connect(exitAct, &QAction::triggered, this, &XA_UIModule_ProfileMenu::on_clcExitLoggin);
	profileMenu->addAction(exitAct);

	this->setPopupMode(QToolButton::InstantPopup);
	this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	this->setText(title);
	this->setMenu(profileMenu);
	this->setStyleSheet(".QToolButton{color: #b9b9b9;\
                                  font-size: 12px;\
                                  font-family:\"Arial Black\";\
                                  font-weight: normal;\
                                  color:rgba(120, 170, 218,0.85);\
                                  border: 2px solid #5D909F;\
                                  border-radius: 3px;\
                                  qproperty-icon: url(Resources/user/avatar/avatar.png) left no-repeat;\
                                  qproperty-iconSize: 26px 26px;\
                                  padding-left: 5px;\
                                  }");
}

XA_UIModule_ProfileMenu::~XA_UIModule_ProfileMenu()
{

}

void XA_UIModule_ProfileMenu::on_clcProfile()
{

}

void XA_UIModule_ProfileMenu::on_clcCheckDevice()
{

}

void XA_UIModule_ProfileMenu::on_clcCheckShader()
{

}

void XA_UIModule_ProfileMenu::on_clcSwitchAccount()
{

}

void XA_UIModule_ProfileMenu::on_clcExitLoggin()
{

}
