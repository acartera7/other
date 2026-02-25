//
// Created by Victus on 2/17/2026.
//

#include "MicCapture.h"
#include <QDebug>

#include "WasapiManager.h"

MicCapture & MicCapture::getInstance() {
    static MicCapture instance;
    return instance;
}

MicCapture::MicCapture(QObject *parent) : QObject(parent) {
}

MicCapture::~MicCapture() {
    stop();
    if (captureRenderClient) captureRenderClient->Release();
    if (outputRenderClient) outputRenderClient->Release();
    CoTaskMemFree(captureMixFormat);
    CoTaskMemFree(outputMixFormat);

}

void MicCapture::setCaptureDevice(const std::wstring &id) {
    releaseCaptureClient();

    if (id.empty()) return;

    IMMDevice *device = nullptr;
    HRESULT hr = WasapiManager::getInstance().getEnumerator()->GetDevice(id.c_str(), &device);

    if (FAILED(hr)) {
        qDebug() << "MicCaptureClient: Failed to set capture device.";
        return;
    }
    initCaptureClient(device);
}

void MicCapture::setOutputDevice(const std::wstring &id) {
    releaseOutputClient();

    if (id.empty()) return;

    IMMDevice *device = nullptr;
    HRESULT hr = WasapiManager::getInstance().getEnumerator()->GetDevice(id.c_str(), &device);
    if (FAILED(hr)) {
        qDebug() << "MicCaptureClient: Failed to set output device.";
        return;
    }
    initOutputClient(device);
}

void MicCapture::releaseCaptureClient() {
    if (captureRenderClient) { captureRenderClient->Release(); captureRenderClient = nullptr; }
    if (captureAudioClient) { captureAudioClient->Release(); captureAudioClient = nullptr; }
    if (captureDevice) { captureDevice->Release(); captureDevice = nullptr; }
    CoTaskMemFree(captureMixFormat);
}

void MicCapture::releaseOutputClient() {
    if (outputRenderClient) { outputRenderClient->Release(); outputRenderClient = nullptr; }
    if (outputAudioClient) { outputAudioClient->Release(); outputAudioClient = nullptr; }
    if (outputDevice) { outputDevice->Release(); outputDevice = nullptr; }
    CoTaskMemFree(outputMixFormat);
}

void MicCapture::initCaptureClient(IMMDevice *device) {
    // Initialize for output
    HRESULT hr = device->Activate(__uuidof(IUnknown),
                                     CLSCTX_ALL,
                                     nullptr,
                                     (void**)&captureAudioClient);
    if (FAILED(hr) || !captureAudioClient) {
        qDebug() << "MicCaptureClient: Failed to activate captureAudioClient.";
        return;
    }

    hr = captureAudioClient->GetMixFormat(&captureMixFormat);
    if (FAILED(hr) || !captureMixFormat) {
        qDebug() << "MicCaptureClient: Failed to get mix format in initCaptureClient.";
        return;
    }

    // Initialize for capture
    REFERENCE_TIME bufferDuration = 100000;
    hr = captureAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
                                 AUDCLNT_STREAMFLAGS_LOOPBACK,
                                 bufferDuration,
                                 0,
                                 captureMixFormat,
                                 nullptr);
    if (FAILED(hr)) {
        qDebug() << "MicCaptureClient: Failed to initialize captureAudioClient.";
        CoTaskMemFree(captureMixFormat);
        return;
    }

    hr = captureAudioClient->GetService(__uuidof(IAudioCaptureClient),
                                 (void**)&captureRenderClient);

    if (FAILED(hr) || !captureRenderClient) {
        qDebug() << "MicCaptureClient: Failed to get captureRenderClient.";
    }

}

void MicCapture::initOutputClient(IMMDevice *device) {
    HRESULT hr = device->Activate(__uuidof(IUnknown),
                                     CLSCTX_ALL,
                                     nullptr,
                                     (void**)&outputAudioClient);
    if (FAILED(hr) || !outputAudioClient) {
        qDebug() << "MicCaptureClient: Failed to activate outputAudioClient.";
        return;
    }


    hr = outputAudioClient->GetMixFormat(&outputMixFormat);
    if (FAILED(hr) || !outputMixFormat) {
        qDebug() << "MicCaptureClient: Failed to get mix format in initOutputClient.";
        return;
    }

    REFERENCE_TIME bufferDuration = 100000;
    hr = outputAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
                                       0,
                                       bufferDuration,
                                       0,
                                       outputMixFormat,
                                       nullptr);
    if (FAILED(hr)) {
        qDebug() << "MicCaptureClient: Failed to initialize outputAudioClient.";
        CoTaskMemFree(outputMixFormat);
        return;
    }

    hr = outputAudioClient->GetService(__uuidof(IAudioCaptureClient),
                                        (void**)&outputRenderClient);
    if (FAILED(hr) || !outputRenderClient) {
        qDebug() << "MicCaptureClient: Failed to get outputRenderClient.";
    }
}

void MicCapture::start() {
    if (!outputRenderClient || !captureRenderClient) {
        return;
    }
    stopFlag = false;

    captureThread = QThread::create([this]() {
        captureAudioClient->Start();
        captureLoop();
        captureAudioClient->Stop();
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
        HRESULT hr = captureRenderClient->GetNextPacketSize(&packetLength);
        if (FAILED(hr)) break;

        while ( packetLength > 0 ) {
            BYTE* pData;
            UINT32 numFrames;
            DWORD flags;

            hr = captureRenderClient->GetBuffer(&pData, &numFrames, &flags, nullptr, nullptr);
            if (FAILED(hr)) break;

            size_t bytes = numFrames * captureMixFormat->nBlockAlign;
            QByteArray pcm(reinterpret_cast<const char*>(pData), bytes);

            emit micDataReady(pcm);

            captureRenderClient->ReleaseBuffer(numFrames);

            hr = captureRenderClient->GetNextPacketSize(&packetLength);
            if (FAILED(hr)) break;
        }

    QThread::msleep(1);
    }
}
