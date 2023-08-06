#ifndef UI_WINDOW_INFO_H
#define UI_WINDOW_INFO_H

#include <QWidget>

class XA_UIMODULE_WindowInfo : public QWidget
{
	Q_OBJECT
public:
	XA_UIMODULE_WindowInfo(QWidget* parent,const QSize &initsize,const QString &source);

private:
	QSize _crtSoucesize;
	QSize _crtWindowsize;
	QString _source;
};

#endif