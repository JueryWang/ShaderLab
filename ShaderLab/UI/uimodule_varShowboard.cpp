#include "uimodule_varShowboard.h"
#include "ui_defaultDfs.h"
#include <QPixmap>
#include <QVBoxLayout>
#include <QFont>
#include <QLineEdit>
#include <QMouseEvent>
#include <QFontDatabase>
#include <QDebug>

QFont var_font;

XA_UIMODULE_ShowBoard::XA_UIMODULE_ShowBoard(int width)
{
	this->resize(width, 20);
	this->width = width;

	QVBoxLayout* vlay = new QVBoxLayout();
	_icon = new ClickableLabel(ICOPATH(pull-down.svg),16);
	connect(_icon, &ClickableLabel::clicked, this, &XA_UIMODULE_ShowBoard::on_ShowVariables);
	_icon->setFixedHeight(20);
	_icon->setStyleSheet("background-color:rgba(240,240,240,255)");
	vlay->addWidget(_icon, Qt::AlignLeft);

	_showboard = new QWidget(this);
	var_grid = new QGridLayout();
	var_grid->setContentsMargins(10, 0, 20, 0);

	_scrollarea = new QScrollArea(this);
	_scrollarea->setAttribute(Qt::WA_TranslucentBackground, true);
	_scrollarea->setWidgetResizable(true);

	int fontId = QFontDatabase::addApplicationFont(FONTPATH(Roboto-BoldItalic.ttf));
	QStringList font_list = QFontDatabase::applicationFontFamilies(fontId);
	global_font_mp["Roboto-BoldItalic"] = font_list[0];
	var_font.setPointSize(10);
	var_font.setFamily(font_list[0]);

	_showboard->setLayout(var_grid);

	vlay->addWidget(_scrollarea);
	vlay->setContentsMargins(0, 0, 0, 0);
	_scrollarea->setWidget(_showboard);
	_scrollarea->hide();

	_animation = new QPropertyAnimation(_scrollarea, "maximumHeight");
	_animation->setEasingCurve(QEasingCurve::Linear);

	this->setLayout(vlay);
}

XA_UIMODULE_ShowBoard::~XA_UIMODULE_ShowBoard()
{
	delete _icon;
	delete _showboard;
	delete _scrollarea;
	delete _animation;
}

void XA_UIMODULE_ShowBoard::setWidth(int width)
{
	this->width = width;
}

void XA_UIMODULE_ShowBoard::setVariantMap(const QMap<QString, QVariant>& var_mp)
{
	this->on_unShowVariables();

	this->_var_mp = var_mp;
	clearLayout(var_grid);
	int row = 0;
	for (const QString& var_name : _var_mp.keys())
	{
		QLabel* label = new QLabel(_showboard);
		label->setStyleSheet("QLabel{color:#E18550;}");
		label->setAttribute(Qt::WA_TranslucentBackground, true);
		label->setFont(var_font);
		label->setText(var_name);
		var_grid->addWidget(label, row, 0, 1, 1);
		QLineEdit* editor = new QLineEdit();
		editor->setStyleSheet(R"(QLineEdit
			{
				color:white;
				border: 2px solid grey;  
				border-radius:5px;
				padding: 0 0px 0 0px;  
			})");
		editor->setAttribute(Qt::WA_TranslucentBackground, true);
		editor->setFont(var_font);
		QPalette palette = editor->palette();
		palette.setColor(QPalette::Normal, QPalette::PlaceholderText, QColor("#92938A"));
		editor->setPalette(palette);
		editor->setFixedHeight(25);
		editor->setPlaceholderText(_var_mp[var_name].toString());
		var_grid->addWidget(editor, row, 1, 1, 2);
		row++;
	}

	var_grid->update();
}

void XA_UIMODULE_ShowBoard::clearLayout(QLayout* layout)
{
	QLayoutItem* item;
	while ((item = layout->takeAt(0)) != 0)
	{
		if (item->widget()) {
			delete item->widget();
		}

		QLayout* childLayout = item->layout();
		if (childLayout) {
			clearLayout(childLayout);
		}
		delete item;
	}
}

void XA_UIMODULE_ShowBoard::on_ShowVariables()
{
	_icon->setPixmap(QPixmap(ICOPATH(pull-up.svg)).scaled(16, 16));
	disconnect(_icon, &ClickableLabel::clicked, this, &XA_UIMODULE_ShowBoard::on_ShowVariables);
	connect(_icon, &ClickableLabel::clicked, this, &XA_UIMODULE_ShowBoard::on_unShowVariables);
	_scrollarea->show();
	_animation->setDuration(500);
	_animation->setStartValue(20);
	_animation->setEndValue(200);
	_animation->start();
	this->setFixedHeight(200);
}

void XA_UIMODULE_ShowBoard::on_unShowVariables()
{
	_icon->setPixmap(QPixmap(ICOPATH(pull-down.svg)).scaled(16, 16));
	disconnect(_icon, &ClickableLabel::clicked, this, &XA_UIMODULE_ShowBoard::on_unShowVariables);
	connect(_icon, &ClickableLabel::clicked, this, &XA_UIMODULE_ShowBoard::on_ShowVariables);
	_scrollarea->hide();
	this->setFixedHeight(20);
}

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