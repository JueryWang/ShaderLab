#include "uimodule_editorpage.h"
#include "ui_defaultDfs.h"
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>

//For json parse
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
#include <QFile>

QStringList getIntellisense(const QString& intelliSenseFile);

QsciLexerCPP* XA_UIMODULE_EditorPage::_cpplexer;
QsciAPIs* XA_UIMODULE_EditorPage::_api;
bool font_inited = false;
QFont font_global;

XA_UIMODULE_EditorPage::XA_UIMODULE_EditorPage()
{
	this->setUtf8(true);
	this->setFont(font_global);
	this->setCustomStyle();
	this->setBraceMatching(QsciScintilla::SloppyBraceMatch);

	this->setTabWidth(4);
	this->setIndentationsUseTabs(false);
	this->setAutoIndent(true);

	//autocomplete
	this->setAutoCompletionSource(QsciScintilla::AcsAll);
	this->setAutoCompletionThreshold(1);
	this->setAutoCompletionCaseSensitivity(false);
	this->setAutoCompletionUseSingle(QsciScintilla::AcusNever);


	//caret settings
	this->setCaretForegroundColor(QColor("#dcdcdc"));
	this->setCaretLineVisible(true);
	this->setCaretWidth(2);
	this->setCaretLineBackgroundColor(QColor(242, 161, 141, 55));
	
	//EOL
	this->setEolMode(QsciScintilla::EolWindows);
	this->setEolVisibility(false);

	this->setLexer(XA_UIMODULE_EditorPage::_cpplexer);

	//line Margins
	this->setMarginType(0, QsciScintilla::NumberMargin);
	this->setMarginWidth(0, "0000");
	this->setMarginsForegroundColor(QColor("#F8A959"));
	this->setMarginsBackgroundColor(QColor("#80838A"));
	QFont FiraSans(FONTPATH(FiraSans-Book.otf));
	this->setMarginsFont(FiraSans);
}

XA_UIMODULE_EditorPage::~XA_UIMODULE_EditorPage()
{

}

void XA_UIMODULE_EditorPage::initFont(const QString& font_name,int size)
{
	if (!font_inited)
	{
		font_global.setFamily(font_name);
		font_global.setPointSize(size);
	}
}

void XA_UIMODULE_EditorPage::initIntellisense()
{
	XA_UIMODULE_EditorPage::_cpplexer = new QsciLexerCPP();
	XA_UIMODULE_EditorPage::_cpplexer->setDefaultFont(font_global);
	XA_UIMODULE_EditorPage::_api = new QsciAPIs(XA_UIMODULE_EditorPage::_cpplexer);
	QStringList buildin_keys = getIntellisense(EDITOR_LANG_CONFIG_PATH(glsl-intellisense.json));
	for (const QString& key : buildin_keys)
	{
		XA_UIMODULE_EditorPage::_api->add(key);
	}
	XA_UIMODULE_EditorPage::_api->prepare();
}

void XA_UIMODULE_EditorPage::keyPressEvent(QKeyEvent* e)
{
	return QsciScintilla::keyPressEvent(e);
}

QStringList getIntellisense(const QString& intelliSenseFile)
{
	QStringList res;
	
	QFile file(intelliSenseFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		//Do some log here : json Load error
		return res;
	}

	QTextStream stream(&file);
	stream.setCodec("UTF8");
	QString str = stream.readAll();
	file.close();

	QJsonParseError jsonError;
	QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &jsonError);
	if (jsonError.error != QJsonParseError::NoError && !doc.isNull())
	{
		//Do some log here : json format error
	}

	QJsonObject rootObj = doc.object();
	QStringList kwords;

	static QStringList type_to_parse = { "c-keyword","data-type","glsl-buildin-vertex",
						   "glsl-buildin-vertex","glsl-buildin-fragment",
						   "glsl-modifier","api" };
	for (int i = 0; i < type_to_parse.size(); i++)
	{
		QJsonValue array = rootObj.value(type_to_parse[i]);

		if (array.type() == QJsonValue::Array) {
			QJsonArray arrayVal = array.toArray();
			int arrsize = arrayVal.size();
			for (int i = 0; i < arrsize; i++)
			{
				QJsonValue key = arrayVal.at(i);
				res.append(key.toString());
			}
		}
	}

	return res;
}

void XA_UIMODULE_EditorPage::setCustomStyle()
{
	this->setStyleSheet(R"(QListView{
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
		})");
}