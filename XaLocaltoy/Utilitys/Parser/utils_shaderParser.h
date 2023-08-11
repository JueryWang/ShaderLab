#ifndef XA_UTILS_SHADERPARSER_H
#define XA_UTILS_SHADERPARSER_H

#include <QObject>
#include <QFile>
#include <QPair>
#include <QMap>

namespace parser
{
	enum ParserRule
	{
		ShaderToy = 0,
		DefaultGLSL
	};

	enum ShaderType
	{
		VERTEX = 0,
		TES_CONTROL,
		TES_EVAL,
		GEOMETRY,
		FRAGMENT
	};

	enum VariantType
	{
		INT = 0,
		FLOAT,
		VEC2,
		VEC3,
		VEC4,
	};

	enum ErrorCode
	{
		PARSE_ERROR = 0,
		NO_FILE_SET,
		NO_MAIN_FUNC,
		NO_DETECTED_VARS,
		NO_ERROR
	};
}

class XA_UTILS_ShaderParser : public QObject
{
	Q_OBJECT
public:
	static XA_UTILS_ShaderParser* getParser();
	static void setCachePath(const QString &path);
	static void cleanCache();
	void setContextParserRule(parser::ParserRule rule);
	void setCurrentFileName(const QString& name, parser::ShaderType type);
	bool parse(const QString& source, parser::ShaderType type, bool custom_var = false, const QMap<QString, QVariant>& custom_var_mp = QMap<QString, QVariant>());
	const QString getDebugInfo() const;
	QMap<QString, QVariant> getParsedVar() const;

private:
	~XA_UTILS_ShaderParser();
	XA_UTILS_ShaderParser();

	parser::ParserRule crt_rule = parser::ShaderToy;
	parser::ErrorCode err_code = parser::ErrorCode::NO_ERROR;
	QMap<QString, QVariant> parsedVariant;


	QFile* _file_handler;
	parser::ShaderType crt_handle_type;
	QString crt_file;

	static XA_UTILS_ShaderParser* _instance;
	static QString cache_path;
	QString validator_output;
};
#endif // XA_UTILS_SHADERPARSER_H