#ifndef UI_VAR_SHOWBOARD_H
#define UI_VAR_SHOWBOARD_H

#include <QWidget>
#include <QMap>
#include <QLabel>
#include <QVariant>
#include <QScrollArea>
#include <QPropertyAnimation>

class ClickableLabel : public QLabel
{
	Q_OBJECT
public:
	ClickableLabel(const QString& path = "", int size = 16);
signals:
	void clicked();
protected:
	void mousePressEvent(QMouseEvent* event) override;
};

class XA_UIMODULE_ShowBoard : public QWidget
{
	Q_OBJECT
public:
	XA_UIMODULE_ShowBoard(int width,const QMap<QString, QVariant>& var_map);
	~XA_UIMODULE_ShowBoard();
	void setWidth(int width);
public slots:
	void on_ShowVariables();
	void on_unShowVariables();
private:
	ClickableLabel* _icon;
	QScrollArea* _scrollarea;
	QWidget* _showboard;
	int width;
	
	QMap<QString, QVariant> var_mp;

	QPropertyAnimation* _animation;

};

#endif