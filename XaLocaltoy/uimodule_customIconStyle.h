#ifndef CUSTOM_ICON_STYLE
#define CUSTOM_ICON_STYLE
#include <QProxyStyle>

class CustomIconStyle : public QProxyStyle
{
	Q_OBJECT

public:
	virtual int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
	{
		if (metric == QStyle::PM_SmallIconSize) {
			return 24;
		}
		return QProxyStyle::pixelMetric(metric, option, widget);
	}
};
#endif