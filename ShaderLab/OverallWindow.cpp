#include "OverallWindow.h"
#include "UI/ui_defaultDfs.h"
#include "UI/uimodule_AssetsWindow.h"
#include "UI/uimodule_editorpage.h"
#include "UI/uimodule_glWidget.h"
#include "UI/uimodule_editorpage.h"
#include "UI/uimodule_windowInfo.h"
#include "UI/uimodule_codeEditor.h"
#include "GL/gl_defaultDfs.h"
#include "GL/glmodule_render.h"
#include "GL/glmodule_backstatge.h"
#include "Utilitys/uitilityDfs.h"
#include "Utilitys/utils_backendThread.h"
#include "Utilitys/Parser/utils_shaderParser.h"
#include <QtConcurrent/QtConcurrent>
#include <QToolButton>
#include <windows.h>
#include <QFileInfo>
#include <QFile>
#include <QScreen>
#include <QSpacerItem>
#include <QDesktopWidget>
#include <QApplication>
#include <QRegularExpression>
#include <QSizePolicy>
#include <QSplitter>

QString OverallWindow::qssPath;
QString OverallWindow::iconPath;
QMap<QString, QString> global_font_mp = QMap<QString, QString>();

OverallWindow::OverallWindow() 
{
	init();
	_owlayout = new QVBoxLayout(this);
	_menubar = new MenuLayertop(this);
	_menubar->setFixedHeight(35);
	this->setContentsMargins(10, 0, 10, 0);
	this->setMinimumSize(1600, 900);
	this->resize(1600, 900);
	this->move(anchorPos);
	this->setStyleSheet("background-color:rgba(23,29,37,200)");
	_owlayout->addWidget(_menubar);
	_owlayout->setContentsMargins(0, 0, 0, 0);


	_glWindow = new XA_UIMODULE_GLWidget(this,"default GL Widget",
		this->width()* GL_WIDGET_DEFAULT_WIDTH_R, this->height()* GL_WIDGET_DEFAULT_HEIGHT_R);
	_glWindow->__setMinimumSize(QSize(this->width() * GL_WIDGET_MIN_WIDTH_R, this->width() * GL_WIDGET_MIN_HEIGHT_R));
	_glWindow->__setMaximumSize(QSize(this->width() * GL_WIDGET_MAX_WIDTH_R, this->width() * GL_WIDGET_MAX_HEIGHT_R));
	connect(_glWindow, &XA_UIMODULE_GLWidget::resetGLWidget, this, &OverallWindow::on_restGLWidget);
	XA_UIMODULE_ASSET_BAR::setAssetsReciver(_glWindow->getRender());

	XA_UIMODULE_CodeEditor* codeEditorInst = XA_UIMODULE_CodeEditor::getEditor();
	codeEditorInst->setOverallWindow(this);

	codeEditorInst->setEditorSize(this->width() * (1. - GL_WIDGET_DEFAULT_WIDTH_R), this->height() * GL_WIDGET_DEFAULT_HEIGHT_R);
	_windowInfo = new XA_UIMODULE_WindowInfo(this, QSize(_glWindow->width(), _glWindow->height()),_STRING_WRAPPER("Ĭ��Դ"));
	_windowInfo->setFixedHeight(30);
	_glWindow->setWindowInfoPanel(_windowInfo);
	_varboard = new XA_UIMODULE_ShowBoard(codeEditorInst->width());
	_varboard->setFixedHeight(20);
	_varboard->setAttribute(Qt::WA_TranslucentBackground, true);

	QWidget* video_controlpanel_wrapper = new QWidget(this);
	QVBoxLayout* video_controlpanel_layout = new QVBoxLayout();
	splitter_v1 = new QSplitter();
	splitter_v1->resize(_glWindow->width(), this->height());
	splitter_v1->setAttribute(Qt::WA_TranslucentBackground, true);
	splitter_v1->setOrientation(Qt::Vertical);
	splitter_v1->addWidget(_glWindow);
	video_controlpanel_layout->addWidget(_windowInfo, Qt::AlignLeft | Qt::AlignTop);
	QWidget* placeholder = new QWidget();
	video_controlpanel_layout->addWidget(placeholder);
	video_controlpanel_wrapper->setLayout(video_controlpanel_layout);
	splitter_v1->addWidget(video_controlpanel_wrapper);
	splitter_v1->setStretchFactor(0, 0);
	splitter_v1->setStretchFactor(1, 1);

	splitter_v2 = new QSplitter();
	splitter_v2->resize(codeEditorInst->width(), this->height());
	splitter_v2->setAttribute(Qt::WA_TranslucentBackground, true);
	splitter_v2->setOrientation(Qt::Vertical);
	splitter_v2->addWidget(_varboard);
	QWidget* editor_compilebar_wrapper = new QWidget(this);
	editor_compilebar_wrapper->resize(codeEditorInst->width(), codeEditorInst->height() + 16);
	QVBoxLayout* editor_compilebar_layout = new QVBoxLayout();
	editor_compilebar_layout->setContentsMargins(0, 0, 0, 0);
	ClickableLabel* compilebar = new ClickableLabel(ICOPATH(compilebar.svg),16);

	compilebar->setFixedHeight(20);
	connect(compilebar, &ClickableLabel::clicked, this, &OverallWindow::on_compileCode);
	editor_compilebar_layout->addWidget(codeEditorInst);
	editor_compilebar_layout->addWidget(compilebar, Qt::AlignLeft);
	editor_compilebar_wrapper->setLayout(editor_compilebar_layout);

	splitter_v2->addWidget(editor_compilebar_wrapper);
	splitter_v2->addWidget(codeEditorInst->findAssetsBarByLabel("Image"));
	splitter_v2->setSizes({ int(1.2*codeEditorInst->height()),this->height() - codeEditorInst->height() });
	splitter_v2->handle(0)->setDisabled(true);
	//splitter_v2->handle(2)->setDisabled(true);
	splitter_v2->setStretchFactor(0, 1);
	splitter_v2->setStretchFactor(1, 1);
	splitter_v2->setStretchFactor(2, 1);
	splitter_v2->setStretchFactor(3, 0);

	splitter_h = new QSplitter(this);
	splitter_h->resize(this->size());
	splitter_h->setAttribute(Qt::WA_TranslucentBackground, true);
	splitter_h->addWidget(splitter_v1);
	splitter_h->addWidget(splitter_v2);
	splitter_h->setStretchFactor(1, 1);
	QWidget* spacing = new QWidget();
	_owlayout->addWidget(splitter_h);

	this->setWindowIcon(QIcon("Resources/icon/ShaderLabIco.png"));
	this->setWindowFlags(Qt::FramelessWindowHint);
}

