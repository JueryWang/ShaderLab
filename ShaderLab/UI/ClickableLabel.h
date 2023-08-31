#ifndef UI_CLICKABLE_LABEL_H
#define UI_CLICKABLE_LABEL_H

#include <QLabel>

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

#endif
