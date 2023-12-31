#ifndef XA_UTILS_SHADERPARSER_H
#define XA_UTILS_SHADERPARSER_H

/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#include "parser_defaultDfs.h"
#include <QObject>
#include <QFileInfo>
#include <QFile>
#include <QPair>
#include <QMap>

class XA_UTILS_ShaderParser : public QObject
{
	Q_OBJECT
public:
	static XA_UTILS_ShaderParser* getParser();
	static void setCachePath(const char* path);
	static void cleanCache();
	void setContextParserRule(parser::ParserRule rule);
	void setCurrentFileName(const QString& name, parser::ShaderType type);
	void setComboParse(bool flag);
	bool parse(const QString& source, parser::ShaderType type, bool custom_var = false, const QMap<QString, QVariant>& custom_var_mp = QMap<QString, QVariant>());
	const QString getDebugInfo() const;
	QMap<QString, QVariant> getParsedVar() const;
	QString getCurrentFilePath();

private:
	~XA_UTILS_ShaderParser();
	XA_UTILS_ShaderParser();

	parser::ParserRule crt_rule = parser::ShaderToy;
	parser::ErrorCode err_code = parser::ErrorCode::NO_ERROR;
	bool combo_parse;
	QMap<QString, QVariant> parsedVariant;


	QFile* _file_handler = nullptr;
	parser::ShaderType crt_handle_type;
	QString crt_file;

	static XA_UTILS_ShaderParser* _instance;
	static const char* cache_path;
	QString validator_output;
};
#endif // XA_UTILS_SHADERPARSER_H