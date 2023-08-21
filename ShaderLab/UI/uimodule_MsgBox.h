#ifndef UI_MSGBOX_H
#define UI_MSGBOX_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

class XA_UIModule_QUEST_BOX : public QWidget
{
	Q_OBJECT
public:
	XA_UIModule_QUEST_BOX(QWidget* parent, const QString& mainMsg, const QString& attachedMsg);
	~XA_UIModule_QUEST_BOX();
	static XA_UIModule_QUEST_BOX* question(QWidget* parent, const QString& title, const QString& text,const QSize& windowSz);

signals:
	void sendChoose(int val);

private:
	void on_clcSave();
	void on_clcUnsave();
	void on_clcCancel();
	bool eventFilter(QObject* obj, QEvent* event) override;

private:
	QWidget* _wgt;
	QLabel* _icon;
	QVBoxLayout* _vlay;
	QHBoxLayout* _hlay;
	QHBoxLayout* _btnsLay;

	bool _isDrag = false;
	QPoint _offsetPoint = QPoint(0,0);
	int _exec_res = -1;
};
#endif // 
