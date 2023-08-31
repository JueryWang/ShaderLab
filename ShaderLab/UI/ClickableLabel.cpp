#include "ClickableLabel.h"
#include <QMouseEvent>

ClickableLabel::ClickableLabel(const QString& path, int size)
{
	this->setPixmap(QPixmap(path).scaled(size, size));
	this->setAlignment(Qt::AlignmentFlag::AlignTop);
}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		emit clicked();
	}

	return QLabel::mousePressEvent(event);
}