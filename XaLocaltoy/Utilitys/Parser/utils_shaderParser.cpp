#include "parser_defaultDfs.h"
#include "utils_shaderParser.h"
#include <QString>
#include <QVariant>
#include <QDir>
#include <QRegExp>
#include <QDebug>

using namespace parser;

XA_UTILS_ShaderParser* XA_UTILS_ShaderParser::_instance = nullptr;
QString XA_UTILS_ShaderParser::cache_path;

XA_UTILS_ShaderParser::XA_UTILS_ShaderParser()
{
	this->_file_handler = new QFile();
}

XA_UTILS_ShaderParser::~XA_UTILS_ShaderParser()
{
	if (_file_handler)
		delete _file_handler;
}

bool XA_UTILS_ShaderParser::parse(const QString& source, parser::ShaderType type, bool custom_var /*= false*/, const QMap<QString, QVariant>& custom_var_mp /*= QMap<QString, QVariant>()*/)
{
	QString parse_res;
	switch (type)
	{
	case VERTEX:
		parse_res += PARSER_PREFIX_VERTEX;
		break;
	case FRAGMENT:
		parse_res += PARSER_PREFIX_FRAGMENT;
		break;
	default:
		break;
	}

	parsedVariant.clear();
	QRegExp re("#define\\s+(.*)?\\s+(.*)?\\\r\\\n");//windows
	re.setMinimal(true);
	int pos = 0;
	while ((pos = re.indexIn(source, pos)) != -1)
	{
		pos += re.matchedLength();
		if (QVariant(re.cap(2)).toFloat() && QVariant(re.cap(2)).toBool())
		{
			parsedVariant.insert(re.cap(1), QVariant(re.cap(2)));
		}
	}

	if (custom_var)
	{
		auto iter = custom_var_mp.begin();
		while (iter != custom_var_mp.end())
		{
			parsedVariant[iter.key()] = iter.value();
		}

		if (parsedVariant.size() == 0)
		{
			parser::ErrorCode::NO_DETECTED_VARS;
			return false;
		}
		//TODO:Add custom_var_mp to parse_res
	}

	pos = 0;
	re.setPattern("main(.*)?\\\n");
	re.setMinimal(true);
	QString replaced;
	if ((pos = re.indexIn(source, pos)) != -1)
	{
		replaced = re.cap(0);
	}

	if (replaced.size() == 0)
	{
		err_code = parser::ErrorCode::NO_MAIN_FUNC;
	}
	else
	{
		parse_res += source;
		parse_res.replace(replaced, "main()");
	}

	if (crt_file.size())
	{
		_file_handler->setFileName(QDir::currentPath()+"/"+cache_path+"/"+crt_file);
		if (_file_handler->open(QIODevice::WriteOnly))
		{
			_file_handler->write(parse_res.toUtf8(), parse_res.length());
		}
		else
		{
			qDebug() << _file_handler->fileName()<<"::write Error";
		}
		_file_handler->close();
		QString cmd = QString("cd bin && glslangValidator %1").arg(_file_handler->fileName());
		validator_output = _UTIL_GET_VALADITOR_RES(cmd.toLatin1().data());
		if (validator_output.size())
		{
			err_code = parser::ErrorCode::PARSE_ERROR;
		}
	}
	else
	{
		err_code = parser::ErrorCode::NO_FILE_SET;
		return false;
	}
	return true;
}

XA_UTILS_ShaderParser* XA_UTILS_ShaderParser::getParser()
{
	if (_instance == nullptr)
	{
		_instance = new XA_UTILS_ShaderParser();
	}
	return _instance;
}

void XA_UTILS_ShaderParser::setCachePath(const QString& path)
{
	QDir dir_helper;
	if (!dir_helper.exists(path))
	{
		dir_helper.mkdir(path);
	}
	XA_UTILS_ShaderParser::cache_path = path;
}

void XA_UTILS_ShaderParser::cleanCache()
{
	QDir dir(QDir::currentPath() + "/" + cache_path);
	dir.removeRecursively();
}

void XA_UTILS_ShaderParser::setContextParserRule(parser::ParserRule rule)
{
	this->crt_rule = rule;
}

void XA_UTILS_ShaderParser::setCurrentFileName(const QString& name, parser::ShaderType type)
{
	this->crt_file= name;
	this->crt_handle_type = type;
}


const QString XA_UTILS_ShaderParser::getDebugInfo() const
{
	return this->validator_output;
}

QMap<QString, QVariant> XA_UTILS_ShaderParser::getParsedVar() const
{
	return this->parsedVariant;
}

