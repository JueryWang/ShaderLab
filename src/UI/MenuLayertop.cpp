#include "MenuLayertop.h"
#include "UI_DefaultDfs.h"
#include "OverallWindow.h"
#include "AboutMenu.h"
#include "FileMenu.h"
#include "MsgBox.h"
#include "LookMenu.h"
#include "WindowMenu.h"
#include "ProfileMenu.h"
#include "CustomIconStyle.h"
#include "GLWidget.h"
#include "Utility/FileSystem/ProjectProcessor.h"
#include <QMouseEvent>
#include <QSizePolicy>
#include <QFileDialog>
#include <QCoreApplication>

using namespace SDL_UI_MODULE;

namespace SDL_UI_MODULE{
SDL_UI_MODULE::MenuLayerTop::MenuLayerTop(OverallWindow *parent) {
    setParent(parent);
    m_ovWindow = parent;
    m_topMenus = new QMenuBar(this);
    m_layout = new QHBoxLayout(this);

    CustomIconStyle *style = new CustomIconStyle(24);
    setStyle(style);

    AboutMenu* aboutMenu = new AboutMenu(tr("About&Update"),true, ICOPATH(ShaderLabIco.png));
    FileMenu* fileMenu = new FileMenu(tr("File"));
    LookMenu* lookMenu = new LookMenu(tr("Check"));
    WindowMenu* windowMenu = new WindowMenu(tr("Window"));
    ProfileMenu* profileMenu = new ProfileMenu("Xaiver");

	aboutMenu->setObjectName("ABOUT&UPDATE");
	aboutMenu->setMinimumWidth(UI_COMPONENT_WIDTH(about_menu_width_refactor));

    fileMenu->setObjectName("FILE");
    fileMenu->setMinimumWidth(UI_COMPONENT_WIDTH(file_menu_width_refactor));

    lookMenu->setObjectName("LOOK");
    lookMenu->setMinimumWidth(UI_COMPONENT_WIDTH(look_menu_width_refactor));

    windowMenu->setObjectName("WINDOW");
    windowMenu->setMinimumWidth(UI_COMPONENT_WIDTH(window_menu_width_refactor));

    profileMenu->setObjectName("PROFILE");
    profileMenu->setMinimumWidth(UI_COMPONENT_WIDTH(profile_menu_width_refactor));

    m_topMenus->setFont(QFont("Microsoft YaHei",10,QFont::Bold));
    m_topMenus->addMenu(aboutMenu);
    m_topMenus->addMenu(fileMenu);
    m_topMenus->addMenu(lookMenu);
    m_topMenus->addMenu(windowMenu);
    m_topMenus->addSeparator();
    m_topMenus->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QToolButton* closeBtn  = new QToolButton(this);
    closeBtn->setFixedSize(30,30);
    closeBtn->setFixedSize(30, 30);
    closeBtn->setStyleSheet(".QToolButton{background-color:transparent;border:1px solid rgba(255,255,255,0);\
								qproperty-icon: url(Resources/icon/close.svg);qproperty-iconSize: 20px 20px;}\
				 .QToolButton:hover,pressed,selected{padding:0px 0px;background-color:rgba(226, 46, 39, 1.0)}");
    closeBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(closeBtn, &QToolButton::clicked, this, &MenuLayerTop::on_clcClose);

    OverallWindow* owWindow = (OverallWindow*)parent;
    QToolButton* fullBtn = new QToolButton(this);
    fullBtn->setObjectName("zoom Button");
    fullBtn->setFixedSize(30, 30);
    fullBtn->setStyleSheet(".QToolButton{background-color:transparent;border:1px solid rgba(255,255,255,0);\
							qproperty-icon: url(Resources/icon/fullscreen.svg);qproperty-iconSize: 20px 20px;}\
				 .QToolButton:hover,pressed,selected{padding:0px 0px;background-color:rgba(61, 68, 80, 1.0)}");
    fullBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(fullBtn, &QToolButton::clicked, owWindow, &OverallWindow::setFullScreen);

    QToolButton* minimizeBtn = new QToolButton(this);
    minimizeBtn->setFixedSize(30, 30);
    minimizeBtn->setStyleSheet(".QToolButton{background-color:transparent;border:1px solid rgba(255,255,255,0);\
							qproperty-icon: url(Resources/icon/minimize.svg);qproperty-iconSize: 20px 20px;}\
				 .QToolButton:hover,pressed,selected{padding:0px 0px;background-color:rgba(61, 68, 80, 1.0)}");
    minimizeBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(minimizeBtn, &QToolButton::clicked, owWindow, &OverallWindow::setMinimum);

    m_layout->addWidget(m_topMenus, 0, Qt::AlignLeft | Qt::AlignTop);
    m_layout->addWidget(profileMenu, 1, Qt::AlignTop | Qt::AlignRight);
    m_layout->addWidget(minimizeBtn, 0, Qt::AlignTop | Qt::AlignRight);
    m_layout->addWidget(fullBtn, 0, Qt::AlignTop | Qt::AlignRight);
    m_layout->addWidget(closeBtn, 0, Qt::AlignTop | Qt::AlignRight);
    m_layout->setContentsMargins(6, 0, 0, 4);
    this->setLayout(m_layout);
    this->installEventFilter(this);
}

SDL_UI_MODULE::MenuLayerTop::~MenuLayerTop() {

}

bool SDL_UI_MODULE::MenuLayerTop::eventFilter(QObject *obj, QEvent *event) {

    switch (event->type()) {
        case QEvent::MouseButtonPress:
        {
            auto e = dynamic_cast<QMouseEvent*>(event);
            if(e->button() == Qt::LeftButton)
            {
                m_isDraging = true;
                m_offsetPoint = e->globalPosition().toPoint() - m_ovWindow->frameGeometry().topLeft();
            }
            event->accept();
            return true;
        }
        case QEvent::MouseMove:
        {
            auto e = dynamic_cast<QMouseEvent*>(event);
            if(m_isDraging)
            {
                m_ovWindow->move(e->globalPosition().toPoint()-m_offsetPoint);
            }
            event->accept();
            return true;
        }
        case QEvent::MouseButtonRelease:
        {
            auto e = dynamic_cast<QMouseEvent*>(event);
            if(e->button() == Qt::LeftButton)
            {
                m_isDraging = false;
            }
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}

void SDL_UI_MODULE::MenuLayerTop::on_clcClose() {
    OverallWindow* ovWindow = (OverallWindow*) this->parent();
    if((sdl_context_flag & SDL_CONTEXT_STATUS::Archive_updated) == 1)
    {
        QCoreApplication::exit();
    }
    else
    {
        QStringList btnTexts = {"Save","Don't Save","Cancel"};
        std::vector<std::function<void(void)>> callbacks;
        callbacks.push_back([&]() {
            QFileDialog::Options options = QFileDialog::Options(QFileDialog::DontUseNativeDialog);
            QString fileName = QFileDialog::getSaveFileName(nullptr, tr("Save File"), QDir::currentPath()+"/untitled.sdl", "SDL Project (*.sdl)", nullptr, options);
            QFileInfo fileInfo(fileName);
            if (fileInfo.suffix().isEmpty() || fileInfo.suffix() != "sdl")
            {
                fileName += ".sdl";
            }

            if (fileName.size())
            {
                SDL_UTILS::ProjectProcessor projectSaver;
                projectSaver.setRenderer(m_ovWindow->getRenderWindow()->getRenderer());
                projectSaver.save(fileName.toLatin1());
            }

            QCoreApplication::exit();
        });
        callbacks.push_back([]() {
            QCoreApplication::exit();
        });
        callbacks.push_back(nullptr);

        MsgBox* quest = MsgBox::question(this,"Save changes before closing?","untitled.sdl",ovWindow->size(),btnTexts,callbacks);
    }
}
}
