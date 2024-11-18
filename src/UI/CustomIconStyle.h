#ifndef CUSTOMICONSTYLE_H
#define CUSTOMICONSTYLE_H

#include <QProxyStyle>
#include <QCommonStyle>

namespace SDL_UI_MODULE {

    class CustomIconStyle : public QProxyStyle {
    Q_OBJECT

    public:
        CustomIconStyle(int size);

    public:
        virtual int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;

    private:
        int m_size;
    };
}

#endif // CUSTOMICONSTYLE_H
