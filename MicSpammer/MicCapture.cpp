//
// Created by Victus on 2/17/2026.
//

#include "MicCapture.h"
#include <QDebug>

MicCapture::MicCapture(IMMDevice *device, QObject *parent) :
    QObject(parent), micDevice(device){

    if (!micDevice) {
        qDebug() << "MicCapture: No device provided.";
        return;
    }

    // Initialize for output
    HRESULT hr = micDevice->Activate(__uuidof(IUnknown),
                                     CLSCTX_ALL,
                                     nullptr,
                                     (void**)&audioClient);
    if (FAILED(hr)) {
        qDebug() << "MicCaptureClient: Failed to activate audio client.";
        return;
    }

    _mixFormat = nullptr;
    hr = audioClient->GetMixFormat(&_mixFormat);
    if (FAILED(hr) || !_mixFormat) {
        qDebug() << "MicCaptureClient: Failed to get mix format.";
        return;
    }

    // Initialize for capture
    REFERENCE_TIME bufferDuration = 100000;
    hr = audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
                                 AUDCLNT_STREAMFLAGS_LOOPBACK,
                                 bufferDuration,
                                 0,
                                 _mixFormat,
                                 nullptr);
    if (FAILED(hr)) {
        qDebug() << "MicCaptureClient: Failed to initialize audio client.";
    }

    audioClient->GetBufferSize(&bufferFrameCount);

    hr = audioClient->GetService(__uuidof(IAudioCaptureClient),
                                 (void**)&captureClient);

    if (FAILED(hr)) {
        qDebug() << "MicCaptureClient: Failed to get capture client.";
    }

    CoTaskMemFree(_mixFormat);
}

MicCapture::~MicCapture() {
    stop();
    if (captureClient) captureClient->Release();
    if (audioClient) audioClient->Release();
}

void MicCapture::start() {
    if (!audioClient || !captureClient) {
        return;
    }

    stopFlag = false;

    captureThread = QThread::create([this]() {
        audioClient->Start();
        captureLoop();
        audioClient->Stop();
    });

    captureThread->start();
}

void MicCapture::stop() {
    stopFlag = true;
    if (captureThread ) {
        captureThread->quit();
        captureThread->wait();
        captureThread = nullptr;
    }
}

void MicCapture::captureLoop() {
    while (!stopFlag) {
        UINT32 packetLength = 0;
        HRESULT hr = captureClient->GetNextPacketSize(&packetLength);
        if (FAILED(hr)) break;

        while ( packetLength > 0 ) {
            BYTE* pData;
            UINT32 numFrames;
            DWORD flags;

            hr = captureClient->GetBuffer(&pData, &numFrames, &flags, nullptr, nullptr);
            if (FAILED(hr)) break;

            size_t bytes = numFrames * _mixFormat->nBlockAlign;
            QByteArray pcm(reinterpret_cast<const char*>(pData), bytes);

            emit micDataReady(pcm);

            captureClient->ReleaseBuffer(numFrames);

            hr = captureClient->GetNextPacketSize(&packetLength);
            if (FAILED(hr)) break;
        }

    QThread::msleep(1);
    }
}
