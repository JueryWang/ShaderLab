#include "EditorPage.h"
#include "CodeEditor.h"
#include "UI_DefaultDfs.h"
#include "OverallWindow.h"
#include "AssetWindow.h"
#include <QApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFile>

QStringList getIntellisense(const QString& intellisenseFile);
bool font_inited = false;
QFont font_global;

#include <QDebug>

namespace SDL_UI_MODULE {
    QsciLexerCPP* EditorPage::s_cpplexer;
    QsciAPIs* EditorPage::s_api;

    SDL_UI_MODULE::EditorPage::EditorPage() {
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

        this->setEolMode(QsciScintilla::EolWindows);
        this->setEolVisibility(false);

        this->setLexer(EditorPage::s_cpplexer);

		this->setMarginType(0, QsciScintilla::NumberMargin);
		this->setMarginWidth(0, "0000");
		this->setMarginsForegroundColor(QColor("#F8A959"));
		this->setMarginsBackgroundColor(QColor("#80838A"));
		QFont FiraSans(FONTPATH(FiraSans - Book.otf));
		this->setMarginsFont(FiraSans);

        m_shader.reset(new SDL_GL_MODULE::SDL_SHADER_INFO());
        m_attachedAssetBar = new WindowAssetBar(this);

        connect(this, &EditorPage::cursorPositionChanged, this, []()
            {

            });
    }

    SDL_UI_MODULE::EditorPage::~EditorPage() {
        m_shader->should_clear = true;
        m_shader.reset();
    }

    void SDL_UI_MODULE::EditorPage::initFont(const QString &font_name, int size) {
        font_global.setFamily(font_name);
        font_global.setPointSize(size);
    }

    void SDL_UI_MODULE::EditorPage::initIntellisense() {
        s_cpplexer = new QsciLexerCPP;
        s_cpplexer->setDefaultFont(font_global);
        s_api = new QsciAPIs(s_cpplexer);
        QStringList buildin_keys = getIntellisense(EDITOR_LANG_CONFIG_PATH(glsl-intellisense.json));
        for(const QString& key : buildin_keys)
        {
            s_api->add(key);
        }
        s_api->prepare();
    }

    void SDL_UI_MODULE::EditorPage::keyPressEvent(QKeyEvent *e) {
        return QsciScintilla::keyPressEvent(e);
    }

    void SDL_UI_MODULE::EditorPage::dragEnterEvent(QDragEnterEvent *event) {
        return CodeEditor::getEditor()->dragEnterEvent(event);
    }

    void SDL_UI_MODULE::EditorPage::dropEvent(QDropEvent *event) {
        return  CodeEditor::getEditor()->dropEvent(event);
    }

    void SDL_UI_MODULE::EditorPage::setCustomStyle() {
        this->setStyleSheet(EDITOR_PAGE_STYLE);
    }
}

QStringList getIntellisense(const QString& intellisenseFile)
{
    QStringList res;
    QFile file(intellisenseFile);
    if(!file.open(QIODevice::ReadOnly))
    {
        return res;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Encoding::Utf8);
    QString str = stream.readAll();
    file.close();

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(),&jsonError);
    if(jsonError.error != QJsonParseError::NoError && !doc.isNull())
    {

    }

    QJsonObject rootObj = doc.object();
    QStringList kwords;

    static QStringList type_to_parse = { "c-keyword","data-type","qualifier","glsl-buildin-vertex",
                                         "glsl-buildin-vertex","glsl-buildin-fragment",
                                         "glsl-modifier","api" };
    for(int i = 0;i < type_to_parse.size(); i++)
    {
        QJsonValue array = rootObj.value(type_to_parse[i]);

        if(array.type() == QJsonValue::Array){
            QJsonArray arrayVal = array.toArray();
            int arrsize = arrayVal.size();
            for(int j = 0; j < arrsize;j++)
            {
                QJsonValue key = arrayVal.at(j);
                res.append(key.toString());
            }
        }
    }
}