#ifndef VARSHOWBOARD_H
#define VARSHOWBOARD_H

#include <QWidget>
#include <QMap>
#include <QVariant>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QGridLayout>
#include "ClickableLabel.h"

namespace SDL_UI_MODULE
{
    class VarShowBoard : public QWidget{
        friend class OverallWindow;
        Q_OBJECT
    public:
        VarShowBoard(int width);
        ~VarShowBoard();
        void setWidth(int width);
        void setVariantMap(const QMap<QString,QVariant>& var_mp);
    private:
        void clearLayout(QLayout* layout);

    public slots:
        void on_ShowVariables();
        void on_unShowVariables();
    private:
        SDL_UI_MODULE::ClickableLabel* m_icon;
        QScrollArea* m_scrollarea;
        QWidget* m_showBoard;
        QGridLayout* m_varGrid;
        int width;

        QMap<QString,QVariant> m_varMap;
        QPropertyAnimation* m_animation;
    };
};

#endif
