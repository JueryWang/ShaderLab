/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef UI_PREFERNCE_WINDOW_H
#define UI_PREFERNCE_WINDOW_H

#include <QWidget>
#include <QList>
#include <QMap>
#include <QPair>
#include <QSlider>
#include <QVBoxLayout>
#include <QPushButton>

namespace preference
{
	enum ConfigType
	{
		CONFIG_SILDER,
		CONFIG_COMBOBOX,
		CONFIG_CHECKBOX,
		CONFIG_PATH
	};
}

class XA_UIModule_ConfigSlider : public QSlider
{
	Q_OBJECT
public:
	XA_UIModule_ConfigSlider(float minVal,float maxVal);

signals:
	void sendValue(float val);

public slots:
	void on_reciveValue(int val);

public:
	float minVal;
	float maxVal;

	float multiplier;
};

class XA_UIModule_ConfigItem : public QWidget
{
	Q_OBJECT
public:
	XA_UIModule_ConfigItem(const QString& title,QWidget* parent);
	void addSliderConfig(const QString& configName,float minVal,float maxVal,QObject *reciver = nullptr,void (*slot_func)(float) = nullptr);
private:
	QMap<QString, QPair<preference::ConfigType, QWidget*>> _configItems;
};

class XA_UIModule_ConfigPanel : public QWidget
{
	Q_OBJECT
public:
	XA_UIModule_ConfigPanel(QWidget* parent);

};

class XA_UIModule_PreferenceWindow : public QWidget
{
	Q_OBJECT
public:
	XA_UIModule_PreferenceWindow();
	~XA_UIModule_PreferenceWindow();
	void initConfigWgts();

public slots:
	void on_clcFrameConfigBtn();
	void on_clcThemeConfigBtn();
	void on_clcEditConfigBtn();
	void on_clcPluginConfigBtn();
	void on_clcSystemConfigBtn();
	void on_clcSaveConfigBtn();
	void on_clcFilePathConfigBtn();

private:
	QWidget* _frameConfigWgt;
	QWidget* _themeConfigWgt;
	QWidget* _editConfigWgt;
	QWidget* _pluginConfigWgt;
	QWidget* _systemConfigWgt;
	QWidget* _saveConfigWgt;
	QWidget* _filePathConfigWgt;

	QList<QPushButton*> _buttons;
	QVBoxLayout* _configLayout;
	QWidget* _curConfigWgt;
	bool first_btn_style_reset = false;
};

#endif // 
