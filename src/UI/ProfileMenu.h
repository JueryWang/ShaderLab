#ifndef PROFILEMENU_H
#define PROFILEMENU_H

#include <QToolButton>

namespace SDL_UI_MODULE
{
    class ProfileMenu : public QToolButton
    {
    public:
        ProfileMenu(const QString& title);
        ~ProfileMenu();
    private:
        void on_clcProfile();
        void on_clcCheckDevice();
        void on_clcCheckShader();
        void on_clcSwitchAccount();
        void on_clcLogOut();
    };

}

#endif // PROFILEMENU_H
