#include "uimodule_filemenu.h"
#include "OverallWindow.h"
#include <QAction>
#include <QLabel>

XA_UIModule_FileMenu::XA_UIModule_FileMenu(QString title, bool showIcon = false,const char* iconpath = nullptr)
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
	connect(openAction, &QAction::triggered, this, &XA_UIModule_FileMenu::on_FileOpen);
	this->addAction(openAction);

	QAction* saveAction = new QAction(QStringLiteral("获取Shader"));
	connect(saveAction, &QAction::triggered, this, &XA_UIModule_FileMenu::on_FileSave);
	this->addAction(saveAction);

}

XA_UIModule_FileMenu::~XA_UIModule_FileMenu()
{

}

void XA_UIModule_FileMenu::on_FileOpen()
{

}

void XA_UIModule_FileMenu::on_FileSave()
{

}
