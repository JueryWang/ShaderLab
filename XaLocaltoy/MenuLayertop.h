#ifndef  MENULAYERTOP_H
#define  MENULAYERTOP_H

#include <QMenu>
#include <QList>

class MenuLayertop : public QMenu
{
	Q_OBJECT
public: 
	MenuLayertop(QMenu* parent = NULL);
private:
	QList<QMenu*> menu_list;
};

#endif
