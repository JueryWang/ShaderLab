#include "GLWidget.h"
#include "UI_DefaultDfs.h"
#include "OverallWindow.h"
#include "WindowInfoPannel.h"
#include "GL/EvSendFrame.h"
#include "GL/GLRenderer.h"
#include <QApplication>
#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>

using namespace SDL_GL_MODULE;

namespace SDL_UI_MODULE
{
    uint8_t* GLWidget::sdl_glwgt_pctBuffing;
    int GLWidget::sdl_glwgt_buffingSize;

    bool first_resize = true;
    int alpha_flick = 255;// alpha value for flicking recording Icon
    int alpha_change_step = 5;

    SDL_UI_MODULE::GLWidget::GLWidget(QWidget *parent, const std::string &title, int width, int height) :m_wgt_width(width),m_wgt_height(height),m_title(title){
        this->resize(width, height);
		this->setParent(parent);
		default_size = QSize(width, height);

		m_recIcon = QImage(ICOPATH(rec.png));
		m_glBackendRender = new GLRenderer(title, SDL_GL_MODULE::PixelType::SDL_GL_RGB, this);
		GLWidget::sdl_glwgt_buffingSize = GLRenderer::SCR_WIDTH * GLRenderer::SCR_HEIGHT * 3 * sizeof(uchar);
		GLWidget::sdl_glwgt_pctBuffing = (uint8_t*)malloc(GLRenderer::SCR_WIDTH * GLRenderer::SCR_HEIGHT * 3 * sizeof(uchar));
		memset(GLWidget::sdl_glwgt_pctBuffing, 0, GLWidget::sdl_glwgt_buffingSize);
		m_renderThread = new QThread(this);
		m_glBackendRender->moveToThread(m_renderThread);

		connect(m_renderThread, &QThread::finished, m_renderThread, &QThread::deleteLater);
		connect(this, &GLWidget::beginGLDraw, m_glBackendRender, &GLRenderer::contextDraw);

		m_renderThread->start();

		this->installEventFilter(this);
        m_fpsTicker = new QTimer(this);
        connect(m_fpsTicker, &QTimer::timeout, [=]()
            {
                m_infoPanel->updateFPS(m_glBackendRender->getFPS());
            });
        m_fpsTicker->start(100);
        m_picture = QImage(sdl_glwgt_pctBuffing, m_glBackendRender->getWidth(), m_glBackendRender->getHeight(), QImage::Format_RGB888);
        repaint();
    }

    SDL_UI_MODULE::GLWidget::~GLWidget() {
        m_glBackendRender->__exit();
        m_renderThread->wait();
        m_renderThread->quit();
        free(GLWidget::sdl_glwgt_pctBuffing);
        delete m_glBackendRender;
    }

    GLRenderer* SDL_UI_MODULE::GLWidget::getRenderer() {
        return m_glBackendRender;
    }

    void SDL_UI_MODULE::GLWidget::setWindowInfoPanel(WindowInfoPanel *panel) {
        m_infoPanel = panel;
        panel->glWindowInst = this;
        connect(m_infoPanel->m_lockBtn,&QPushButton::clicked,this,&GLWidget::on_clcLockSize);
        connect(m_infoPanel->m_resetBtn,&QPushButton::clicked,this,&GLWidget::on_clcResetSize);
    }

    void SDL_UI_MODULE::GLWidget::__setMinimumSize(const QSize &size) {
        minimum_size = size;
        setMinimumSize(size);
    }

    void SDL_UI_MODULE::GLWidget::__setMaximumSize(const QSize &size) {
        maximum_size = size;
        setMaximumSize(size);
    }

    void SDL_UI_MODULE::GLWidget::__reshow() {
        emit beginGLDraw();
    }

    void SDL_UI_MODULE::GLWidget::__startRecord() {
        m_glBackendRender->__record();
        alpha_flick = 255;
        recording = true;
    }

    void SDL_UI_MODULE::GLWidget::__endRecord() {
        m_glBackendRender->__endrecord();
        recording = false;
    }

