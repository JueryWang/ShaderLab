#include "MsgBox.h"
#include "UI_DefaultDfs.h"
#include <QMouseEvent>
#include <QGuiApplication>
#include <QScreen>
using namespace SDL_UI_MODULE;

SDL_UI_MODULE::MsgBox::MsgBox(QWidget *parent, const QString &mainMsg, const QString &attachedMsg) {
    m_wgt = new QWidget(this);
    //static init cannot use macro UI_COMPONENT
    m_wgt->setFixedSize(QGuiApplication::primaryScreen()->geometry().size().width() * msgbox_width_refactor,
                        QGuiApplication::primaryScreen()->geometry().size().height() * msgbox_height_refactor);
    m_wgt->setProperty("class", "blackWidget");

    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);
    this->setVisible(true);
    m_wgt->installEventFilter(this);

    QVBoxLayout* vlay = new QVBoxLayout();
    QHBoxLayout* hlay = new QHBoxLayout();
    QHBoxLayout* btnsLay = new QHBoxLayout();
    m_icon = new QLabel();
    m_icon->setFixedSize(QGuiApplication::primaryScreen()->geometry().size().width() *msgbox_icon_width_refactor,
                         QGuiApplication::primaryScreen()->geometry().size().height() *msgbox_icon_height_refactor);

    hlay->addWidget(m_icon);
    hlay->addSpacing(20);
    QFont textFont("Microsoft YaHei", 10);
    QVBoxLayout* discripLayout = new QVBoxLayout();
    QLabel* maindisp = new QLabel();
    maindisp->setObjectName("MsgBoxMainDisp");

    maindisp->setFont(textFont);
    maindisp->setFixedHeight(30);
    maindisp->setWordWrap(true);
    maindisp->setText(mainMsg);
    discripLayout->addWidget(maindisp);
    QLabel* attachedDisp = new QLabel();
    attachedDisp->setObjectName("MsgBoxAttachedDisp");

    attachedDisp->setFont(textFont);
    attachedDisp->setWordWrap(true);
    attachedDisp->setText(attachedMsg);
    discripLayout->addWidget(attachedDisp);
    discripLayout->addSpacing(50);
    hlay->addLayout(discripLayout);
    vlay->addLayout(hlay);

    m_btn1 = new QPushButton();
    connect(m_btn1, &QPushButton::clicked, this, &MsgBox::on_clcBtn1);
    m_btn2 = new QPushButton();
    connect(m_btn2, &QPushButton::clicked, this, &MsgBox::on_clcBtn2);
    m_btn3 = new QPushButton();
    connect(m_btn3, &QPushButton::clicked, this, &MsgBox::on_clcBtn3);

    QFont btnFont("Arial Black", 10, 75);
    m_btn1->setObjectName("MsgBoxBtn1");
    m_btn1->setFont(btnFont);
    m_btn2->setObjectName("MsgBoxBtn2");
    m_btn2->setFont(btnFont);
    m_btn3->setObjectName("MsgBoxBtn3");
    m_btn3->setFont(btnFont);
    btnsLay->setSpacing(10);
    btnsLay->addSpacing(80);
    btnsLay->addWidget(m_btn1);
    btnsLay->addWidget(m_btn2);
    btnsLay->addWidget(m_btn3);

    vlay->addLayout(btnsLay);
    m_wgt->setLayout(vlay);
}

SDL_UI_MODULE::MsgBox::~MsgBox() {

}

MsgBox* SDL_UI_MODULE::MsgBox::question(QWidget *parent, const QString &title, const QString &text, const QSize &windowSz,
                               const QStringList &btnTexts, const std::vector<std::function<void(void)>> &callbacks) {
    static MsgBox *questMsg = new MsgBox(parent,title,text);
    questMsg->setButtonTexts(btnTexts);
    questMsg->setCallbacks(callbacks);
    QPixmap pix(ICOPATH(quest.svg));
    questMsg->m_icon->setPixmap(pix);
    questMsg->move((screen_resolution_x-questMsg->width())/2,(screen_resolution_y-questMsg->height())/2);
    questMsg->show();
    questMsg->m_wgt->activateWindow();
    questMsg->m_wgt->setFocus();
    questMsg->setAttribute(Qt::WA_DeleteOnClose,true);

    return questMsg;
}


MsgBox* SDL_UI_MODULE::MsgBox::warning(QWidget* parent, const QString& title, const QString& text, const QSize& windowSz, const QStringList& btnTexts, const const std::vector<std::function<void(void)>>& callbacks)
{
    static MsgBox* warningMsg = new MsgBox(parent, title, text);
    warningMsg->setButtonTexts(btnTexts);
    warningMsg->setCallbacks(callbacks);
    QPixmap pix(ICOPATH(warn.svg));
    warningMsg->m_icon->setPixmap(pix);
    warningMsg->move((screen_resolution_x - warningMsg->width()) / 2, (screen_resolution_y - warningMsg->height()) / 2);
	warningMsg->show();
	warningMsg->m_wgt->activateWindow();
	warningMsg->m_wgt->setFocus();
	warningMsg->setAttribute(Qt::WA_DeleteOnClose, true);

    return warningMsg;
}

void SDL_UI_MODULE::MsgBox::setButtonTexts(const QStringList &texts) {
    if(texts[0] != "") { m_btn1->setText(texts[0]); m_btn1->show();}
    else m_btn1->hide();

    if(texts[1] != "") {m_btn2->setText(texts[1]); m_btn2->show();}
    else m_btn2->hide();

    if(texts[2] != "") {m_btn3->setText(texts[2]); m_btn3->show();}
    else m_btn3->hide();
}

void SDL_UI_MODULE::MsgBox::setCallbacks(const std::vector<std::function<void(void)>> &callbacks) {
    if(callbacks[0] != nullptr) m_cb1 = callbacks[0];
    if(callbacks[1] != nullptr) m_cb2 = callbacks[1];
    if(callbacks[2] != nullptr) m_cb3 = callbacks[2];
}

void SDL_UI_MODULE::MsgBox::on_clcBtn1() {
    if (m_cb1)
        m_cb1();

    this->hide();
}

void SDL_UI_MODULE::MsgBox::on_clcBtn2() {
    if (m_cb2)
        m_cb2();

    this->hide();
}

void SDL_UI_MODULE::MsgBox::on_clcBtn3() {
    if (m_cb3)
        m_cb3();

    this->hide();
}

bool SDL_UI_MODULE::MsgBox::eventFilter(QObject *obj, QEvent *event) {
    switch (event->type()) {
        case QEvent::MouseButtonPress:
        {
            auto e = dynamic_cast<QMouseEvent*>(event);
            if(e->button() == Qt::LeftButton)
            {
                m_isDraging = true;
                m_offsetPoint = e->globalPosition().toPoint() - this->frameGeometry().topLeft();
            }
            event->accept();
            return true;
        }
        case QEvent::MouseMove:
        {
            auto e = dynamic_cast<QMouseEvent*>(event);
            if(m_isDraging)
            {
                this->move(e->globalPosition().toPoint() - m_offsetPoint);
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
        case QEvent::FocusOut:
        {
            this->hide();
        }
    }
    return QWidget::eventFilter(obj, event);
}
