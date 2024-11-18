#include "LookMenu.h"
#include "CustomIconStyle.h"
#include "UI_DefaultDfs.h"

using namespace SDL_UI_MODULE;

namespace SDL_UI_MODULE {

    LookMenu::LookMenu(const QString &title, bool showIcon, const char *iconpath) {

        setTitle(title);
        setProperty("class", "blackMenu");
        setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
        this->setAttribute(Qt::WA_TranslucentBackground);

        if (showIcon) {
            this->setIcon(QIcon(iconpath));
        }

        QAction *repoAction = new QAction(tr("library"));
        repoAction->setIcon(QIcon(ICOPATH(repo.svg)));
        connect(repoAction, &QAction::triggered, this, &LookMenu::on_clcRepo);
        this->addAction(repoAction);

        QMenu *friendMenu = new QMenu(tr("friend"));
        friendMenu->setProperty("class", "blackMenu");
        friendMenu->setIcon(QIcon(ICOPATH(friend.svg)));
        QAction *friendListAct = new QAction(tr("friend list"));
        connect(friendListAct, &QAction::triggered, this, &LookMenu::on_clcQueryFriend);
        friendMenu->addAction(friendListAct);
        QAction *addNewFriendAct = new QAction(tr("add friend"));
        connect(addNewFriendAct, &QAction::triggered, this, &LookMenu::on_clcNewFriend);
        friendMenu->addAction(addNewFriendAct);
        QAction *friendRequestAct = new QAction(tr("friend request"));
        connect(friendRequestAct, &QAction::triggered, this, &LookMenu::on_clcQueryFriendReq);
        friendMenu->addAction(friendRequestAct);
        this->addMenu(friendMenu);


        QMenu *historyMenu = new QMenu(tr("history"));
        historyMenu->setProperty("class", "blackMenu");
        QAction *nullAction = new QAction(tr("no history record"));
        historyMenu->addAction(nullAction);
        connect(nullAction, &QAction::hovered, this, &LookMenu::on_clcGetHistory);
        this->addMenu(historyMenu);
        //detect previous project and add Actions

        QAction *scrCapStorageAct = new QAction(tr("screenshot"));
        connect(scrCapStorageAct, &QAction::triggered, this, &LookMenu::on_clcShowScrCap);
        this->addAction(scrCapStorageAct);

        QAction *updateNewsAct = new QAction(tr("update News"));
        connect(updateNewsAct, &QAction::triggered, this, &LookMenu::on_clcShowNews);
        this->addAction(updateNewsAct);
    }

    LookMenu::~LookMenu() {

    }

    void LookMenu::on_clcRepo() {

    }

    void LookMenu::on_clcQuery() {

    }

    void LookMenu::on_clcNewFriend() {

    }

    void LookMenu::on_clcQueryFriendReq() {

    }

    void LookMenu::on_clcGetHistory() {

    }

    void LookMenu::on_clcShowScrCap() {

    }

    void LookMenu::on_clcShowNews() {

    }

    void LookMenu::on_clcQueryFriend() {

    }
}
