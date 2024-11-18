#ifndef ABOUTMENU_H
#define ABOUTMENU_H

#include <QMenu>

namespace SDL_UI_MODULE {

    class AboutMenu : public QMenu {
    Q_OBJECT

    public:
        AboutMenu(const QString title, bool showIcon = false, const char *icopath = nullptr);

        ~AboutMenu();

    private:
        void on_clcAbout();

        void on_clcGatherShader();

        void on_clcDonate();
    };
}

#endif // ABOUTMENU_H
