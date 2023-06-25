#include "uimodule_filemenu.h"
#include "OverallWindow.h"
#include <QAction>
#include <QLabel>

XA_UIModule_FileMenu::XA_UIModule_FileMenu(QString title)
{
	setTitle(title);
	setProperty("class", "blackMenu");
	setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground);

	QAction* openAction = new QAction(QStringLiteral("打开文件"));
	connect(openAction, &QAction::triggered, this, &XA_UIModule_FileMenu::on_FileOpen);
	this->addAction(openAction);

	QAction* saveAction = new QAction(QStringLiteral("保存文件"));
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
