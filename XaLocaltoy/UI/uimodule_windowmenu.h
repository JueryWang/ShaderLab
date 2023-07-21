#ifndef UI_MENU_WINDOW
#define UI_MENU_WINDOW

#include <QMenu>

class XA_UIModule_WINDOWMenu : public QMenu
{
public:
	XA_UIModule_WINDOWMenu(QString title, bool showIcon = false, const char* iconpath = nullptr);
	~XA_UIModule_WINDOWMenu();
private:
	void on_clcNewWindow();
	void on_clcOpenTerminal();
	void on_clcCloseTerminal();

	bool terminalOpened = false;
};


#endif

