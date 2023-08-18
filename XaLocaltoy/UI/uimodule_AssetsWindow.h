#ifndef UI_ASSETS_WINDOW_H
#define UI_ASSETS_WINDOW_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QList>

class ASSET_WINDOW : public QWidget
{
public:
	ASSET_WINDOW(const QSize& size);
protected:
	virtual void paintEvent(QPaintEvent* event);
public:
	enum ShowType
	{
		NONE = 0,
		IMAGE,
		AUDIO
	}show_type;

	QLabel cross;
	QString asset_path;
	QImage show_img;
	QRect cross_rect;
};

class XA_UIMODULE_ASSET_WINDOW : public QWidget
{
public:
	XA_UIMODULE_ASSET_WINDOW(int index);
protected:
	virtual bool eventFilter(QObject* obj, QEvent* event);
private:
	QVBoxLayout* _vlay;
	ASSET_WINDOW* _window;
	QLabel* _label;
	bool opened_asset = false;

	QImage show_img;
	QString img_file;
	QString audio_file;
};

class XA_UIMODULE_ASSET_BAR : public QScrollArea
{
public:
	XA_UIMODULE_ASSET_BAR(int width);

private:
	QList<XA_UIMODULE_ASSET_WINDOW*> asset_win_list;
};
#endif
