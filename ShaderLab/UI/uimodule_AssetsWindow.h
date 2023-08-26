#ifndef UI_ASSETS_WINDOW_H
#define UI_ASSETS_WINDOW_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QList>

class XA_GLMODULE_RENDER;

class ASSET_WINDOW : public QWidget
{
public:
	ASSET_WINDOW(const QSize& size);
protected:
	virtual void paintEvent(QPaintEvent* event);
public:
	enum AssetType
	{
		NONE = 0,
		IMAGE,
		AUDIO
	}asset_type;

	QLabel cross;
	QString asset_path;
	QImage show_img;
	QRect cross_rect;
};

class XA_UIMODULE_ASSET_WINDOW : public QWidget
{
	Q_OBJECT
public:
	XA_UIMODULE_ASSET_WINDOW(int index);
	void setAudioPlayDone();
protected:
	virtual bool eventFilter(QObject* obj, QEvent* event);
private:
	void sendAssets(ASSET_WINDOW::AssetType type);
	void setupAudioSets();
	void unsetupAudioSets();
public slots:
	void on_clcAudioPause();
	void on_clcAudioResume();
	void on_clcAudioRewind();
	void on_clcAudioVolume();
	void on_clcAudioSetting();

private:
	QVBoxLayout* _vlay;
	QHBoxLayout* _audioSetsLay;
	ASSET_WINDOW* _window;
	QWidget* _audioToolbar;
	QLabel* _label;
	QPushButton* pauseBtn;
	QPushButton* volumeBtn;
	QPushButton* rewindBtn;
	QPushButton* settingBtn;
	QSlider* _auVolumeSlider;

	int _index;
	bool opened_asset = false;
	bool playingAudio = false;//only play one audio

	QImage show_img;
};

class XA_UIMODULE_ASSET_BAR : public QScrollArea
{
	friend class XA_UIMODULE_ASSET_WINDOW;
public:
	XA_UIMODULE_ASSET_BAR(int width);
	static void setAssetsReciver(XA_GLMODULE_RENDER* reciver);

private:
	QList<XA_UIMODULE_ASSET_WINDOW*> asset_win_list;
	static XA_GLMODULE_RENDER* _glReciver;
};
#endif