OverallWindow::~OverallWindow()
{
	delete _menubar;
	delete _owlayout;
	delete _glWindow;
	delete _windowInfo;
	delete _varboard;
}

const QSize OverallWindow::getMonitorsz() const
{
	return this->monitor_resolution;
}

void OverallWindow::setFullScreen()
{
	this->resize(monitor_resolution);
	this->move(0, 0);
	QToolButton* tbn = _menubar->findChild<QToolButton*>("zoom Button");
	disconnect(tbn, &QToolButton::clicked, this, &OverallWindow::setFullScreen);
	tbn->setStyleSheet(".QToolButton{background-color:transparent;border:1px solid rgba(255,255,255,0);\
							qproperty-icon: url(Resources/icon/zoomOut.svg);qproperty-iconSize: 20px 20px;}\
				 .QToolButton:hover,pressed,selected{padding:0px 0px;background-color:rgba(61, 68, 80, 1.0)}");
	connect(tbn, &QToolButton::clicked, this, &OverallWindow::rollbackNormal);
}

void OverallWindow::setMinimum()
{
	this->showMinimized();
}

void OverallWindow::on_restGLWidget(const QSize& size)
{
	splitter_h->setSizes({ size.width(),this->normalSize.width() - size.width() });
	splitter_v1->setSizes({ size.height(),this->normalSize.height() - size.height() });
}

