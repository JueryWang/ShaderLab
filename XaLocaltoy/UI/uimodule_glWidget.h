#ifndef UI_GLWIDGET_H
#define UI_GLWIDGET_H

#include <QWidget>
#include <QThread>
#include <QImage>

class XA_GLMODULE_RENDER;
class XA_UIMODULE_WindowInfo;

class XA_UIMODULE_GLWidget : public QWidget
{
	Q_OBJECT
public:
	XA_UIMODULE_GLWidget(const std::string& title,int width,int height);
	~XA_UIMODULE_GLWidget();
	void setWindowInfoPanel(XA_UIMODULE_WindowInfo* inst);

public slots:
	void on_clickLockSize();
	void on_clickResetSize();

signals:
	void beginGLDraw();

protected:
	void paintEvent(QPaintEvent* event) override;
	bool eventFilter(QObject* obj, QEvent* event) override;

private:
	QThread* _renderThread;
	XA_GLMODULE_RENDER* _glBackendRender;
	XA_UIMODULE_WindowInfo* _infoPanel;

	QImage _picture;

	static uchar* _glwgt_pctbuffing;
	static int _glwgt_buffingsize;

	int wgt_width;
	int wgt_height;
	QSize dragged_size;
	QSize default_size;
	std::string _title;
};
#endif // ifndef UI_GLWIDGET_H
