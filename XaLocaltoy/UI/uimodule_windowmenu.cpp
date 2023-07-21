#include "uimodule_windowmenu.h"
#include "../Utilitys/uitilityDfs.h"
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

	QAction* newWindowAct = new QAction(_STRING_WRAPPER("�½�����"));
	connect(newWindowAct, &QAction::triggered, this, &XA_UIModule_WINDOWMenu::on_clcNewWindow);
	this->addAction(newWindowAct);

	QAction* togTerminalAct = new QAction(_STRING_WRAPPER("��������"));
	connect(togTerminalAct, &QAction::triggered, this, &XA_UIModule_WINDOWMenu::on_clcOpenTerminal);
	this->addAction(togTerminalAct);
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

