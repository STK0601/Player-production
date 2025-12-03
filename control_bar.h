#ifndef CONTROL_BAR_H
#define CONTROL_BAR_H

#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

class ThePlayer;

class ControlBar : public QWidget {
    Q_OBJECT
public:
    explicit ControlBar(ThePlayer* player, QWidget *parent = nullptr);
    void setControlSizes(bool isMobile);
private:
    ThePlayer* player;
    QPushButton* playPauseBtn;
    QSlider* progressBar;
    QSlider* volumeSlider;
    QPushButton* fullscreenBtn;
    QComboBox* speedCombo;
    QLabel* timeLabel;

private slots:
    void keyPressEvent(QKeyEvent *event) override;
    void onPlayPauseClicked();
    void onProgressChanged(int value);
    void onVolumeChanged(int value);
    void onFullscreenClicked();
    void onSpeedChanged(int index);
    void updateProgress(qint64 position);
    void updateDuration(qint64 duration);
};


#endif // CONTROL_BAR_H
