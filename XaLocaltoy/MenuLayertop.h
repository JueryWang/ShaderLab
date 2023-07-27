#ifndef  MENULAYERTOP_H
#define  MENULAYERTOP_H
#include <QMenuBar>
#include <QHBoxLayout>

class OverallWindow;
class MenuLayertop : public QWidget
{
	friend class OverallWindow;
	Q_OBJECT
public: 
	MenuLayertop(OverallWindow* parent = NULL);
	~MenuLayertop();

protected:
	bool eventFilter(QObject* obj, QEvent* event) override;

public slots:
	void on_Exit_Msg(int val);

private:
	void on_clcClose();
private:
	bool _isDrag = false;
	QPoint _offsetPoint = QPoint(0, 0);

	QMenuBar* _topMenus;
	QHBoxLayout* _layout;

	OverallWindow* _parentInst;
};

#endif
