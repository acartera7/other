//
// Created by Andrei on 5/4/2025.
//
#include <Audioclient.h>
#include <Mmdeviceapi.h>
#include <QThread>

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

class AudioPlayer : public QObject {
    Q_OBJECT
public:
    explicit AudioPlayer(QObject *parent = nullptr);
    void play(const QString &filePath);
    void stop();
    void setVolume(float volume);

private:
    void initializeWasapi();
    //void loadAudioFile(const QString &filePath);
};


#endif //AUDIOPLAYER_H
