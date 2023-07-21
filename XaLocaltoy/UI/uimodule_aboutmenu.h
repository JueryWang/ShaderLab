#ifndef UI_MENU_ABOUT
#define UI_MENU_ABOUT
#include <QMenu>

class XA_UIModule_AboutMenu : public QMenu
{
	Q_OBJECT
public:
	XA_UIModule_AboutMenu(QString title, bool showIcon = false, const char* iconpath = nullptr);
	~XA_UIModule_AboutMenu();

private: //no need to add slots flag because there are no args between click signal and click event
	void on_clcAbout();
	void on_clcGatherShader();
	void on_clcDonate();
};
#endif