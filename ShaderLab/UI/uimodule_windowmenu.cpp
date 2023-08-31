#include "uimodule_windowmenu.h"
#include "../Utilitys/uitilityDfs.h"
#include "uimodule_preferenceWindow.h"
#include "uimodule_customIconStyle.h"
#include <QAction>

XA_UIModule_WINDOWMenu::XA_UIModule_WINDOWMenu(QString title, bool showIcon /*= false*/, const char* iconpath /*= nullptr*/)
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

	QAction* newWindowAct = new QAction(_STRING_WRAPPER("新建窗口"));
	connect(newWindowAct, &QAction::triggered, this, &XA_UIModule_WINDOWMenu::on_clcNewWindow);
	this->addAction(newWindowAct);

	QAction* togTerminalAct = new QAction(_STRING_WRAPPER("打开命令行"));
	connect(togTerminalAct, &QAction::triggered, this, &XA_UIModule_WINDOWMenu::on_clcOpenTerminal);
	this->addAction(togTerminalAct);

	QAction* preferenceAct = new QAction(_STRING_WRAPPER("偏好设置"));
	connect(preferenceAct, &QAction::triggered, this, &XA_UIModule_WINDOWMenu::on_clcPreference);
	preferenceAct->setIcon(QIcon(ICOPATH(setting.svg)));
	this->addAction(preferenceAct);
}

XA_UIModule_WINDOWMenu::~XA_UIModule_WINDOWMenu()
{

}

void XA_UIModule_WINDOWMenu::on_clcNewWindow()
{

}

void XA_UIModule_WINDOWMenu::on_clcOpenTerminal()
{

}

void XA_UIModule_WINDOWMenu::on_clcCloseTerminal()
{

}

void XA_UIModule_WINDOWMenu::on_clcPreference()
{
	static XA_UIModule_PreferenceWindow* prefrenceWindow = new XA_UIModule_PreferenceWindow();
	prefrenceWindow->show();
}