    void SDL_UI_MODULE::GLWidget::on_clcLockSize() {
        size_locked = true;
        setFixedSize(m_wgt_width,m_wgt_height);
        m_infoPanel->m_lockBtn->setText("Unlock current aspect ratio");
        m_infoPanel->m_lockBtn->setIcon(QIcon(ICOPATH(unlock.svg)));
        disconnect(m_infoPanel->m_lockBtn,&QPushButton::clicked,this,&GLWidget::on_clcLockSize);
        connect(m_infoPanel->m_lockBtn,&QPushButton::clicked,this,&GLWidget::on_clcUnlockSize);
    }

    void SDL_UI_MODULE::GLWidget::on_clcUnlockSize() {
        size_locked = false;
        this->setFixedSize(m_wgt_width,m_wgt_height);
        m_infoPanel->m_lockBtn->setText(tr("Lock current aspect ratio"));
        m_infoPanel->m_lockBtn->setIcon(QIcon(ICOPATH(unlock.svg)));
        disconnect(m_infoPanel->m_lockBtn,&QPushButton::clicked,this,&GLWidget::on_clcUnlockSize);
        connect(m_infoPanel->m_lockBtn,&QPushButton::clicked,this,&GLWidget::on_clcLockSize);

        this->setMaximumSize(maximum_size);
        this->setMinimumSize(minimum_size);
    }

    void SDL_UI_MODULE::GLWidget::on_clcResetSize() {
        size_locked = false;
        this->setMaximumSize(maximum_size);
        this->setMinimumSize(minimum_size);

        QResizeEvent *ev = new QResizeEvent(default_size,QSize(m_wgt_width,m_wgt_height));
        QApplication::postEvent(this,ev);
        emit resetGLWidget(default_size);

        m_wgt_width = default_size.width();m_wgt_height = default_size.height();

        m_infoPanel->m_lockBtn->setText(tr("Lock current aspect"));
        m_infoPanel->m_lockBtn->setIcon(QIcon(ICOPATH(locksize.svg)));
        disconnect(m_infoPanel->m_lockBtn,&QPushButton::clicked,this,&GLWidget::on_clcLockSize);
        disconnect(m_infoPanel->m_lockBtn,&QPushButton::clicked,this,&GLWidget::on_clcUnlockSize);
        connect(m_infoPanel->m_lockBtn,&QPushButton::clicked,this,&GLWidget::on_clcLockSize);
    }

    void GLWidget::paintEvent(QPaintEvent *event) {
		QPainterPath clipPath;
		clipPath.addRoundedRect(QRect(0, 0, m_wgt_width, m_wgt_height).adjusted(10, 10, -10, -10), 10, 10);
		QPainter painter(this);

		painter.setClipPath(clipPath);
		painter.drawImage(QRect(0, 0, m_wgt_width, m_wgt_height), m_picture);

		if (recording)
		{
			for (int col = 0; col < 48; col++)
			{
				for (int row = 0; row < 48; row++)
				{
					QColor pixelColor = m_recIcon.pixelColor(row, col);
					if (pixelColor.alpha() != 0)
						pixelColor.setAlpha(alpha_flick);
					m_recIcon.setPixelColor(row, col, pixelColor);
				}
			}
			QRect recIconRegion = QRect(m_wgt_width - 70, 10, 48, 48);
			painter.setRenderHint(QPainter::Antialiasing);
			painter.drawImage(recIconRegion, m_recIcon);
			if ((alpha_flick + alpha_change_step) < 100 || (alpha_flick + alpha_change_step) > 255)
			{
				alpha_change_step = -alpha_change_step;
			}
			alpha_flick = alpha_flick + alpha_change_step;
		}

		painter.end();
    }

    bool GLWidget::eventFilter(QObject *obj, QEvent *event) {
        if(event->type() == EvSendFrame::eventType)
        {
            m_picture = QImage(sdl_glwgt_pctBuffing,m_glBackendRender->getWidth(),m_glBackendRender->getHeight(),QImage::Format_RGB888);
            this->repaint();
        }
        if(event->type() == QEvent::Resize)
        {
            if(!first_resize && !size_locked)
            {
                QResizeEvent* ev = dynamic_cast<QResizeEvent*>(event);
                dragged_size = ev->size();
                if(m_infoPanel != nullptr)
                    m_infoPanel->updateInfo(dragged_size);
                m_wgt_width = dragged_size.width();
                m_wgt_height = dragged_size.height();
            }else
            {
                first_resize = false;
            }
        }
        return QObject::eventFilter(obj, event);
    }
}
