#include "ClickableLabel.h"
#include <QMouseEvent>

namespace SDL_UI_MODULE {
    ClickableLabel::ClickableLabel(const QString &path, int size) {
        this->setPixmap(QPixmap(path).scaled(size,size));
        this->setAlignment(Qt::AlignmentFlag::AlignTop);
    }

    void ClickableLabel::mousePressEvent(QMouseEvent *event)
    {
        if(event->buttons() == Qt::LeftButton){
            emit clicked();
        }

        return QLabel::mousePressEvent(event);
    }
}