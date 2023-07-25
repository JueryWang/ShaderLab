#ifndef  MENULAYERTOP_H
#define  MENULAYERTOP_H
#include <QMenuBar>
#include <QHBoxLayout>

class MenuLayertop : public QWidget
{
	friend class OverallWindow;
	Q_OBJECT
public: 
	MenuLayertop(QWidget* parent = NULL);
	~MenuLayertop();

public slots:
	void on_Exit_Msg(int val);

private:
	void on_clcClose();
private:
	QMenuBar* _topMenus;
	QHBoxLayout* _layout;
};

#endif
