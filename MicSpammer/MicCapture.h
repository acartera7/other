//
// Created by Victus on 2/17/2026.
//
#pragma once

#ifndef MICSPAMMER_MICCAPTURE_H
#define MICSPAMMER_MICCAPTURE_H
#include <QObject>
#include <QThread>
#include <QJsonObject>

#include <atomic>
#include <audioclient.h>
#include <mmdeviceapi.h>

class MicCapture : public QObject {
    Q_OBJECT
public:
    static MicCapture& getInstance();

    MicCapture(const MicCapture&) = delete;
    MicCapture& operator=(const MicCapture&) = delete;

    void setInputDevice(QString id);
    void setOutputDevice(QString id);
    void setVolume(float volume);
    void start();
    void stop();

    // session persistence
    QJsonObject saveState();
    void loadState(QJsonObject);

signals:
    void micDataReady(const QByteArray &pcm);

private:
    explicit MicCapture(QObject *parent = nullptr);
    ~MicCapture() override;

    void captureLoop();
    void releaseInputClient();
    void releaseOutputClient();
    void initInputClient();
    void initOutputClient();

    IMMDevice* inputDevice = nullptr;
    IMMDevice* outputDevice = nullptr;

    IAudioClient* inputAudioClient = nullptr;
    IAudioCaptureClient* inputCaptureClient = nullptr;

    IAudioClient* outputAudioClient = nullptr;
    IAudioRenderClient* outputRenderClient = nullptr;

    QThread* captureThread = nullptr;
    std::atomic<bool> stopFlag{false};

    UINT32 bufferFrameCount = 0;
    WAVEFORMATEX* captureFormat;

    std::atomic<float> outputVolume;

};


#endif //MICSPAMMER_MICCAPTURE_H