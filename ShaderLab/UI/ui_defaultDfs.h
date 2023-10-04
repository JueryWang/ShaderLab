/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef UI_DEFAULT_DFS
#define UI_DEFAULT_DFS

#include <QString>
#include <QMap>
#define ICOPATH(x) "Resources/icon/"#x
#define AVTRPATH(x) "Resources/user/avatar/"#x
#define FONTPATH(x) "Resources/font/"#x
#define EDITOR_LANG_CONFIG_PATH(x) "Resources/editor/intellisense/"#x
#define SHADOW_WIDTH 10	

enum MsgBoxIcon {
	NoIcon = 0,
	Information = 1,
	Warning = 2,
	Critical = 3,
	Question = 4
};

const static QString EDITOR_PAGE_STYLE = R"(QListView{
	background-color: #75A4CD;
	border-radius:5px;
	border:1px solid #D3D3D3;
	padding:5px;
	color:#1F282F;
}

QScrollBar:vertical
{
	border: none;
	width: 14px;
	margin: 0px 0 0px 0;
	background-color: wheat;
	border-radius: 0px;
}

QScrollBar::handle:vertical{
	background-color: rgba(49,95,120,100);
	margin: 0 0px 0 0px;
	border-radius: 6px;
}

QScrollBar::handle:vertical:pressed{
	background-color:rgba(110,148,189,100);
}

QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{
	background:none;
}

QScrollBar:horizontal
{
	border: none;
	width: 14px;
	margin: 0px 0 0px 0;
	background-color: wheat;
	border-radius: 0px;
}

QScrollBar::handle:horizontal{
	background-color: rgba(49,95,120,100);
	margin: 0 0px 0 0px;
	border-radius: 6px;
}

QScrollBar::handle:horizontal:pressed{
	background-color: #5c5b5b;
}

QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal{
	background:none;
})";
const static QString WINDOW_FONT_COLOR_STYLE = "QLabel{background:transparent;color:#eeeed4;}";

const static QString WINDOWINFO_BTN_STYLE_1 = R"(.QPushButton{
		border-style:none;
		border:1px solid #2A2C32;
		color:#FFFFFE;
		padding:5px;
		min-height:15px;
		border-radius:5px;
		background:#383A42;
		}

		.QPushButton:hover{
		background:#9698a1;
})";

const static QString WINDOWINFO_BTN_STYLE_2 = R"(.QPushButton{
		border-style:none;
		color:#FFFFFE;
		padding:5px;
		min-height:15px;
		background:transparent;
		}

		.QPushButton:hover{
		border:2px solid #9C9EA3;
		background:transparent;
		border-radius:5px;
})";

const static QString WINDOW_ASEETBAR_BTN_STYLE = R"(.QPushButton{
		border-style:none;
		color:#FFFFFE;
		padding:5px;
		min-height:1px;
		background:transparent;
		}

		.QPushButton:hover{
		border:2px solid #9C9EA3;
		background:transparent;
		border-radius:5px;
})";

const static QString PREFERENCE_WINDOW_STYLE = R"(QWidget{
		border-color: rgb(99, 136, 148);
		background-color: rgba(24, 24, 24, 230);
	})";

const static QString PREFERENCE_COLUMBTN_STYLE_FIRST = R"(.QPushButton{
		border-style:none;
		border-top-left-radius: 3px;
		border-top-right-radius: 3px;
		color:#FFFFFE;
		padding-left:8px;
		background:#545454;
		}
		.QPushButton:hover{
		border:none;
		padding-left:8px;
		background:rgb(71,114,179);
})";

const static QString PREFERENCE_COLUMBTN_STYLE_LAST = R"(.QPushButton{
		border-style:none;
		border-bottom-left-radius: 3px;
		border-bottom-right-radius: 3px;
		color:#FFFFFE;
		padding-left:8px;
		background:#545454;
		}
		.QPushButton:hover{
		border:none;
		padding-left:8px;
		background:rgb(71,114,179);
})";

const static QString PREFERENCE_COLUMBTN_STYLE_MEDIUM = R"(.QPushButton{
		border-style:none;
		color:#FFFFFE;
		padding-left:8px;
		background:#545454;
		}
		.QPushButton:hover{
		border:none;
		padding-left:8px;
		background:rgb(71,114,179);
})";

const static QString TABLABLE_EDITOR_STYLE = R"(
* {
	outline: none;
}

QLineEdit {
	border: 1px solid #A0A0A0;
	border-radius: 3px;
	padding-left: 5px;
	background-color: #F2F2F2;
	color: #A0A0A0;
	selection-background-color: #A0A0A0;
	selection-color: #2D2D2D;
	font-family: "Microsoft YaHei";
	font-size: 10pt;
}

QLineEdit:hover {
	border: 1px solid #298DFF;
	border-radius: 3px;
	background-color: #F2F2F2;
	color: #2D2D2D;
	selection-background-color: #298DFF;
	selection-color: #F2F2F2;
}

QLineEdit[echoMode="2"] {
	lineedit-password-character: 9679;
	lineedit-password-mask-delay: 2000;
}

QLineEdit:disabled {
	border: 1px solid #CDCDCD;
	background-color: #CDCDCD;
	color: #B4B4B4;
}

QLineEdit:read-only {
	background-color: #CDCDCD;
	color: #2D2D2D;
}
)";

const static QString TYPES_LISTVIEW_STYLE = R"(
QListView{
    background-color: white;
    border-radius:5px;
    border:1px solid #D3D3D3;
    padding:5px;
    color:#1F282F;
};
QListView::item{width:80px};
)";

const static QString TYPES_MENU_STYLE = R"(

QMenu {
	color:black;
	background-color:white; 
	border-radius:3px;
	padding:5px;
	margin:6px;
}
QMenu::item:text { 
	padding-left:10px;
	padding-right:10px;
}
QMenu::item:selected{ 
	color:#1aa3ff;
	background-color: #e5f5ff;
	border-radius:3px;
}
QMenu::separator{
	height:1px;
	background:#bbbbbb;
	margin:5px;
	margin-left:10px;
	margin-right:10px;
}
)";
extern QMap<QString, QString> global_font_mp;
#endif // UI_DEFAULT_DFS\
