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
