#ifndef UI_MENU_FILE
#define UI_MENU_FILE
#include <QMenu>

class XA_UIModule_FILEMenu : public QMenu
{
public:
	XA_UIModule_FILEMenu(QString title, bool showIcon = false, const char* iconpath = nullptr);
	~XA_UIModule_FILEMenu();

private:
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
};

#endif