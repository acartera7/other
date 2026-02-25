//
// Created by Victus on 2/17/2026.
//
#pragma once

#ifndef MICSPAMMER_MICCAPTURE_H
#define MICSPAMMER_MICCAPTURE_H
#include <QObject>
#include <QThread>

#include <atomic>
#include <audioclient.h>
#include <mmdeviceapi.h>

class MicCapture : public QObject {
    Q_OBJECT
public:
    static MicCapture& getInstance();

    MicCapture(const MicCapture&) = delete;
    MicCapture& operator=(const MicCapture&) = delete;

    void setCaptureDevice(const std::wstring &id);
    void setOutputDevice(const std::wstring &id);

    void start();
    void stop();

signals:
    void micDataReady(const QByteArray &pcm);

private:
    explicit MicCapture(QObject *parent = nullptr);
    ~MicCapture() override;

    void captureLoop();
    void releaseCaptureClient();
    void releaseOutputClient();
    void initCaptureClient(IMMDevice *device);
    void initOutputClient(IMMDevice *device);

    IMMDevice* captureDevice = nullptr;
    IMMDevice* outputDevice = nullptr;

    IAudioClient* captureAudioClient = nullptr;
    IAudioCaptureClient* captureRenderClient = nullptr;

    IAudioClient* outputAudioClient = nullptr;
    IAudioRenderClient* outputRenderClient = nullptr;

    QThread* captureThread = nullptr;
    std::atomic<bool> stopFlag{false};

    UINT32 bufferFrameCount = 0;
    WAVEFORMATEX *captureMixFormat = nullptr;
    WAVEFORMATEX *outputMixFormat = nullptr;

};


#endif //MICSPAMMER_MICCAPTURE_H