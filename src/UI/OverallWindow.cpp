#include "OverallWindow.h"
#include "UI/UI_DefaultDfs.h"
#include "UI/AssetWindow.h"
#include "UI/EditorPage.h"
#include "UI/GLWidget.h"
#include "UI/EditorPage.h"
#include "UI/WindowInfoPannel.h"
#include "UI/CodeEditor.h"
#include "UI/VarShowBoard.h"
#include "GL/GL_DefaultDfs.h"
#include "GL/GLRenderer.h"

#include "GL/GLSessionManager.h"
#include "Utility/Utils_DefaultDfs.h"
#include "Utility/Utils_SessionManager.h"
#include "Utility/Parser/ShaderParser.h"
#include <QToolButton>
#include <QFile>
#include <QFileInfo>
#include <QScreen>
#include <QSpacerItem>
#include <QApplication>
#include <QSizePolicy>
#include <QSplitter>

using namespace SDL_UI_MODULE;
using namespace SDL_GL_MODULE;
using namespace SDL_UTILS;

SDL_UI_MODULE::OverallWindow::OverallWindow() {
    init();
    this->setObjectName("OverallWindow");
    m_ovlayout = new QVBoxLayout(this);
    m_menubar = new MenuLayerTop(this);
    this->setContentsMargins(10,0,10,0);
    m_menubar->setFixedHeight(UI_COMPONENT_HEIGHT(menubar_height_refactor));
    this->setMinimumSize(ovWindow_minimum_width,ovWindow_minimum_height);
    this->resize(ovWindow_minimum_width,ovWindow_minimum_height);
    this->move(m_anchorPos);
    this->setStyleSheet("background-color:rgba(23,29,37,200)");

    m_ovlayout->addWidget(m_menubar);
    m_ovlayout->setContentsMargins(0,0,0,0);

    m_glWindow = new GLWidget(this,"Default GL Widget",this->width()*GL_INNER_UI_COMPONENT_DEFAULT_WIDTH_RATIO,this->height()*GL_INNER_UI_COMPONENT_DEFAULT_HEIGHT_RATIO);
    m_glWindow->__setMinimumSize(QSize(UI_COMPONENT_WIDTH(GL_INNER_UI_COMPONENT_MINIMUM_WIDTH_RATIO),UI_COMPONENT_HEIGHT(GL_INNER_UI_COMPONENT_MINIMUM_HEIGHT_RATIO)));
    m_glWindow->__setMaximumSize(QSize(UI_COMPONENT_WIDTH(GL_INNER_UI_COMPONENT_MAXIMUM_WIDTH_RATIO),UI_COMPONENT_HEIGHT(GL_INNER_UI_COMPONENT_MAXIMUM_HEIGHT_RATIO)));
    connect(m_glWindow,&GLWidget::resetGLWidget,this,&OverallWindow::on_resetGLWidget);
    WindowAssetBar::setAssetsReciver(m_glWindow->getRenderer());

    CodeEditor* editorInst = CodeEditor::getEditor();
    editorInst->setOverallWindow(this);
    editorInst->setEditorSize(this->width() * (1.0 - GL_INNER_UI_COMPONENT_DEFAULT_WIDTH_RATIO), this->height() * GL_INNER_UI_COMPONENT_DEFAULT_HEIGHT_RATIO);
    editorInst->setRenderer(m_glWindow->getRenderer());

    m_windowInfoPanel = new WindowInfoPanel(this,QSize(m_glWindow->width(),m_glWindow->height()),tr("Default Source"));
    m_windowInfoPanel->setFixedHeight(UI_COMPONENT_HEIGHT(window_infoPanel_width_factor));
    m_glWindow->setWindowInfoPanel(m_windowInfoPanel);

    m_varShowBoard = new VarShowBoard(editorInst->width());
    m_varShowBoard->setFixedHeight(20);
    m_varShowBoard->setAttribute(Qt::WA_TranslucentBackground,true);

    m_splitter_v1 = new QSplitter(this);
    m_splitter_v1->setOrientation(Qt::Vertical);
    m_splitter_v1->resize(m_glWindow->width(),this->height());
    m_splitter_v1->setAttribute(Qt::WA_TranslucentBackground,true);
    m_splitter_v1->setOrientation(Qt::Vertical);
    m_splitter_v1->addWidget(m_glWindow);
    QWidget* video_controlpanel_wrapper = new QWidget(m_splitter_v1);
    QVBoxLayout* video_controlpanel_layout = new QVBoxLayout();
    video_controlpanel_layout->addWidget(m_windowInfoPanel,Qt::AlignLeft | Qt::AlignTop);
    QWidget* placeholder = new QWidget();
    video_controlpanel_layout->addWidget(placeholder);
    video_controlpanel_wrapper->setLayout(video_controlpanel_layout);
    m_splitter_v1->addWidget(video_controlpanel_wrapper);
    m_splitter_v1->setStretchFactor(0,0);
    m_splitter_v1->setStretchFactor(1,1);

    m_splitter_v2 = new QSplitter(this);
    m_splitter_v2->resize(editorInst->width(),this->height());
    m_splitter_v2->setAttribute(Qt::WA_TranslucentBackground,true);
    m_splitter_v2->setOrientation(Qt::Vertical);
    m_splitter_v2->addWidget(m_varShowBoard);
    QWidget* editor_compilebar_wrapper = new QWidget(this);
    editor_compilebar_wrapper->resize(editorInst->width(),editorInst->height()+16);
    QVBoxLayout* editor_compilebar_layout = new QVBoxLayout();
    editor_compilebar_layout->setContentsMargins(0,0,0,0);
    ClickableLabel* compilebar = new ClickableLabel(ICOPATH(compilebar.svg),16);

    compilebar->setFixedHeight(UI_COMPONENT_HEIGHT(editor_compile_bar_height));
    connect(compilebar,&ClickableLabel::clicked,this,&OverallWindow::on_compileCode);
    editor_compilebar_layout->addWidget(editorInst);
    editor_compilebar_layout->addWidget(compilebar,Qt::AlignLeft);
    editor_compilebar_wrapper->setLayout(editor_compilebar_layout);

    m_splitter_v2->addWidget(editor_compilebar_wrapper);
    m_assetBar = editorInst->findAssetsBarByLabel("Image");
    m_splitter_v2->addWidget(editorInst->findAssetsBarByLabel("Image"));
    m_splitter_v2->setSizes({int(1.2*editorInst->height()),this->height()-editorInst->height()});
    m_splitter_v2->handle(0)->setDisabled(true);
    m_splitter_v2->setStretchFactor(0, 1);
    m_splitter_v2->setStretchFactor(1, 1);
    m_splitter_v2->setStretchFactor(2, 1);
    m_splitter_v2->setStretchFactor(3, 0);

    m_splitter_h = new QSplitter(this);
    m_splitter_h->resize(this->size());
    m_splitter_h->setAttribute(Qt::WA_TranslucentBackground,true);
    m_splitter_h->addWidget(m_splitter_v1);
    m_splitter_h->addWidget(m_splitter_v2);
    m_splitter_h->setStretchFactor(1,1);
    m_ovlayout->addWidget(m_splitter_h);

    this->setWindowIcon(QIcon("./Resources/icon/ShaderLabIco.png"));
    this->setWindowFlags(Qt::FramelessWindowHint);
}

