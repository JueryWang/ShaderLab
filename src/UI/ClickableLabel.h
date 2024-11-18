#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>

namespace SDL_UI_MODULE {

    class ClickableLabel : public QLabel{
        Q_OBJECT
    public:
        ClickableLabel(const QString& path = "",int size = 16);
    signals:
        void clicked();
    protected:
        void mousePressEvent(QMouseEvent* event) override;
    };

}

#endif
