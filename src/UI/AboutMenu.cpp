#include "AboutMenu.h"
#include "UI_DefaultDfs.h"
#include "CustomIconStyle.h"
#include <QAction>
#include <QLabel>

using namespace SDL_UI_MODULE;

namespace SDL_UI_MODULE {

    SDL_UI_MODULE::AboutMenu::AboutMenu(const QString title, bool showIcon, const char *icopath) {

        setTitle(title);
        setProperty("class", "blackMenu");
        setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
        this->setAttribute(Qt::WA_TranslucentBackground);

        if (showIcon) {
            this->setIcon(QIcon(icopath));
        }

        QAction *aboutAction = new QAction(tr("About ShaderLab"));
        connect(aboutAction, &QAction::triggered, this, &AboutMenu::on_clcAbout);
        this->addAction(aboutAction);

        QAction *getAction = new QAction(tr("Gather Shader"));
        connect(getAction, &QAction::triggered, this, &AboutMenu::on_clcGatherShader);
        this->addAction(getAction);

        QAction *donateAction = new QAction(tr("Donate"));
        connect(donateAction, &QAction::triggered, this, &AboutMenu::on_clcDonate);
        this->addAction(donateAction);
    }

    SDL_UI_MODULE::AboutMenu::~AboutMenu() {

    }

    void SDL_UI_MODULE::AboutMenu::on_clcAbout() {

    }

    void SDL_UI_MODULE::AboutMenu::on_clcGatherShader() {

    }

    void SDL_UI_MODULE::AboutMenu::on_clcDonate() {

    }

}
