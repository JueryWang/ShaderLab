#include "WindowMenu.h"
#include "UI_DefaultDfs.h"
#include <QAction>

using namespace SDL_UI_MODULE;

SDL_UI_MODULE::WindowMenu::WindowMenu(const QString& title,bool showIcon,const char* iconpath) {
    setTitle(title);
    setProperty("class", "blackMenu");
    setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    if (showIcon)
    {
        this->setIcon(QIcon(iconpath));
    }

    QAction* newWindowAct = new QAction(tr("new Window"));
    connect(newWindowAct, &QAction::triggered, this, &WindowMenu::on_clcNewWindow);
    this->addAction(newWindowAct);

    QAction* togTerminalAct = new QAction(tr("Open Terminal"));
    connect(togTerminalAct, &QAction::triggered, this, &WindowMenu::on_clcOpenTerminal);
    this->addAction(togTerminalAct);

    QAction* preferenceAct = new QAction(tr("preference"));
    connect(preferenceAct, &QAction::triggered, this, &WindowMenu::on_clcPreference);
    preferenceAct->setIcon(QIcon(ICOPATH(setting.svg)));
    this->addAction(preferenceAct);
}

SDL_UI_MODULE::WindowMenu::~WindowMenu() {

}

void SDL_UI_MODULE::WindowMenu::on_clcNewWindow() {

}

void SDL_UI_MODULE::WindowMenu::on_clcOpenTerminal() {

}

void SDL_UI_MODULE::WindowMenu::on_clcCloseTerminal() {

}

void SDL_UI_MODULE::WindowMenu::on_clcPreference() {

}
