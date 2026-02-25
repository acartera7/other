//
// Created by Andrei on 5/4/2025.
//
#pragma once
#include <QWidget>
#include <QFile>

#include <QDebug>
#include <QThread>

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
    void setVolume(float volume);

    void setMonitorDevice(const std::wstring& id);
    void setOutputDevice(const std::wstring& id);

private slots:
    void onInstanceFinished(SoundInstance* instance);

private:
    explicit AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer() override;

    void releaseMonitorClient();
    void releaseOutputClient();
    void initMonitorClient(IMMDevice* device);
    void initOutputClient(IMMDevice* device);

    IMMDevice* monitorDevice = nullptr;
    IMMDevice* outputDevice = nullptr;

    IAudioClient* monitorAudioClient = nullptr;
    IAudioRenderClient* monitorRenderClient = nullptr;

    IAudioClient* outputAudioClient = nullptr;
    IAudioRenderClient* outputRenderClient = nullptr;

    std::vector<SoundInstance*> activeInstances;      // keep track of threads playing sound
    float _volume;
};


#endif //AUDIOPLAYER_H
