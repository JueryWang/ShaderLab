#include "uimodule_profilemenu.h"
#include "uimodule_customIconStyle.h"
#include "../Utilitys/uitilityDfs.h"
#include <QMenu>
#include <QAction>

XA_UIModule_ProfileMenu::XA_UIModule_ProfileMenu(QString title)
{

	//setProperty("class", "blackMenu");
	//setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
	//CustomIconStyle* style = new CustomIconStyle(24);
	//
	//QMenu* profileMenu = new QMenu(_STRING_WRAPPER("����"));
	//profileMenu->setStyle(style);
	//QAction* myprofileAct = new QAction(_STRING_WRAPPER("��������"));
	//myprofileAct->setIcon(QIcon(ICOPATH(profile.svg)));
	//connect(myprofileAct, &QAction::triggered, this, &XA_UIModule_ProfileMenu::on_clcProfile);
	//profileMenu->addAction(myprofileAct);
	//QAction* myDvcAct = new QAction(_STRING_WRAPPER("�ҵ��豸"));
	//myDvcAct->setIcon(QIcon(ICOPATH(mydevice.svg)));
	//connect(myDvcAct, &QAction::triggered, this, &XA_UIModule_ProfileMenu::on_clcProfile);
	//profileMenu->addAction(myDvcAct);
	//QAction* myShaderAct = new QAction(_STRING_WRAPPER("�ҵ�Shader"));
	//myDvcAct->setIcon(QIcon(ICOPATH(unrealengine.svg)));
	//connect(myShaderAct, &QAction::triggered, this, &XA_UIModule_ProfileMenu::on_clcCheckShader);
	//profileMenu->addAction(myShaderAct);
	//QAction* switchAct = new QAction(_STRING_WRAPPER("�����˻�"));
	//connect(switchAct, &QAction::triggered, this, &XA_UIModule_ProfileMenu::on_clcSwitchAccount);
	//profileMenu->addAction(switchAct);
	//QAction* exitAct = new QAction(_STRING_WRAPPER("�˳��˻�"));
	//connect(exitAct, &QAction::triggered, this, &XA_UIModule_ProfileMenu::on_clcExitLoggin);
	//profileMenu->addAction(exitAct);
	//
	//this->setAttribute(Qt::WA_TranslucentBackground);
	//this->setPopupMode(QToolButton::InstantPopup);
	//this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	//this->setText(title);
	//this->setMenu(profileMenu);
	//this->setStyleSheet(".QToolButton{color: #b9b9b9;\
 //                                 font-size: 12px;\
 //                                 font-family:\"Arial Black\";\
 //                                 font-weight: normal;\
 //                                 color:rgba(120, 170, 218,0.85);\
 //                                 border: 2px solid #b9b9b9;\
 //                                 border-radius: 3px;\
 //                                 qproperty-icon: url(icons/avatar.png) left no-repeat;\
 //                                 qproperty-iconSize: 30px 30px;\
 //                                 padding-left: 5px;\
 //                                   }");
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

