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
    explicit MicCapture(IMMDevice* device, QObject *parent = nullptr);
    ~MicCapture() override;

    void start();
    void stop();

signals:
    void micDataReady(const QByteArray &pcm);

private:
    void captureLoop();

    IMMDevice* micDevice = nullptr;
    IAudioClient* audioClient = nullptr;
    IAudioCaptureClient* captureClient = nullptr;

    QThread* captureThread = nullptr;
    std::atomic<bool> stopFlag{false};

    UINT32 bufferFrameCount = 0;
    WAVEFORMATEX *_mixFormat;
};


#endif //MICSPAMMER_MICCAPTURE_H