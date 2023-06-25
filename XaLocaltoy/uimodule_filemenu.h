#ifndef UI_MENU_FILE
#define UI_MENU_FILE
#include <QMenu>

class XA_UIModule_FileMenu : public QMenu
{
public:
	XA_UIModule_FileMenu(QString title);
	~XA_UIModule_FileMenu();

	void on_FileOpen();
	void on_FileSave();
private:

};
#endif