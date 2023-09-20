/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef UI_GLWIDGET_H
#define UI_GLWIDGET_H

#include <QWidget>
#include <QThread>
#include <QImage>

class XA_GLMODULE_RENDER;
class XA_UIMODULE_WindowInfo;

class XA_UIMODULE_GLWidget : public QWidget
{
	friend class OverallWindow;
	Q_OBJECT
public:
	XA_UIMODULE_GLWidget(QWidget* parent,const std::string& title,int width,int height);
	~XA_UIMODULE_GLWidget();
	XA_GLMODULE_RENDER* getRender();
	void setWindowInfoPanel(XA_UIMODULE_WindowInfo* inst);
	void __setMinimumSize(const QSize& size);
	void __setMaximumSize(const QSize& size);
	void __reshow();
	void __startRecord();
	void __endRecord();

public slots:
	void on_clickLockSize();
	void on_clickUnlockSize();
	void on_clickResetSize();

signals:
	void beginGLDraw();
	void resetGLWidget(const QSize &size);
protected:
	void paintEvent(QPaintEvent* event) override;
	bool eventFilter(QObject* obj, QEvent* event) override;

public:
	static uchar* _glwgt_pctbuffing;
	static int _glwgt_buffingsize;

private:
	QThread* _renderThread;
	XA_GLMODULE_RENDER* _glBackendRender;
	XA_UIMODULE_WindowInfo* _infoPanel;

	QImage _picture;
	QImage _recIcon;


	int wgt_width;
	int wgt_height;
	QSize dragged_size;
	QSize default_size;
	QSize minimum_size;
	QSize maximum_size;
	std::string _title;
	bool size_locked = false;
	bool recording = false;
};
#endif // ifndef UI_GLWIDGET_H
