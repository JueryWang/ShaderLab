#include "VarShowBoard.h"
#include "UI_DefaultDfs.h"
#include "ClickableLabel.h"
#include "Utility/Utils_DefaultDfs.h"
#include <QPixmap>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFontDatabase>
#include <QPushButton>

namespace SDL_UI_MODULE
{
    QFont s_varFont;

    VarShowBoard::VarShowBoard(int width) {
        this->resize(width, UI_COMPONENT_WIDTH(variable_board_height_factor));
        this->width = width;

        QVBoxLayout* vlay = new QVBoxLayout();
        m_icon = new ClickableLabel(ICOPATH(pull-down.svg),16);
        connect(m_icon,&ClickableLabel::clicked,this,&VarShowBoard::on_ShowVariables);
        m_icon->setFixedHeight(UI_COMPONENT_WIDTH(variable_board_commitBtn_width_factor));
        m_icon->setStyleSheet("background-color:rgba(240,240,240,255)");
        vlay->addWidget(m_icon,Qt::AlignLeft);

        m_showBoard = new QWidget(this);
        m_varGrid = new QGridLayout();
        m_varGrid->setContentsMargins(10,0,20,0);

        m_scrollarea = new QScrollArea(this);
        m_scrollarea->setAttribute(Qt::WA_TranslucentBackground,true);
        m_scrollarea->setWidgetResizable(true);

        int fontId = QFontDatabase::addApplicationFont(FONTPATH(Roboto-BoldItalic.ttf));
        QStringList font_list = QFontDatabase::applicationFontFamilies(fontId);
        global_font_mp["Roboto-BoldItalic"] = font_list[0];
        s_varFont.setPointSize(10);
        s_varFont.setFamily(font_list[0]);

        m_showBoard->setLayout(m_varGrid);

        vlay->addWidget(m_scrollarea);
        vlay->setContentsMargins(0,0,0,0);
        m_scrollarea->setWidget(m_showBoard);
        m_scrollarea->hide();

        m_animation = new QPropertyAnimation(m_scrollarea,"maximumHeight");
        m_animation->setEasingCurve(QEasingCurve::Linear);

        this->setLayout(vlay);
    }

    VarShowBoard::~VarShowBoard() {

    }

    void VarShowBoard::setWidth(int width) {
        this->width = width;
    }

    void VarShowBoard::setVariantMap(const QMap<QString, QVariant> &var_mp) {
        this->on_unShowVariables();

        this->m_varMap = var_mp;
        clearLayout(m_varGrid);
        int row = 0;
        for(const QString& var_name : m_varMap.keys())
        {
            QLabel* label = new QLabel(m_showBoard);
            label->setStyleSheet("QLabel{color:#E18550;}");
            label->setAttribute(Qt::WA_TranslucentBackground,true);
            label->setFont(s_varFont);
            label->setText(var_name);
            m_varGrid->addWidget(label,row,0,1,1);
            QLineEdit* editor = new QLineEdit();
            editor->setStyleSheet(R"(QLineEdit
			{
				color:white;
				border: 2px solid grey;
				border-radius:5px;
				padding: 0 0px 0 0px;
			})");
            editor->setAttribute(Qt::WA_TranslucentBackground, true);
            editor->setFont(s_varFont);
            QPalette palette = editor->palette();
            palette.setColor(QPalette::Normal, QPalette::PlaceholderText, QColor("#92938A"));
            editor->setPalette(palette);
            editor->setFixedHeight(25);
            editor->setPlaceholderText(m_varMap[var_name].toString());
            m_varGrid->addWidget(editor, row, 1, 1, 2);
            row++;
        }

        QPushButton* commitBtn = new QPushButton();
        commitBtn->setStyleSheet(WINDOWINFO_BTN_STYLE_1);
        commitBtn->setIconSize(QSize(20, 20));
        commitBtn->setIcon(QIcon(ICOPATH(commit.svg)));
        commitBtn->setFixedSize(120,22);
        commitBtn->setText(_STRING_WRAPPER("uploadVariables"));
        m_varGrid->addWidget(commitBtn, row, 1,1, 2, Qt::AlignTop | Qt::AlignRight);
        m_varGrid->update();
    }

    void VarShowBoard::clearLayout(QLayout *layout) {
        QLayoutItem* item;
        while((item = layout->takeAt(0)) != 0)
        {
            if(item->widget()){
                delete item->widget();
            }

            QLayout* childLayout = item->layout();
            if(childLayout)
            {
                clearLayout(childLayout);
            }
            delete item;
        }
    }

    void VarShowBoard::on_ShowVariables() {
        m_icon->setPixmap(QPixmap(ICOPATH(pull-up.svg)).scaled(16,16));
        disconnect(m_icon,&ClickableLabel::clicked,this,&VarShowBoard::on_ShowVariables);
        connect(m_icon,&ClickableLabel::clicked,this,&VarShowBoard::on_unShowVariables);
        m_scrollarea->show();
        m_animation->setDuration(500);
        m_animation->setStartValue(20);
        m_animation->setEndValue(200);
        m_animation->start();
        this->setFixedHeight(UI_COMPONENT_HEIGHT(variable_board_height_factor));
    }

    void VarShowBoard::on_unShowVariables() {
        m_icon->setPixmap(QPixmap(ICOPATH(pull-down.svg)).scaled(16,16));
        disconnect(m_icon,&ClickableLabel::clicked,this,&VarShowBoard::on_unShowVariables);
        connect(m_icon,&ClickableLabel::clicked,this,&VarShowBoard::on_unShowVariables);
        m_scrollarea->hide();
        this->setFixedHeight(UI_COMPONENT_WIDTH(variable_board_commitBtn_width_factor));
    }
}