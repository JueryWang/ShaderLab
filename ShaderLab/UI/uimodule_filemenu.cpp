#include "uimodule_filemenu.h"
#include "uimodule_customIconStyle.h"
#include "../Utilitys/uitilityDfs.h"
#include <iostream>
#include <QMenu>
#include <QAction>

XA_UIModule_FILEMenu::XA_UIModule_FILEMenu(QString title, bool showIcon, const char* iconpath)
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

	QAction* newAction = new QAction(_STRING_WRAPPER("�½�"));
	newAction->setShortcut(QKeySequence::New);
	newAction->setIcon(QIcon(ICOPATH(new.svg)));
	connect(newAction, &QAction::triggered, this, &XA_UIModule_FILEMenu::on_clcNew);
	this->addAction(newAction);
	
	QMenu* openMenu = new QMenu(_STRING_WRAPPER("��"));
	openMenu->setStyle(style);
	openMenu->setProperty("class", "blackMenu");
	openMenu->setIcon(QIcon(ICOPATH(open.svg)));
	QAction* openFromLocal = new QAction(_STRING_WRAPPER("�ӱ���.."));
	openFromLocal->setIcon(QIcon(ICOPATH(local.svg)));
	connect(openFromLocal, &QAction::triggered, this, &XA_UIModule_FILEMenu::on_clcOpenLocal);
	openMenu->addAction(openFromLocal);
	QAction* openFromClould = new QAction(_STRING_WRAPPER("���ƶ�.."));
	openFromClould->setIcon(QIcon(ICOPATH(cloud.svg)));
	connect(openFromClould, &QAction::triggered, this, &XA_UIModule_FILEMenu::on_clcOpenCloud);
	openMenu->addAction(openFromClould);
	QAction* openFromDevice = new QAction(_STRING_WRAPPER("���豸.."));
	connect(openFromDevice, &QAction::triggered, this, &XA_UIModule_FILEMenu::on_clcOpenCloud);
	openFromDevice->setIcon(QIcon(ICOPATH(device.svg)));
	openMenu->addAction(openFromDevice);
	this->addMenu(openMenu);


	QMenu* saveMenu = new QMenu(_STRING_WRAPPER("����"));
	saveMenu->setStyle(style);
	saveMenu->setProperty("class", "blackMenu");
	QAction* saveContextAct = new QAction(_STRING_WRAPPER("��ǰ״̬"));//only triggered by shortcut but not visiable and clickable
	saveContextAct->setShortcut(QKeySequence::Save);
	saveMenu->addAction(saveContextAct);
	connect(saveContextAct, &QAction::triggered, this, &XA_UIModule_FILEMenu::on_tgrSaveContxt);
	QAction* saveToLocal = new QAction(_STRING_WRAPPER("������.."));
	saveToLocal->setIcon(QIcon(ICOPATH(local.svg)));
	connect(saveToLocal, &QAction::triggered, this, &XA_UIModule_FILEMenu::on_clcSaveLocal);
	saveMenu->addAction(saveToLocal);
	QAction* saveToCloud = new QAction(_STRING_WRAPPER("���ƶ�.."));
	saveToCloud->setIcon(QIcon(ICOPATH(cloud.svg)));
	connect(saveToCloud, &QAction::triggered, this, &XA_UIModule_FILEMenu::on_clcSaveCloud);
	saveMenu->addAction(saveToCloud);
	QAction* saveToDevice = new QAction(_STRING_WRAPPER("���豸.."));
	saveToDevice->setIcon(QIcon(ICOPATH(device.svg)));
	saveMenu->addAction(saveToDevice);
	this->addMenu(saveMenu);

	QMenu* importMenu = new QMenu(_STRING_WRAPPER("����"));
	importMenu->setProperty("class", "blackMenu");
	importMenu->setStyle(style);
	importMenu->setIcon(QIcon(ICOPATH(import.svg)));
	QAction* import3DAct = new QAction(_STRING_WRAPPER("3D ����"));
	connect(import3DAct, &QAction::triggered, this, &XA_UIModule_FILEMenu::on_clcImport3D);
	
	importMenu->addAction(import3DAct);
	QAction* importTextureAct = new QAction(_STRING_WRAPPER("�������"));
	connect(importTextureAct, &QAction::triggered, this, &XA_UIModule_FILEMenu::on_clcImportTexture);
	importMenu->addAction(importTextureAct);
	QAction* importAudioAct = new QAction(_STRING_WRAPPER("��Ƶ����"));
	connect(importAudioAct, &QAction::triggered, this, &XA_UIModule_FILEMenu::on_clcImportAudio);
	importMenu->addAction(importAudioAct);
	QAction* importFontAct = new QAction(_STRING_WRAPPER("�������"));
	connect(importFontAct, &QAction::triggered, this, &XA_UIModule_FILEMenu::on_clcImportFont);
	importMenu->addAction(importFontAct);

	this->addMenu(importMenu);
	

	QMenu* exportMenu = new QMenu(_STRING_WRAPPER("����"));
	exportMenu->setProperty("class", "blackMenu");
	exportMenu->setStyle(style);
	exportMenu->setIcon(QIcon(ICOPATH(export.svg)));
	QAction* saveSDLAct = new QAction(_STRING_WRAPPER("�����SDL(.sdl)"));
	connect(saveSDLAct, &QAction::triggered, this, &XA_UIModule_FILEMenu::on_clcExportSDL);
	exportMenu->addAction(saveSDLAct);
	QAction* saveVideoAct = new QAction(_STRING_WRAPPER("�����Video(.mp4)"));
	connect(saveVideoAct, &QAction::triggered, this, &XA_UIModule_FILEMenu::on_clcExportViedo);
	exportMenu->addAction(saveVideoAct);
	this->addMenu(exportMenu);
	
	QAction* quitAction = new QAction(_STRING_WRAPPER("�˳�"));
	connect(quitAction, &QAction::triggered, this, &XA_UIModule_FILEMenu::on_clcQuit);
	quitAction->setIcon(QIcon(ICOPATH(quit.svg)));
	this->addAction(quitAction);
	
}

XA_UIModule_FILEMenu::~XA_UIModule_FILEMenu()
{

}

void XA_UIModule_FILEMenu::on_clcNew()
{
	std::cout << "XA_UIModule_FILEMenu::on_clcNew()" << std::endl;
}

void XA_UIModule_FILEMenu::on_clcOpenLocal()
{

}

void XA_UIModule_FILEMenu::on_clcOpenCloud()
{

}

void XA_UIModule_FILEMenu::on_clcOpenDevice()
{

}

void XA_UIModule_FILEMenu::on_tgrSaveContxt()
{
	std::cout << "XA_UIModule_FILEMenu::on_tgrSaveContxt()" << std::endl;
}

void XA_UIModule_FILEMenu::on_clcSaveLocal()
{

}

void XA_UIModule_FILEMenu::on_clcSaveCloud()
{

}

void XA_UIModule_FILEMenu::on_clcSaveDevice()
{

}

void XA_UIModule_FILEMenu::on_clcImport3D()
{

}

void XA_UIModule_FILEMenu::on_clcImportTexture()
{

}

void XA_UIModule_FILEMenu::on_clcImportAudio()
{

}

void XA_UIModule_FILEMenu::on_clcImportFont()
{

}

void XA_UIModule_FILEMenu::on_clcExportSDL()
{

}

void XA_UIModule_FILEMenu::on_clcExportViedo()
{

}

void XA_UIModule_FILEMenu::on_clcQuit()
{

}
