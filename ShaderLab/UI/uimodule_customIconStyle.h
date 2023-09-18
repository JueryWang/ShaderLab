/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/

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