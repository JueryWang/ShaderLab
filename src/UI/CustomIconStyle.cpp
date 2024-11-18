#include "CustomIconStyle.h"

SDL_UI_MODULE::CustomIconStyle::CustomIconStyle(int size) : QProxyStyle(),m_size(size){

}

int SDL_UI_MODULE::CustomIconStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
	if (metric == QStyle::PM_SmallIconSize) {
		return m_size;
	}
	return QProxyStyle::pixelMetric(metric, option, widget);
}

