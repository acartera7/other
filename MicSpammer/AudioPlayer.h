//
// Created by Andrei on 5/4/2025.
//
#pragma once
#include <QWidget>
#include <QFile>
#include <QDebug>
#include <QThread>
#include <QJsonObject>

#include <vector>
#include <cstring>

#include "SoundInstance.h"
#include "WasapiManager.h"

#define NO_DSHOW_STRSAFE

#include <Mmdeviceapi.h>
#include <Audioclient.h>



#undef TIMECODE_SAMPLE

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H


//struct LoadedWav {
//    std::vector<BYTE> data;
//    WAVEFORMATEX format;
//    bool ok = false;
//};

class AudioPlayer : public QObject {
    Q_OBJECT
public:
    static AudioPlayer& getInstance();

    AudioPlayer(const AudioPlayer&) = delete;
    AudioPlayer& operator=(const AudioPlayer&) = delete;

    void play(const QString &filePath);
    void stopAll();
    void setMonitorVolume(float volume);
    void setOutputVolume(float volume);

    void setMonitorDevice(QString id);
    void setOutputDevice(QString id);

    // session persistence
    QJsonObject saveState();
    void loadState(QJsonObject);

private slots:
    void onInstanceFinished(SoundInstance* instance);

private:
    explicit AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer() override;

    void releaseMonitorDevice();
    void releaseOutputDevice();

    IMMDevice* monitorDevice = nullptr;
    IMMDevice* outputDevice = nullptr;

    std::vector<SoundInstance*> activeInstances;      // keep track of threads playing sound
    float _monitorVolume;
    float _outputVolume;
};


#endif //AUDIOPLAYER_H
