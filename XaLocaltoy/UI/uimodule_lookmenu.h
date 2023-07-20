#ifndef UI_MENU_LOOK
#define UI_MENU_LOOK

#include <QMenu>

class XA_UIModule_LOOKMenu : public QMenu
{
public:
	XA_UIModule_LOOKMenu(QString title, bool showIcon = false, const char* iconpath = nullptr);
	~XA_UIModule_LOOKMenu();
private:
	void on_clcRepo();
	void on_clcQueryFriend();
	void on_clcNewFriend();
	void on_clcQueryFriendReq();
	void on_clcHistory();
	void on_clcShowScrCap();
	void on_clcShowNews();
};

#endif // UI_MENU_LOOK