SDL_UI_MODULE::OverallWindow::~OverallWindow() {

}

const QSize SDL_UI_MODULE::OverallWindow::getMontorsz() const {
    return QSize(screen_resolution_x,screen_resolution_y);
}

void SDL_UI_MODULE::OverallWindow::init() {
#if (QT_VERSION >= QT_VERSION_CHECK(5,11,0))
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5,14,0)) && defined(_WIN32)
	QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    QScreen* screen = QGuiApplication::primaryScreen();
    screen_resolution_x = screen->geometry().size().width();
    screen_resolution_y = screen->geometry().size().height();
    m_anchorPos = QPoint((screen_resolution_x - ovWindow_minimum_width)/2,(screen_resolution_y - ovWindow_minimum_height)/2);
    loadQssTheme();
    GLRenderer::__reset(QSize(UI_COMPONENT_WIDTH(GL_INNER_UI_COMPONENT_DEFAULT_WIDTH_RATIO), UI_COMPONENT_HEIGHT(GL_INNER_UI_COMPONENT_DEFAULT_HEIGHT_RATIO)));
    ShaderParser::setCachePath(USER_TEMPORARY_SHADER_PATH);

    GLSessionManager* glSessionManager=GLSessionManager::getInstance();
    glSessionManager->start(QThread::NormalPriority);
    connect(glSessionManager,&GLSessionManager::shaderParseDone,this,&OverallWindow::on_updateVarBoard);

    Utils_SessionManager::setAudioCachePath(USER_TEMPORARY_AUDIO_PATH);
    Utils_SessionManager* utilsSessionManager = Utils_SessionManager::getInstance();
    utilsSessionManager->QThread::start(QThread::NormalPriority);
}

