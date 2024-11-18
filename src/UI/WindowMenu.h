#ifndef WINDOWMENU_H
#define WINDOWMENU_H

#include <QMenu>
namespace SDL_UI_MODULE
{
    class WindowMenu : public QMenu
    {
        Q_OBJECT
    public:
        WindowMenu(const QString& title,bool showIcon = false,const char* iconpath = nullptr);
        ~WindowMenu();
    private:
        void on_clcNewWindow();
        void on_clcOpenTerminal();
        void on_clcCloseTerminal();
        void on_clcPreference();

        bool terminalOpened = false;
    };
}

#endif // WINDOWMENU_H
