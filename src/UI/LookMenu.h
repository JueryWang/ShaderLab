#ifndef LOOKMENU_H
#define LOOKMENU_H

#include <QMenu>

namespace SDL_UI_MODULE
{
    class LookMenu : public QMenu
    {
        Q_OBJECT
    public:
        LookMenu(const QString& title,bool showIcon = false,const char* iconpath = nullptr);
        ~LookMenu();
    private:
        void on_clcRepo();
        void on_clcQuery();
        void on_clcNewFriend();
        void on_clcQueryFriend();
        void on_clcQueryFriendReq();
        void on_clcGetHistory();
        void on_clcShowScrCap();
        void on_clcShowNews();
    };
}

#endif // LOOKMENU_H
