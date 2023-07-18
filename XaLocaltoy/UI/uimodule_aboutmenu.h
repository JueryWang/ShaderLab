#ifndef UI_MENU_FILE
#define UI_MENU_FILE
#include <QMenu>

class XA_UIModule_AboutMenu : public QMenu
{
public:
	XA_UIModule_AboutMenu(QString title, bool showIcon, const char* iconpath);
	~XA_UIModule_AboutMenu();

	void on_clcAbout();
	void on_clcGatherShader();
private:

};
#endif