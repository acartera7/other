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

MicCapture::MicCapture(QObject *parent) : QObject(parent), captureFormat() {
}

MicCapture::~MicCapture() {
    stop();
    if (inputCaptureClient) inputCaptureClient->Release();
    if (outputRenderClient) outputRenderClient->Release();

}

void MicCapture::setInputDevice(QString id) {
    releaseInputClient();

    if (id.isEmpty() || id == "None") return;


    HRESULT hr = WasapiManager::getInstance().getEnumerator()->GetDevice(id.toStdWString().c_str(), &inputDevice);

    if (FAILED(hr)) {
        qDebug() << "MicCaptureClient: Failed to set capture device.";
        return;
    }
    initInputClient();
    if (outputRenderClient && inputAudioClient) {
        start();
    }
}

void MicCapture::setOutputDevice(QString id) {
    releaseOutputClient();

    if (id.isEmpty() || id == "None") return;


    HRESULT hr = WasapiManager::getInstance().getEnumerator()->GetDevice(id.toStdWString().c_str(), &outputDevice);
    if (FAILED(hr)) {
        qDebug() << "MicCaptureClient: Failed to set output device.";
        return;
    }
    initOutputClient();
    // Auto-start, check if other device is available before
    if (inputCaptureClient && inputAudioClient) {
        start();
    }
}

void MicCapture::setVolume(float volume) {
    outputVolume.store(volume);
}

void MicCapture::releaseInputClient() {
    stop();
    if (inputCaptureClient) { inputCaptureClient->Release(); inputCaptureClient = nullptr; }
    if (inputAudioClient) { inputAudioClient->Release(); inputAudioClient = nullptr; }
    if (inputDevice) { inputDevice->Release(); inputDevice = nullptr; }
}

void MicCapture::releaseOutputClient() {
    stop();
    if (outputRenderClient) { outputRenderClient->Release(); outputRenderClient = nullptr; }
    if (outputAudioClient) { outputAudioClient->Release(); outputAudioClient = nullptr; }
    if (outputDevice) { outputDevice->Release(); outputDevice = nullptr; }
}

void MicCapture::initInputClient() {
    if (!inputDevice) {
        qDebug() << "MicCapture: cannot start, missing device";
        return;
    }
    // Initialize for output
    HRESULT hr = inputDevice->Activate(__uuidof(IAudioClient),
                                     CLSCTX_ALL,
                                     nullptr,
                                     (void**)&inputAudioClient);
    if (FAILED(hr) || !inputAudioClient) {
        qDebug() << "MicCaptureClient: Failed to activate captureAudioClient.";
        return;
    }

    // Initialize for capture
    inputAudioClient->GetMixFormat(&captureFormat);
    REFERENCE_TIME bufferDuration = 100000;
    hr = inputAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
                                 0,
                                 bufferDuration,
                                 0,
                                 captureFormat,
                                 nullptr);
    if (FAILED(hr)) {
        qDebug() << "MicCaptureClient: Failed to initialize captureAudioClient.";
        return;
    }

    hr = inputAudioClient->GetService(__uuidof(IAudioCaptureClient),
                                 (void**)&inputCaptureClient);

    if (FAILED(hr) || !inputCaptureClient) {
        qDebug() << "MicCaptureClient: Failed to get captureRenderClient.";
    }

    // reinitialize output in case it hasn't successfully been initialized due to format missing
    if (outputDevice) {
        initOutputClient();
    }

}

void MicCapture::initOutputClient() {
    if (!outputDevice) {
        qDebug() << "MicCapture: cannot initialize, missing device";
        return;
    }
    if (!captureFormat) {
        qDebug() << "MicCapture: cannot initialize, no input device selected";
        return;
    }
    HRESULT hr = outputDevice->Activate(__uuidof(IAudioClient),
                                     CLSCTX_ALL,
                                     nullptr,
                                     (void**)&outputAudioClient);
    if (FAILED(hr) || !outputAudioClient) {
        qDebug() << "MicCaptureClient: Failed to activate outputAudioClient.";
        return;
    }
    REFERENCE_TIME bufferDuration = 100000;
    hr = outputAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
                                       AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY,
                                       bufferDuration,
                                       0,
                                       captureFormat,
                                       nullptr);
    if (FAILED(hr)) {
        qDebug() << "MicCaptureClient: Failed to initialize outputAudioClient.";
        return;
    }

    hr = outputAudioClient->GetService(__uuidof(IAudioRenderClient),
                                        (void**)&outputRenderClient);
    if (FAILED(hr) || !outputRenderClient) {
        qDebug() << "MicCaptureClient: Failed to get outputRenderClient.";
    }
}

