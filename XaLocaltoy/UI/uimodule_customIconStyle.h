#ifndef CUSTOM_ICON_STYLE_H
#define CUSTOM_ICON_STYLE_H
#include <QProxyStyle>
#include <QCommonStyle>

class CustomIconStyle : public QProxyStyle
{
	Q_OBJECT
public:
	CustomIconStyle(int size)
		: QProxyStyle(), _size(size) {}

public:
	virtual int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
	{
		if (metric == QStyle::PM_SmallIconSize) {
			return _size;
		}
		return QProxyStyle::pixelMetric(metric, option, widget);
	}
private:
	int _size;
};


#endif // ifndef CUSTOM_ICON_STYLE