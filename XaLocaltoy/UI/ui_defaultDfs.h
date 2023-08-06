#ifndef UI_DEFAULT_DFS
#define UI_DEFAULT_DFS

#include <QString>
#define ICOPATH(x) "Resources/icon/"#x
#define AVTRPATH(x) "Resources/user/avatar/"#x
#define FONTPATH(x) "Resources/font/"#x
#define EDITOR_LANG_CONFIG_PATH(x) "Resources/editor/intellisense/"#x

enum MsgBoxIcon {
	NoIcon = 0,
	Information = 1,
	Warning = 2,
	Critical = 3,
	Question = 4
};

const QString EDITOR_PAGE_STYLE = R"(QListView{
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

const QString WINDOWINFO_BTN_STYLE = R"(.QPushButton{
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


#endif // UI_DEFAULT_DFS\
