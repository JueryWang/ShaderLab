#ifndef UI_VAR_SHOWBOARD_H
#define UI_VAR_SHOWBOARD_H

#include <QWidget>
#include <QMap>
#include <QLabel>
#include <QVariant>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QGridLayout>

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
	XA_UIMODULE_ShowBoard(int width);
	~XA_UIMODULE_ShowBoard();
	void setWidth(int width);
	void setVariantMap(const QMap<QString, QVariant>& var_mp);
private:
	void clearLayout(QLayout* layout);

public slots:
	void on_ShowVariables();
	void on_unShowVariables();
private:
	ClickableLabel* _icon;
	QScrollArea* _scrollarea;
	QWidget* _showboard;
	QGridLayout* var_grid;
	int width;
	
	QMap<QString, QVariant> _var_mp;
	QPropertyAnimation* _animation;

};

#endif