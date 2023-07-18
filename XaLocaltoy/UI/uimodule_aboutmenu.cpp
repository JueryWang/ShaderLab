#include "UI/uimodule_aboutmenu.h"
#include "OverallWindow.h"
#include <QAction>
#include <QLabel>

XA_UIModule_AboutMenu::XA_UIModule_AboutMenu(QString title, bool showIcon = false,const char* iconpath = nullptr)
{
	setTitle(title);
	setProperty("class", "blackMenu");
	setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground);
	
	if (showIcon)
	{
		this->setIcon(QIcon(iconpath));
	}

	QAction* openAction = new QAction(QStringLiteral("关于ShaderLab"));
	connect(openAction, &QAction::triggered, this, &XA_UIModule_AboutMenu::on_clcAbout);
	this->addAction(openAction);

	QAction* saveAction = new QAction(QStringLiteral("获取Shader"));
	connect(saveAction, &QAction::triggered, this, &XA_UIModule_AboutMenu::on_clcGatherShader);
	this->addAction(saveAction);

}

XA_UIModule_AboutMenu::~XA_UIModule_AboutMenu()
{

}

void XA_UIModule_AboutMenu::on_clcAbout()
{

}

void XA_UIModule_AboutMenu::on_clcGatherShader()
{

}
