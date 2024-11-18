#ifndef MSGBOX_H
#define MSGBOX_H

#include <array>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <functional>
#include <QLabel>

namespace SDL_UI_MODULE {
    class MsgBox : public QWidget{
        Q_OBJECT
    public:
        MsgBox(QWidget* parent,const QString& mainMsg,const QString& attachedMsg);
        ~MsgBox();
        static MsgBox* question(QWidget* parent,const QString& title,const QString& text,const QSize& windowSz,const QStringList& btnTexts,const std::vector<std::function<void(void)>> &callbacks);
        static MsgBox* warning(QWidget* parent, const QString& title, const QString& text, const QSize& windowSz, const QStringList& btnTexts, const const std::vector<std::function<void(void)>>& callbacks);
        void setButtonTexts(const QStringList& texts);
        void setCallbacks(const std::vector<std::function<void(void)>>& callbacks);
    private:
        void on_clcBtn1();
        void on_clcBtn2();
        void on_clcBtn3();
        bool eventFilter(QObject* obj,QEvent* event) override;

    private:
        QWidget* m_wgt;
        QLabel* m_icon;

        QPushButton* m_btn1;
        QPushButton* m_btn2;
        QPushButton* m_btn3;

        std::function<void(void)> m_cb1;
        std::function<void(void)> m_cb2;
        std::function<void(void)> m_cb3;
        bool m_isDraging = false;
        QPoint m_offsetPoint = QPoint(0,0);
        int m_exec_res = -1;
    };
}
#endif
