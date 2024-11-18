#ifndef RADIO_VOLUME_CONDITIONER_H
#define RADIO_VOLUME_CONDITIONER_H

#include <QWidget>
#include <QSlider>

namespace SDL_UI_MODULE
{
    class RadioConditioner : public QWidget{
        Q_OBJECT
    public:
        RadioConditioner();
        ~RadioConditioner();
        void setValue(int value);

    public slots:
        void onVolumeChanged();

    protected:
        void paintEvent(QPaintEvent* event) override;
        bool event(QEvent* e) override;
    private:
        QSlider* m_slider;
    };
}

#endif
