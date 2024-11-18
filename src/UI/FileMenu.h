#ifndef FILEMENU_H
#define FILEMENU_H

#include <QMenu>
#include <QFile>

namespace SDL_UI_MODULE
{

class FileMenu : public QMenu
{
    Q_OBJECT

public:
    FileMenu(const QString& title,bool showIcon = false,const char* icopath = nullptr);
    ~FileMenu();

private:
    void on_clcNew();
    void on_clcOpenLocal();
    void on_clcOpenCloud();
    void on_clcSaveContext();
    void on_clcSaveLocal();
    void on_clcSaveCloud();
    void on_clcSaveDevice();
    void on_clcImport3D();
    void on_clcImportTexture();
    void on_clcImportAudio();
    void on_clcImportFont();
    void on_clcExportSDL();
    void on_clcExportVideo();
    void on_clcQuit();

private:
    QFile m_fileHandler;
};

}
#endif // FILEMENU_H
