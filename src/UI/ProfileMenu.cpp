#include "ProfileMenu.h"
#include "UI_DefaultDfs.h"
#include "CustomIconStyle.h"
#include <QMenu>

using namespace SDL_UI_MODULE;

SDL_UI_MODULE::ProfileMenu::ProfileMenu(const QString &title) {

    QMenu* profileMenu = new QMenu(tr("personal"));
    profileMenu->setMinimumWidth(135);
    profileMenu->setProperty("class", "blackMenu");

    QAction* myprofileAct = new QAction(tr("profile"));
    myprofileAct->setIcon(QIcon(ICOPATH(profile.svg)));
    connect(myprofileAct, &QAction::triggered, this, &ProfileMenu::on_clcProfile);
    profileMenu->addAction(myprofileAct);
    QAction* myDvcAct = new QAction(tr("my device"));
    myDvcAct->setIcon(QIcon(ICOPATH(mydevice.svg)));
    connect(myDvcAct, &QAction::triggered, this, &ProfileMenu::on_clcProfile);
    profileMenu->addAction(myDvcAct);
    QAction* myShaderAct = new QAction(tr("my shader"));
    myShaderAct->setIcon(QIcon(ICOPATH(torus.svg)));
    connect(myShaderAct, &QAction::triggered, this, &ProfileMenu::on_clcCheckShader);
    profileMenu->addAction(myShaderAct);
    QAction* switchAct = new QAction(tr("switch account"));
    connect(switchAct, &QAction::triggered, this, &ProfileMenu::on_clcSwitchAccount);
    profileMenu->addAction(switchAct);
    QAction* exitAct = new QAction(tr("log out"));
    connect(exitAct, &QAction::triggered, this, &ProfileMenu::on_clcLogOut);
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

SDL_UI_MODULE::ProfileMenu::~ProfileMenu() {

}

void SDL_UI_MODULE::ProfileMenu::on_clcProfile() {

}

void SDL_UI_MODULE::ProfileMenu::on_clcCheckDevice() {

}

void SDL_UI_MODULE::ProfileMenu::on_clcCheckShader() {

}

void SDL_UI_MODULE::ProfileMenu::on_clcSwitchAccount() {

}

void SDL_UI_MODULE::ProfileMenu::on_clcLogOut() {

}
