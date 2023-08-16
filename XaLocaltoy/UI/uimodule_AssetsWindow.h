#ifndef UI_ASSETS_WINDOW_H
#define UI_ASSETS_WINDOW_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QList>

class XA_UIMODULE_ASSET_WINDOW : public QWidget
{
public:
	XA_UIMODULE_ASSET_WINDOW(int index);

private:
	QVBoxLayout* _vlay;
	QWidget* _window;
	QLabel* _label;

	QImage show_img;
};

class XA_UIMODULE_ASSET_BAR : public QScrollArea
{
public:
	XA_UIMODULE_ASSET_BAR(int width);

private:
	QList<XA_UIMODULE_ASSET_WINDOW*> asset_win_list;
};
#endif
