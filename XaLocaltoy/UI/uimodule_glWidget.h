#ifndef UI_GLWIDGET_H
#define UI_GLWIDGET_H

#include <QWidget>
#include <QThread>
#include <QImage>
#include "../GL/glmodule_render.h"

class XA_UIModule_GLWidget : public QWidget
{
	Q_OBJECT
public:
	XA_UIModule_GLWidget(const char* title,int width,int height);
	~XA_UIModule_GLWidget();
signals:
	void beginGLDraw();
protected:
	void paintEvent(QPaintEvent* event) override;
	bool eventFilter(QObject* obj, QEvent* event) override;

private:
	QThread* _renderThread;
	XA_GLMODULE_RENDER* _glBackendRender;
	QImage _picture;

	static uchar* _glwgt_pctbuffing;
	static int _glwgt_buffingsize;

	int wgt_width;
	int wgt_height;
};
#endif // ifndef UI_GLWIDGET_H
