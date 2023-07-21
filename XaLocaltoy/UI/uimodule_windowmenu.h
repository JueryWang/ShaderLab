#ifndef UI_MENU_WINDOW
#define UI_MENU_WINDOW

#include <QMenu>

class XA_UIModule_WINDOWMenu : public QMenu
{
	Q_OBJECT
public:
	XA_UIModule_WINDOWMenu(QString title, bool showIcon = false, const char* iconpath = nullptr);
	~XA_UIModule_WINDOWMenu();

private://no need to add slots flag because there are no args between click signal and click event
	void on_clcNewWindow();
	void on_clcOpenTerminal();
	void on_clcCloseTerminal();

	bool terminalOpened = false;
};


#endif

