#include "uimodule_aboutmenu.h"
#include "../Utilitys/uitilityDfs.h"
#include <QAction>
#include <QLabel>

XA_UIModule_AboutMenu::XA_UIModule_AboutMenu(QString title, bool showIcon,const char* iconpath)
{
	setTitle(title);
	setProperty("class", "blackMenu");
	setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground);
	
	if (showIcon)
	{
		this->setIcon(QIcon(iconpath));
	}

	QAction* aboutAction = new QAction(_STRING_WRAPPER("关于ShaderLab"));
	connect(aboutAction, &QAction::triggered, this, &XA_UIModule_AboutMenu::on_clcAbout);
	this->addAction(aboutAction);

	QAction* getAction = new QAction(_STRING_WRAPPER("获取Shader"));
	connect(getAction, &QAction::triggered, this, &XA_UIModule_AboutMenu::on_clcGatherShader);
	this->addAction(getAction);

	QAction* donateAction = new QAction(_STRING_WRAPPER("捐赠"));
	connect(getAction, &QAction::triggered, this, &XA_UIModule_AboutMenu::on_clcDonate);
	this->addAction(donateAction);

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

void XA_UIModule_AboutMenu::on_clcDonate()
{

}