void SDL_UI_MODULE::OverallWindow::rollbackNormal() {
    this->resize(m_normalSize);
    this->move(m_anchorPos);
    QToolButton* tbn = m_menubar->findChild<QToolButton*>("zoom Button");
    disconnect(tbn, &QToolButton::clicked, this, &OverallWindow::rollbackNormal);
    tbn->setStyleSheet(".QToolButton{background-color:transparent;border:1px solid rgba(255,255,255,0);\
							qproperty-icon: url(Resources/icon/fullscreen.svg);qproperty-iconSize: 20px 20px;}\
				 .QToolButton:hover,pressed,selected{padding:0px 0px;background-color:rgba(61, 68, 80, 1.0)}");
    connect(tbn, &QToolButton::clicked, this, &OverallWindow::setFullScreen);
}

void SDL_UI_MODULE::OverallWindow::setFullScreen() {
    this->resize(screen_resolution_x,screen_resolution_y);
    this->move(0,0);
    QToolButton* tbn = m_menubar->findChild<QToolButton*>("zoom Button");
    disconnect(tbn,&QToolButton::clicked,this,&OverallWindow::setFullScreen);
    tbn->setStyleSheet(".QToolButton{background-color:transparent;border:1px solid rgba(255,255,255,0);\
							qproperty-icon: url(Resources/icon/zoomOut.svg);qproperty-iconSize: 20px 20px;}\
				 .QToolButton:hover,pressed,selected{padding:0px 0px;background-color:rgba(61, 68, 80, 1.0)}");
    connect(tbn,&QToolButton::clicked,this,&OverallWindow::rollbackNormal);
}

void SDL_UI_MODULE::OverallWindow::setMinimum() {
    this->showMinimized();
}

void SDL_UI_MODULE::OverallWindow::on_resetGLWidget(const QSize &size) {

}

void SDL_UI_MODULE::OverallWindow::on_compileCode() {
    std::pair<GLRenderer*, SDL_GL_TASK> new_task;
    new_task.first = this->m_glWindow->m_glBackendRender;
    new_task.second.type = GL_COMPILE_SHADER;
    new_task.second.param.shaderCompileTask.rule = SDL_UTILS::ParserRule::ShaderToy;
    new_task.second.param.shaderCompileTask.varboard = this->m_varShowBoard;
    GLSessionManager::getInstance()->addTask(new_task);
}

void SDL_UI_MODULE::OverallWindow::on_updateVarBoard() {

}

void SDL_UI_MODULE::OverallWindow::on_changeAssetBar(WindowAssetBar *newBar) {
    m_splitter_v2->replaceWidget(2, newBar);
}

void SDL_UI_MODULE::OverallWindow::loadQssTheme() {
    m_qssPath = "./Resources/qss";
    QFile file(m_qssPath + "/default.qss");
    file.open(QIODevice::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    file.close();
    qApp->setStyleSheet(styleSheet);
}