void MicCapture::start() {
    if (!inputAudioClient || !outputAudioClient) {
        qDebug() << "MicCapture: cannot start, missing device";
        return;
    }
    stopFlag = false;

    captureThread = QThread::create([this]() {
        inputAudioClient->Start();
        outputAudioClient->Start();
        captureLoop();
        inputAudioClient->Stop();
        outputAudioClient->Stop();
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

QJsonObject MicCapture::saveState() {
    QJsonObject result;
    return result;
}

void MicCapture::loadState(QJsonObject) {
}

void MicCapture::captureLoop() {
    //UINT32 blockAlign = captureFormat->nBlockAlign;

    while (!stopFlag) {
        UINT32 packetLength = 0;
        HRESULT hr = inputCaptureClient->GetNextPacketSize(&packetLength);
        if (FAILED(hr)) break;

        while (packetLength > 0) {
            BYTE* inData = nullptr;
            UINT32 numFrames = 0;
            DWORD flags = 0;

            hr = inputCaptureClient->GetBuffer(&inData, &numFrames, &flags, nullptr, nullptr);
            if (FAILED(hr)) break;

            BYTE* outData = nullptr;
            hr = outputRenderClient->GetBuffer(numFrames, &outData);
            if (FAILED(hr)) {
                inputCaptureClient->ReleaseBuffer(numFrames);
                qDebug() << "MicCaptureClient: Failed to get buffer.";
                break;
            }

            size_t bytes = numFrames * captureFormat->nBlockAlign;
            float vol = outputVolume.load();

            if (captureFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
                WAVEFORMATEXTENSIBLE* ext = reinterpret_cast<WAVEFORMATEXTENSIBLE*>(captureFormat);
                if (ext->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT) {
                    const float* src = reinterpret_cast<const float*>(inData);
                    float* dst = reinterpret_cast<float*>(outData);
                    size_t samples = bytes / sizeof(float);
                    for (size_t i = 0; i < samples; ++i) dst[i] = src[i] * vol;
                } else if (ext->SubFormat == KSDATAFORMAT_SUBTYPE_PCM &&
                           ext->Format.wBitsPerSample == 16) {
                    const int16_t* src = reinterpret_cast<const int16_t*>(inData);
                    int16_t* dst = reinterpret_cast<int16_t*>(outData);
                    size_t samples = bytes / sizeof(int16_t);
                    for (size_t i = 0; i < samples; ++i) {
                        int sample = static_cast<int>(src[i] * vol);
                        dst[i] = std::clamp(sample, -32768, 32767);
                    }
                } else {
                   memcpy(outData, inData, bytes); // fallback
                }
            } else if (captureFormat->wFormatTag == WAVE_FORMAT_IEEE_FLOAT) {
                const float* src = reinterpret_cast<const float*>(inData);
                float* dst = reinterpret_cast<float*>(outData);
                size_t samples = bytes / sizeof(float);
                for (size_t i = 0; i < samples; ++i) dst[i] = src[i] * vol;
            } else if (captureFormat->wFormatTag == WAVE_FORMAT_PCM &&
                       captureFormat->wBitsPerSample == 16) {
                const int16_t* src = reinterpret_cast<const int16_t*>(inData);
                int16_t* dst = reinterpret_cast<int16_t*>(outData);
                size_t samples = bytes / sizeof(int16_t);
                for (size_t i = 0; i < samples; ++i) {
                    int sample = static_cast<int>(src[i] * vol);
                    dst[i] = std::clamp(sample, -32768, 32767);
                }
           } else {
               memcpy(outData, inData, bytes); // fallback
           }


            inputCaptureClient->ReleaseBuffer(numFrames);
            outputRenderClient->ReleaseBuffer(numFrames, 0);

            hr = inputCaptureClient->GetNextPacketSize(&packetLength);
            if (FAILED(hr)) break;
        }

        //QThread::msleep(1);
    }
}
