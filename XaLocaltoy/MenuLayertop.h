#ifndef  MENULAYERTOP_H
#define  MENULAYERTOP_H

#include "UI/uimodule_aboutmenu.h"
#include <QMenuBar>
#include <QList>

class MenuLayertop : public QMenuBar
{
	Q_OBJECT
public: 
	MenuLayertop(QMenuBar* parent = NULL);
private:
	QList<QMenu*> menu_list;
};

#endif
