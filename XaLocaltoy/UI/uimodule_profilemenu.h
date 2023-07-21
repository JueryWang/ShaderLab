#ifndef UI_PROFILE_MENU
#define UI_PROFILE_MENU
#include <QToolButton>

class XA_UIModule_ProfileMenu : public QToolButton
{
public:
	XA_UIModule_ProfileMenu(QString title);
	~XA_UIModule_ProfileMenu();

private://no need to add slots flag because there are no args between click signal and click event
	void on_clcProfile();
	void on_clcCheckDevice();
	void on_clcCheckShader();
	void on_clcSwitchAccount();
	void on_clcExitLoggin();
};
#endif
