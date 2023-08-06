#ifndef UI_WINDOW_INFO_H
#define UI_WINDOW_INFO_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class XA_UIMODULE_WindowInfo : public QWidget
{
	friend class XA_UIMODULE_GLWidget;
	Q_OBJECT
public:
	XA_UIMODULE_WindowInfo(QWidget* parent,const QSize &initsize,const QString &source);
	void updateInfo(const QSize& newsize);

private:
	QLabel* crtSource_show;
	QLabel* crtSource_info;
	QLabel* crtSize_show;
	QLabel* crtSize_info;
	QPushButton* lockbtn;
	QPushButton* resetbtn;

	QSize _crtSoucesize;
	QSize _crtWindowsize;
	QString _source;
};

#endif