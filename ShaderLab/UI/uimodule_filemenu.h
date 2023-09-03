#ifndef UI_MENU_FILE_H
#define UI_MENU_FILE_H

#include <QMenu>
#include <QFile>

class XA_UIModule_FILEMenu : public QMenu
{
	Q_OBJECT
public:
	XA_UIModule_FILEMenu(QString title, bool showIcon = false, const char* iconpath = nullptr);
	~XA_UIModule_FILEMenu();

private://no need to add slots flag because there are no args between click signal and click event
	void on_clcNew();
	void on_clcOpenLocal();
	void on_clcOpenCloud();
	void on_clcOpenDevice();
	void on_tgrSaveContxt();
	void on_clcSaveLocal();
	void on_clcSaveCloud();
	void on_clcSaveDevice();
	void on_clcImport3D();
	void on_clcImportTexture();
	void on_clcImportAudio();
	void on_clcImportFont();
	void on_clcExportSDL();
	void on_clcExportViedo();
	void on_clcQuit();

private:
	QFile* _file_handler;
};

#endif