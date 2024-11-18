#include "ShaderParser.h"
#include <QString>
#include <QVariant>
#include <QDir>
#include <QRegularExpression>
#include <QDebug>

namespace SDL_UTILS {

    ShaderParser* ShaderParser::s_instance = nullptr;
    const char* ShaderParser::s_cachedPath;

    SDL_UTILS::ShaderParser *SDL_UTILS::ShaderParser::getParser() {
        if(ShaderParser::s_instance == nullptr)
        {
            ShaderParser::s_instance = new ShaderParser();
        }
        return s_instance;
    }

    void SDL_UTILS::ShaderParser::setCachePath(const char *path) {
        QDir dir;
        if(!dir.exists(path))
        {
            dir.mkdir(path);
        }
        ShaderParser::s_cachedPath = path;
    }

    void SDL_UTILS::ShaderParser::clearCache() {
        QDir dir(QDir::currentPath() + "/" + s_cachedPath);
        dir.removeRecursively();
    }

    void SDL_UTILS::ShaderParser::setContextParserRule(ParserRule rule) {
        m_rule = rule;
    }

    void SDL_UTILS::ShaderParser::setCurrentFileName(const QString &name, SDL_UTILS::ShaderType type) {
        this->m_handledFile = name;
        this->m_currentType = type;
    }

    void SDL_UTILS::ShaderParser::setComboParse(bool flag) {
        combo_parse = flag;
    }

    bool SDL_UTILS::ShaderParser::parse(const QString &source, SDL_UTILS::ShaderType type, bool customVar,
                                        const QMap<QString, QVariant> &customVarMp) {
        QString parse_res;
        switch (type) {
            case VERTEX:
                parse_res += PARSER_PREFIX_VERTEX;
                break;
            case FRAGMENT:
                parse_res += PARSER_PREFIX_FRAGMENT;
                break;
            default:
                break;
        }

        if(!combo_parse) m_parsedVariant.clear();

        QRegularExpression re("#define\\s+\\b(.*)?\\b\\s+\\b(.*)?\\b");

        re.setPatternOptions(QRegularExpression::NoPatternOption);
        QRegularExpressionMatchIterator iterator = re.globalMatch(source,0,QRegularExpression::NormalMatch,QRegularExpression::NoMatchOption);
        while(iterator.hasNext())
        {
            QRegularExpressionMatch match = iterator.next();
            m_parsedVariant[match.captured(1)] = match.captured(2);
        }

        if (customVar)
        {
            auto iter = customVarMp.begin();
            while (iter != customVarMp.end())
            {
                m_parsedVariant[iter.key()] = iter.value();
            }

            if (m_parsedVariant.size() == 0)
            {
                m_errCode = ErrorCode::NO_DETECTED_VARS;
                return false;
            }
        }

        QString replaced;
        parse_res += source;

        re.setPattern("main(.*)?");
        iterator = re.globalMatch(parse_res, 0, QRegularExpression::NormalMatch, QRegularExpression::NoMatchOption);
        if (iterator.hasNext())
        {
            QRegularExpressionMatch match = iterator.next();
            QString replaced = match.captured(0);
            if (replaced.size() == 0)
            {
                m_errCode = ErrorCode::NO_MAIN_FUNC;
            }
            else
            {
                parse_res.replace(replaced, "main()");
            }
        }

        if (m_handledFile.size())
        {
            m_fileHandler.setFileName(QDir::currentPath() + "/" + s_cachedPath + "/" + m_handledFile);
            if (m_fileHandler.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                m_fileHandler.write(parse_res.toUtf8().constData(), parse_res.toUtf8().size());
                m_fileHandler.close();
            }
            else
            {
                //Do some log here
            }

            m_validatorOutput = GetShaderParserResult(m_fileHandler.fileName());
            if (m_validatorOutput.size())
            {
                m_errCode = ErrorCode::PARSE_ERROR;
            }
        }
        else
        {
            m_errCode = ErrorCode::NO_FILE_SET;
        }

        return false;
    }

    const QString SDL_UTILS::ShaderParser::getDebugInfo() const {
        return m_validatorOutput;
    }

    QMap<QString, QVariant> SDL_UTILS::ShaderParser::getParsedVar() const {
        return m_parsedVariant;
    }

    QString SDL_UTILS::ShaderParser::getCurrenFilePath() {
        return m_fileHandler.fileName();
    }

    SDL_UTILS::ShaderParser::ShaderParser() {
    }

    SDL_UTILS::ShaderParser::~ShaderParser() {
        clearCache();
    }
}