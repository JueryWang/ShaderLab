#include "uimodule_preferenceWindow.h"
#include "../Utilitys/uitilityDfs.h"
#include "ui_defaultDfs.h"
#include <QHBoxLayout>
#include <QApplication>
#include <QHoverEvent>
#include <QIcon>
#include <QDebug>

XA_UIModule_PreferenceWindow::XA_UIModule_PreferenceWindow()
{
	this->setWindowIcon(QIcon("Resources/icon/ShaderLabIco.png"));
	this->setWindowTitle(_STRING_WRAPPER("ShaderLab Preferences"));
	this->setStyleSheet(PREFERENCE_WINDOW_STYLE);
	this->setFixedSize(660, 550);
	
	initConfigWgts();

	QHBoxLayout* overallLayout = new QHBoxLayout(this);
	overallLayout->setContentsMargins(0, 0, 0, 0);
	QWidget *leftBox = new QWidget(this);
	leftBox->setFixedWidth(160);
	
	QStringList columNames = { "Frames","Themes","Editing","Plugin","System","Save & Load","File Paths" };
	typedef void (XA_UIModule_PreferenceWindow::* slot_func)(void);
	QList<slot_func> slot_functions = {&XA_UIModule_PreferenceWindow ::on_clcFrameConfigBtn,
										&XA_UIModule_PreferenceWindow::on_clcThemeConfigBtn,
										&XA_UIModule_PreferenceWindow::on_clcEditConfigBtn,
										&XA_UIModule_PreferenceWindow::on_clcPluginConfigBtn,
										&XA_UIModule_PreferenceWindow::on_clcSystemConfigBtn,
										&XA_UIModule_PreferenceWindow::on_clcSaveConfigBtn,
										&XA_UIModule_PreferenceWindow::on_clcFilePathConfigBtn};

	QVBoxLayout* leftBoxLayout = new QVBoxLayout(this);
	leftBoxLayout->setContentsMargins(8, 12, 8, 8);
	leftBoxLayout->setSpacing(0);

	int columnlen = columNames.length();
	for (int i = 0; i < columnlen;i++)
	{
		QPushButton* columnBtn = new QPushButton();
		columnBtn->setFixedHeight(24);
		if (i == 0)
		{
			//set first button to hover color
			columnBtn->setStyleSheet(R"(.QPushButton{
			border-style:none;
			border-top-left-radius: 3px;
			border-top-right-radius: 3px;
			color:#FFFFFE;
			padding-left:8px;
			background:rgb(71,114,179);
			})");
		}
		else if (i == columnlen - 1)
			columnBtn->setStyleSheet(PREFERENCE_COLUMBTN_STYLE_LAST);
		else
			columnBtn->setStyleSheet(PREFERENCE_COLUMBTN_STYLE_MEDIUM);

		columnBtn->setText(columNames[i]);
		columnBtn->setFixedWidth(140);
		connect(columnBtn, &QPushButton::clicked, this, slot_functions[i]);
		leftBoxLayout->addWidget(columnBtn);
		leftBoxLayout->addSpacing(1);
		_buttons.append(columnBtn);
	}
	leftBoxLayout->addWidget(new QWidget(this));
	leftBox->setLayout(leftBoxLayout);

	_configLayout = new QVBoxLayout();
	_configLayout->setContentsMargins(0, 0, 0, 0);
	_configLayout->addWidget(_frameConfigWgt);
	
	overallLayout->addWidget(leftBox);
	overallLayout->addLayout(_configLayout);
	this->setLayout(overallLayout);
}

XA_UIModule_PreferenceWindow::~XA_UIModule_PreferenceWindow()
{

}

void XA_UIModule_PreferenceWindow::initConfigWgts()
{
	_frameConfigWgt = new QWidget(this);
	_themeConfigWgt = new QWidget(this);
	_editConfigWgt = new QWidget(this);
	_pluginConfigWgt = new QWidget(this);
	_systemConfigWgt = new QWidget(this);
	_saveConfigWgt = new QWidget(this);
	_filePathConfigWgt = new QWidget(this);
}

void XA_UIModule_PreferenceWindow::on_clcFrameConfigBtn()
{
	
}

void XA_UIModule_PreferenceWindow::on_clcThemeConfigBtn()
{
	
}

void XA_UIModule_PreferenceWindow::on_clcEditConfigBtn()
{

}

void XA_UIModule_PreferenceWindow::on_clcPluginConfigBtn()
{

}

void XA_UIModule_PreferenceWindow::on_clcSystemConfigBtn()
{

}

void XA_UIModule_PreferenceWindow::on_clcSaveConfigBtn()
{

}

void XA_UIModule_PreferenceWindow::on_clcFilePathConfigBtn()
{

}

XA_UIModule_ConfigSlider::XA_UIModule_ConfigSlider(float minVal, float maxVal):minVal(minVal),maxVal(maxVal)
{
	this->setRange(0, 100);
	multiplier = (maxVal - minVal)/100;
	connect(this, &XA_UIModule_ConfigSlider::valueChanged, this, &XA_UIModule_ConfigSlider::on_reciveValue);
}

void XA_UIModule_ConfigSlider::on_reciveValue(int val)
{
	float sendVal = minVal+multiplier * val;
	emit sendValue(sendVal);
}

void XA_UIModule_ConfigItem::addSliderConfig(const QString& configName, float minVal, float maxVal, QObject* reciver, void (*slot_func)(float))
{
	XA_UIModule_ConfigSlider* slider = new XA_UIModule_ConfigSlider(minVal, maxVal);
	connect(slider, &XA_UIModule_ConfigSlider::sendValue, reciver, slot_func);
}
