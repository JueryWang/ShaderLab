/*
	ShaderLab source code ,Copyright(c) JueryWang 2023 -
	This file is part of ShaderLab

	This is free and unencumbered software released into the private domain for personal research or educational purpose.
	You cannot host, display, distribute or share this Work in any form,
	including physical and digital. You cannot use this Work in any
	commercial website or project.
*/
#ifndef UI_RADIO_VOLUME_H
#define UI_RADIO_VOLUME_H

#include <QWidget>
#include <QSlider>

class XA_UIMODULE_RadioVolume : public QWidget
{
	Q_OBJECT
public:
	XA_UIMODULE_RadioVolume();
	~XA_UIMODULE_RadioVolume();
	void setValue(int value);

private:
	void onVolumeChanged();

protected:
	void paintEvent(QPaintEvent* event) override;
	bool event(QEvent* e) override;
private:
	QSlider* _slider;
};

#endif // 
