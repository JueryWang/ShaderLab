#ifndef MENULAYERTOP_H
#define MENULAYERTOP_H
#include <QMenuBar>
#include <QHBoxLayout>

namespace SDL_UI_MODULE
{
class OverallWindow;
class MenuLayerTop : public QWidget
{
    friend class OverallWindow;
    Q_OBJECT
public:
    MenuLayerTop(OverallWindow* parent = NULL);
    ~MenuLayerTop();

protected:
    bool eventFilter(QObject* obj,QEvent* event) override;

private:
    void on_clcClose();

private:
    bool m_isDraging = false;
    QPoint m_offsetPoint = QPoint(0,0);

    QMenuBar* m_topMenus;
    QHBoxLayout* m_layout;

    OverallWindow* m_ovWindow;
};
}

#endif
