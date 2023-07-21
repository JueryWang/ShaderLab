#include "uimodule_lookmenu.h"
#include "uimodule_customIconStyle.h"
#include "../Utilitys/uitilityDfs.h"
#include <QIcon>
#include <QAction>

XA_UIModule_LOOKMenu::XA_UIModule_LOOKMenu(QString title, bool showIcon /*= false*/, const char* iconpath /*= nullptr*/)
{
	setTitle(title);
	setProperty("class", "blackMenu");
	setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground);

	if (showIcon)
	{
		this->setIcon(QIcon(iconpath));
	}
	CustomIconStyle* style = new CustomIconStyle(24);

	QAction* repoAction = new QAction(_STRING_WRAPPER("库"));
	repoAction->setIcon(QIcon(ICOPATH(repo.svg)));
	connect(repoAction, &QAction::triggered, this, &XA_UIModule_LOOKMenu::on_clcRepo);
	this->addAction(repoAction);

	QMenu* friendMenu = new QMenu(_STRING_WRAPPER("好友"));
	friendMenu->setProperty("class", "blackMenu");
	friendMenu->setStyle(style);
	friendMenu->setIcon(QIcon(ICOPATH(friend.svg)));
	QAction* friendListAct = new QAction(_STRING_WRAPPER("好友列表"));
	connect(friendListAct, &QAction::triggered, this, &XA_UIModule_LOOKMenu::on_clcQueryFriend);
	friendMenu->addAction(friendListAct);
	QAction* addNewFriendAct = new QAction(_STRING_WRAPPER("添加好友"));
	connect(addNewFriendAct, &QAction::triggered, this, &XA_UIModule_LOOKMenu::on_clcNewFriend);
	friendMenu->addAction(addNewFriendAct);
	QAction* friendRequestAct = new QAction(_STRING_WRAPPER("查看好友申请"));
	connect(friendRequestAct, &QAction::triggered, this, &XA_UIModule_LOOKMenu::on_clcQueryFriendReq);
	friendMenu->addAction(friendRequestAct);
	this->addMenu(friendMenu);
	


	QMenu* historyMenu = new QMenu(_STRING_WRAPPER("历史"));
	historyMenu->setProperty("class", "blackMenu");
	QAction* nullAction = new QAction(_STRING_WRAPPER("未有历史记录"));
	historyMenu->addAction(nullAction);
	connect(nullAction, &QAction::hovered, this, &XA_UIModule_LOOKMenu::on_clcGetHistory);
	this->addMenu(historyMenu);
	//detect previous project and add Actions

	QAction *scrCapStorageAct = new QAction(_STRING_WRAPPER("截图"));
	connect(scrCapStorageAct, &QAction::triggered, this, &XA_UIModule_LOOKMenu::on_clcShowScrCap);
	this->addAction(scrCapStorageAct);

	QAction* updateNewsAct = new QAction(_STRING_WRAPPER("更新新闻"));
	connect(updateNewsAct, &QAction::triggered, this, &XA_UIModule_LOOKMenu::on_clcShowNews);
	this->addAction(updateNewsAct);
}

XA_UIModule_LOOKMenu::~XA_UIModule_LOOKMenu()
{

}

void XA_UIModule_LOOKMenu::on_clcRepo()
{

}

void XA_UIModule_LOOKMenu::on_clcQueryFriend()
{

}

void XA_UIModule_LOOKMenu::on_clcNewFriend()
{

}

void XA_UIModule_LOOKMenu::on_clcQueryFriendReq()
{

}

void XA_UIModule_LOOKMenu::on_clcGetHistory()
{
}

void XA_UIModule_LOOKMenu::on_clcShowScrCap()
{

}

void XA_UIModule_LOOKMenu::on_clcShowNews()
{

}

