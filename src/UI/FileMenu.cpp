#include "FileMenu.h"
#include "CustomIconStyle.h"
#include "Utility/FileSystem/ProjectProcessor.h"
#include "UI_DefaultDfs.h"
#include <QFileDialog>
#include <QApplication>

using namespace SDL_UI_MODULE;

SDL_UI_MODULE::FileMenu::FileMenu(const QString &title, bool showIcon, const char *icopath) {

    setTitle(title);
    setProperty("class", "blackMenu");
    setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    if (showIcon) {
        this->setIcon(QIcon(icopath));
    }

    QAction *newAction = new QAction(tr("new"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setIcon(QIcon(ICOPATH(new.svg)));
    connect(newAction, &QAction::triggered, this, &FileMenu::on_clcNew);
    this->addAction(newAction);

    QMenu *openMenu = new QMenu("Open");
    openMenu->setProperty("class", "blackMenu");
    openMenu->setIcon(QIcon(ICOPATH(open.svg)));
    QAction *openFromLocal = new QAction(tr("Open From Local.."));
    connect(openFromLocal, &QAction::triggered, this, &FileMenu::on_clcOpenLocal);
    openMenu->addAction(openFromLocal);
    QAction *openFromCloud = new QAction(tr("Open From Cloud.."));
    openFromCloud->setIcon(QIcon(ICOPATH(cloud.svg)));
    connect(openFromCloud, &QAction::triggered, this, &FileMenu::on_clcOpenCloud);
    openMenu->addAction(openFromCloud);
    QAction *openFromDevice = new QAction(tr("Open From Device.."));
    openFromDevice->setIcon(QIcon(ICOPATH(device.svg)));
    openMenu->addAction(openFromDevice);
    this->addMenu(openMenu);

    QMenu *importMenu = new QMenu(tr("Import"));
    importMenu->setProperty("class", "blackMenu");
    importMenu->setIcon(QIcon(ICOPATH(import.svg)));
    QAction *import3DAct = new QAction(tr("3D Object"));
    connect(import3DAct, &QAction::triggered, this, &FileMenu::on_clcImport3D);
    importMenu->addAction(import3DAct);
    QAction *importTextureAct = new QAction(tr("Texture Object"));
    importMenu->addAction(import3DAct);
    connect(importTextureAct, &QAction::triggered, this, &FileMenu::on_clcImportTexture);
    importMenu->addAction(importTextureAct);
    QAction *importAudioAct = new QAction(tr("Audio Object"));
    connect(importAudioAct, &QAction::triggered, this, &FileMenu::on_clcImportFont);
    importMenu->addAction(importAudioAct);
    this->addMenu(importMenu);

    QMenu *exportMenu = new QMenu(tr("Export"));
    exportMenu->setProperty("class", "blackMenu");
    exportMenu->setIcon(QIcon(ICOPATH(export.svg)));
    QAction *saveSDLAct = new QAction(tr("Output to SDL project(.sdl)"));
    connect(saveSDLAct, &QAction::triggered, this, &FileMenu::on_clcExportSDL);
    exportMenu->addAction(saveSDLAct);
    QAction *saveVideoAct = new QAction("Output to Video(.mp4)");
    exportMenu->addAction(saveVideoAct);
    this->addMenu(exportMenu);

    QAction *quitAction = new QAction("Exit");
    connect(quitAction, &QAction::triggered, this, &FileMenu::on_clcQuit);
    quitAction->setIcon(QIcon(ICOPATH(quit.svg)));
    this->addAction(quitAction);
}

SDL_UI_MODULE::FileMenu::~FileMenu() {

}

void SDL_UI_MODULE::FileMenu::on_clcNew() {

}

void SDL_UI_MODULE::FileMenu::on_clcOpenLocal() {
    
    QFileDialog::Options options = QFileDialog::Options(QFileDialog::DontUseNativeDialog);
    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open Project"), tr("Project"), "SDL Project (*.sdl)", nullptr, options);
    if (fileName.size())
    {
        SDL_UTILS::ProjectProcessor loader;
        loader.load(fileName.toLatin1());
    }
}

void SDL_UI_MODULE::FileMenu::on_clcOpenCloud() {

}

void SDL_UI_MODULE::FileMenu::on_clcSaveContext() {

}

void SDL_UI_MODULE::FileMenu::on_clcSaveLocal() {

}

void SDL_UI_MODULE::FileMenu::on_clcSaveCloud() {

}

void SDL_UI_MODULE::FileMenu::on_clcSaveDevice() {

}

void SDL_UI_MODULE::FileMenu::on_clcImport3D() {

}

void SDL_UI_MODULE::FileMenu::on_clcImportTexture() {

}

void SDL_UI_MODULE::FileMenu::on_clcImportAudio() {

}

void SDL_UI_MODULE::FileMenu::on_clcImportFont() {

}

void SDL_UI_MODULE::FileMenu::on_clcExportSDL() {

}

void SDL_UI_MODULE::FileMenu::on_clcExportVideo() {

}

void SDL_UI_MODULE::FileMenu::on_clcQuit() {

}
