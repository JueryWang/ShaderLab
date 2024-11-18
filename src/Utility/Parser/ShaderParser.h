#ifndef SHADERPARSER_H
#define SHADERPARSER_H

#include "ShaderParser_Dfs.h"
#include <QObject>
#include <QFileInfo>
#include <QFile>
#include <QPair>
#include <QMap>

namespace SDL_UTILS {

    class ShaderParser : QObject{
        Q_OBJECT
    public:
        static ShaderParser* getParser();
        static void setCachePath(const char* path);
        static void clearCache();
        void setContextParserRule(ParserRule rule);
        void setCurrentFileName(const QString& name,ShaderType type);
        void setComboParse(bool flag);
        bool parse(const QString& source,ShaderType type,bool customVar = false,const QMap<QString,QVariant>& customVarMp = QMap<QString,QVariant>());
        const QString getDebugInfo() const;
        QMap<QString,QVariant> getParsedVar() const;
        QString getCurrenFilePath();
    private:
        ShaderParser();
        ~ShaderParser();

        ParserRule m_rule = ParserRule::ShaderToy;
        ErrorCode m_errCode = ErrorCode::NO_ERROR;
        bool combo_parse;
        QMap<QString,QVariant> m_parsedVariant;

        QFile m_fileHandler;
        ShaderType m_currentType;
        QString m_handledFile;

        static ShaderParser* s_instance;
        static const char* s_cachedPath;
        QString m_validatorOutput;
    };
}

#endif
