#ifndef  MENULAYERTOP_H
#define  MENULAYERTOP_H

#include <QMenuBar>
#include <QHBoxLayout>

class MenuLayertop : public QWidget
{
	Q_OBJECT
public: 
	MenuLayertop(QMenuBar* parent = NULL);
	~MenuLayertop();
	QMenuBar* GetMenus();
private:
	QMenuBar* _topMenus;
	QHBoxLayout* _layout;
};

#endif
