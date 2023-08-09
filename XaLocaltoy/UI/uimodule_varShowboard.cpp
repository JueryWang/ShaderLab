#include "uimodule_varShowboard.h"
#include "ui_defaultDfs.h"
#include <QPixmap>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QMouseEvent>
#include <QFontDatabase>

XA_UIMODULE_ShowBoard::XA_UIMODULE_ShowBoard(int width,const QMap<QString, QVariant>& var_map)
{
	this->resize(width, 20);
	this->var_mp = var_mp;
	this->width = width;

	QVBoxLayout* vlay = new QVBoxLayout();
	_icon = new ClickableLabel(ICOPATH(pull-down.svg),16);
	connect(_icon, &ClickableLabel::clicked, this, &XA_UIMODULE_ShowBoard::on_ShowVariables);
	_icon->setFixedHeight(20);
	_icon->setStyleSheet("background-color:rgba(240,240,240,255)");
	vlay->addWidget(_icon, Qt::AlignLeft);

	this->var_mp.insert("erosionStrength", 1.0);
	this->var_mp.insert("VOLUME_FILTERING_NEAREST", 0);
	this->var_mp.insert("FBM_NOISE", 1);
	this->var_mp.insert("BASIC_ANIMATEDE_NOISE", 1);
	this->var_mp.insert("BASIC_ANIMATED_MEDIA", 0);
	this->var_mp.insert("BUNNY_VOLUME_SIZE", 32);

	_showboard = new QWidget(this);
	QGridLayout* grid = new QGridLayout();
	grid->setContentsMargins(10, 0, 20, 0);

	_scrollarea = new QScrollArea(this);
	_scrollarea->setAttribute(Qt::WA_TranslucentBackground, true);
	_scrollarea->setWidgetResizable(true);

	int fontId = QFontDatabase::addApplicationFont(FONTPATH(Roboto-BoldItalic.ttf));
	QStringList font_list = QFontDatabase::applicationFontFamilies(fontId);
	QFont font;
	font.setPointSize(10);
	font.setFamily(font_list[0]);

	int row = 0;
	for (const QString& var_name : var_mp.keys())
	{
		QLabel* label = new QLabel(_showboard);
		label->setStyleSheet("QLabel{color:#E18550;}");
		label->setAttribute(Qt::WA_TranslucentBackground, true);
		label->setFont(font);
		label->setText(var_name);
		grid->addWidget(label,row, 0, 1, 1);
		QLineEdit* editor = new QLineEdit();
		editor->setStyleSheet(R"(QLineEdit
			{
				color:white;
				border: 2px solid grey;  
				border-radius:5px;
				padding: 0 0px 0 0px;  
			})");
		editor->setAttribute(Qt::WA_TranslucentBackground, true);
		editor->setFont(font);
		QPalette palette = editor->palette();
		palette.setColor(QPalette::Normal, QPalette::PlaceholderText, QColor("#92938A"));
		editor->setPalette(palette);
		editor->setFixedHeight(25);
		editor->setPlaceholderText(var_mp[var_name].toString());
		grid->addWidget(editor, row, 1, 1, 2);
		row++;
	}

	_showboard->setLayout(grid);

	vlay->addWidget(_scrollarea);
	vlay->setContentsMargins(0, 0, 0, 0);
	_scrollarea->setWidget(_showboard);
	_scrollarea->hide();

	_animation = new QPropertyAnimation(_showboard, "maximumHeight");
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