void OverallWindow::on_compileCode()
{
	std::pair<XA_GLMODULE_RENDER*, XA_GL_TASK> new_task;
	new_task.first = this->_glWindow->_glBackendRender;
	new_task.second.type = XA_GL_COMPILE_SHADER;
	new_task.second.param.compileTask_parm.rule = parser::ShaderToy;
	new_task.second.param.compileTask_parm.varboard = this->_varboard;
	XA_GLMODULE_BACKSTG::getBackStage()->addTask(new_task);
}

void OverallWindow::on_updateVarBoard()
{
	this->_varboard->setVariantMap(XA_UTILS_ShaderParser::getParser()->getParsedVar());
}

void OverallWindow::on_changeAssetsBar(XA_UIMODULE_ASSET_BAR* newBar)
{
	splitter_v2->replaceWidget(2, newBar);
}

void OverallWindow::rollbackNormal()
{
	this->resize(normalSize);
	this->move(anchorPos);
	QToolButton* tbn = _menubar->findChild<QToolButton*>("zoom Button");
	disconnect(tbn, &QToolButton::clicked, this, &OverallWindow::rollbackNormal);
	tbn->setStyleSheet(".QToolButton{background-color:transparent;border:1px solid rgba(255,255,255,0);\
							qproperty-icon: url(Resources/icon/fullscreen.svg);qproperty-iconSize: 20px 20px;}\
				 .QToolButton:hover,pressed,selected{padding:0px 0px;background-color:rgba(61, 68, 80, 1.0)}");
	connect(tbn, &QToolButton::clicked, this, &OverallWindow::setFullScreen);
}

void OverallWindow::init()
{
	QString s = __FILE__;
	QRegularExpression re;
	re.setPattern("^(.*)\\\\");
	QRegularExpressionMatch match = re.match(s);
	if (match.hasMatch())
	{
		qssPath = re.match(s).captured(0) + "Resources\\qss\\";
		iconPath = re.match(s).captured(0) + "Resources\\icon\\";
		QFile file(qssPath + "default.qss");
		file.open(QIODevice::ReadOnly);
		QString stylesheet = QLatin1String(file.readAll());
		file.close();
		qApp->setStyleSheet(stylesheet);
	}
	QDesktopWidget desktop;
	QRect monitorRct = QApplication::desktop()->availableGeometry();
	monitor_resolution = QSize(monitorRct.width(), monitorRct.height());
	anchorPos = QPoint((monitor_resolution.width() - normalSize.width()) / 2, (monitor_resolution.height() - normalSize.height()) / 2);
	XA_GLMODULE_RENDER::__reset(QSize(monitor_resolution.width() * GL_WIDGET_DEFAULT_WIDTH_R,
		monitor_resolution.height() * GL_WIDGET_DEFAULT_HEIGHT_R));
	XA_UTILS_ShaderParser::setCachePath(USER_TEMPORARY_SHADER_PATH);

	XA_GLMODULE_BACKSTG* gl_backstgThread = XA_GLMODULE_BACKSTG::getBackStage();
	gl_backstgThread->start(QThread::NormalPriority);
	connect(gl_backstgThread, &XA_GLMODULE_BACKSTG::shaderParsedone, this, &OverallWindow::on_updateVarBoard);

	XA_UTILS_BACKEND::setCachePath(USER_TEMPORARY_AUDIO_PATH);
	XA_UTILS_BACKEND* utils_backendThread = XA_UTILS_BACKEND::getUtilBackend();
	utils_backendThread->start(QThread::NormalPriority);
}


