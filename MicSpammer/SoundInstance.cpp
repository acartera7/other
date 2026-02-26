//
// Created by Victus on 2/8/2026.
//

#include <QDebug>
#include <QFileInfo>
#include "SoundInstance.h"

SoundInstance::SoundInstance(const QString& path, IMMDevice* monitorDevice, IMMDevice* outputDevice, float volume, QObject *parent) :
        QObject(parent),
        filePath(path),
        stopFlag(false),
        _volume(volume){
    // Initialize monitor audio client in shared mode
    HRESULT hr;
    if (monitorDevice) {

        hr = monitorDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,(void**)&monitorAudioClient);
        if (FAILED(hr) || !monitorAudioClient) {
            QFileInfo f(filePath);
            qDebug() << QString("Failed to activate audio client for sound instance: %1").arg(f.completeBaseName()+"."+f.suffix());
            return;
        }

        hr = initMonitorAudioClient();
        if (FAILED(hr)) {
            QFileInfo f(filePath);
            qDebug() << QString("Failed to initialize monitor audio client for sound instance: %1").arg(f.completeBaseName()+"."+f.suffix());
        }
    }

    if (outputDevice) {
        // Initialize output audio client in shared mode
        hr = outputDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,(void**)&outputAudioClient);
        if (FAILED(hr) || !outputAudioClient) {
            QFileInfo f(filePath);
            qDebug() << QString("Failed to activate audio client for sound instance: %1").arg(f.completeBaseName()+"."+f.suffix());
            return;
        }

        hr = initOutputAudioClient();
        if (FAILED(hr)) {
            QFileInfo f(filePath);
            qDebug() << QString("Failed to initialize output audio client: for sound instance: %1").arg(f.completeBaseName()+"."+f.suffix());
        }
    }

    //TODO two loaders?
    bytesPerFrame = monitorAudioFmt->nBlockAlign;

    QAudioFormat qFormat;
    qFormat.setSampleRate(monitorAudioFmt->nSamplesPerSec);
    qFormat.setChannelCount(monitorAudioFmt->nChannels);
    qFormat.setChannelConfig(QAudioFormat::ChannelConfigStereo);
    //qFormat.setSampleFormat(QAudioFormat::Float);

    if (monitorAudioFmt->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
        WAVEFORMATEXTENSIBLE* wfext = reinterpret_cast<WAVEFORMATEXTENSIBLE*>(monitorAudioFmt);
        if (wfext->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT) {
            qFormat.setSampleFormat(QAudioFormat::Float);
        } else if (wfext->SubFormat == KSDATAFORMAT_SUBTYPE_PCM) {
            qFormat.setSampleFormat(QAudioFormat::Int16);
        }
    } else if (monitorAudioFmt->wFormatTag == WAVE_FORMAT_IEEE_FLOAT) {
        qFormat.setSampleFormat(QAudioFormat::Float);
    } else {
        qFormat.setSampleFormat(QAudioFormat::Int16);
    }

    loader = new AudioLoader(this);
    loader->setDecoderFormat(qFormat);
    _qFormat = qFormat;

    connect(loader, &AudioLoader::pcmReady, this, &SoundInstance::onPcmReady);
}
SoundInstance::~SoundInstance() {
    stop();
    monitorAudioClient->Release();
    outputAudioClient->Release();
    CoTaskMemFree(monitorAudioFmt);
}

void SoundInstance::start() {
    loader->loadFile(filePath);
    loadedOK = false;
}

void SoundInstance::stop() {
    stopFlag = true;

    if (monitorAudioClient) monitorAudioClient->Stop(); // <-- instant halt
    if (outputAudioClient) outputAudioClient->Stop(); // <-- instant halt

    if (playbackThreadMonitor) {
        playbackThreadMonitor->quit();
        playbackThreadMonitor->wait();
        playbackThreadMonitor = nullptr;
    }
    if (playbackThreadOutput) {
        playbackThreadOutput->quit();
        playbackThreadOutput->wait();
        playbackThreadOutput = nullptr;
    }
}

QString SoundInstance::getFileName() {
    return filePath;
}

void SoundInstance::setVolume(float volume) {
    _volume = volume;
}

void SoundInstance::onPcmReady(const QByteArray& data) {
    // called once when file fully decoded

    pcmData = std:: vector<BYTE>(data.begin(), data.end());
    loadedOK = true;

    startPlaybackThread();
}

void SoundInstance::startPlaybackThread() {
    if (!loadedOK)
        return;

    if (monitorAudioClient) {
        playbackThreadMonitor = QThread::create([this]() {
            IAudioRenderClient* monitorRenderClient = nullptr;
            HRESULT hr = monitorAudioClient->GetService(__uuidof(IAudioRenderClient), (void**)&monitorRenderClient);

            if (FAILED(hr) || !monitorRenderClient) return;

            hr = monitorAudioClient->Start();
            if (FAILED(hr)) {
                QFileInfo f(filePath);
                qDebug() << QString("Failed to start the monitorAudioClient for sound instance: %1").arg(f.completeBaseName()+"."+f.suffix());
            }
            writeAudioData(monitorAudioClient,monitorRenderClient); // blocking loop
            monitorAudioClient->Stop();
            monitorRenderClient->Release();
        });
        connect(playbackThreadMonitor, &QThread::finished, this, [this]() {
            emit finished(this);
        });
        playbackThreadMonitor->start();
    }
    if (outputAudioClient) {
        playbackThreadOutput = QThread::create([this]() {
            IAudioRenderClient* outputRenderClient = nullptr;
            HRESULT hr = outputAudioClient->GetService(__uuidof(IAudioRenderClient), (void**)&outputRenderClient);

            if (FAILED(hr) || !outputRenderClient) return;

            hr = outputAudioClient->Start();
            if (FAILED(hr)) {
                QFileInfo f(filePath);
                qDebug() << QString("Failed to start the outputAudioClient for sound instance: %1").arg(f.completeBaseName()+"."+f.suffix());
            }
            writeAudioData(outputAudioClient,outputRenderClient); // blocking loop
            outputAudioClient->Stop();
            outputRenderClient->Release();
        });
        connect(playbackThreadOutput, &QThread::finished, this, [this]() {
            emit finished(this);
        });
        playbackThreadOutput->start();
    }
}

void SoundInstance::writeAudioData(IAudioClient *audioClient,
                                   IAudioRenderClient *renderClient) {

    UINT32 bufferFrameCount = 0;
    audioClient->GetBufferSize(&bufferFrameCount);

    size_t totalBytes = pcmData.size();
    size_t bytesConsumed = 0;

    while (!stopFlag && bytesConsumed < totalBytes) {
        UINT32 padding = 0;
        audioClient->GetCurrentPadding(&padding);
        UINT32 framesAvailable = bufferFrameCount - padding;
        if (framesAvailable == 0) {
            QThread::msleep(1);
            continue;
        }

        BYTE* pData = nullptr;
        HRESULT hr = renderClient->GetBuffer(framesAvailable, &pData);
        if (FAILED(hr)) break;

        size_t bytesToWrite = framesAvailable * bytesPerFrame;
        size_t bytesRemaining = totalBytes - bytesConsumed;
        if (bytesToWrite > bytesRemaining) {
            bytesToWrite = bytesRemaining;
            framesAvailable = (UINT32)(bytesToWrite / bytesPerFrame);
        }

        //memcpy(pData, pcmData.data() + bytesConsumed, bytesToWrite);

        // Copy + volume scale
        if (_qFormat.sampleFormat() == QAudioFormat::Float) {
            const float* src = reinterpret_cast<const float*>(pcmData.data() + bytesConsumed);
            float* dst = reinterpret_cast<float*>(pData);
            size_t samples = bytesToWrite / sizeof(float);
            for (size_t i = 0; i < samples; ++i) dst[i] = src[i] * _volume;
        } else {
            const int16_t* src = reinterpret_cast<const int16_t*>(pcmData.data() + bytesConsumed);
            int16_t* dst = reinterpret_cast<int16_t*>(pData);
            size_t samples = bytesToWrite / sizeof(int16_t);
            for (size_t i = 0; i < samples; ++i) {
                int sample = static_cast<int>(src[i] * _volume);
                dst[i] = std::clamp(sample, -32768, 32767);
            }
        }
        bytesConsumed += bytesToWrite;
        hr = renderClient->ReleaseBuffer(framesAvailable, 0);
        if (FAILED(hr)) {
            QFileInfo f(filePath);
            qDebug() << QString("Failed to release buffer for sound instance: %1").arg(f.completeBaseName()+"."+f.suffix());
            break;
        }
    }
}

HRESULT SoundInstance::initMonitorAudioClient() {

    HRESULT hr = monitorAudioClient->GetMixFormat(&monitorAudioFmt);
    if (FAILED(hr) || !monitorAudioFmt) {
        QFileInfo f(filePath);
        qDebug() << QString("Failed to get mix format for sound instance: %1").arg(f.completeBaseName()+"."+f.suffix());
        return hr;
    }

    REFERENCE_TIME bufferDuration = 100000;
    hr = monitorAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
                                 0,
                                 bufferDuration,
                                 0,
                                 monitorAudioFmt,
                                 nullptr);

    if (FAILED(hr)) {
        QFileInfo f(filePath);
        qDebug() << QString("Failed to initialize audioClient for sound instance: %1").arg(f.completeBaseName()+"."+f.suffix());
        return hr;
    }

    return 0;
}

HRESULT SoundInstance::initOutputAudioClient() {

    HRESULT hr = outputAudioClient->GetMixFormat(&outputAudioFmt);
    if (FAILED(hr) || !outputAudioFmt) {
        QFileInfo f(filePath);
        qDebug() << QString("Failed to get mix format for sound instance: %1").arg(f.completeBaseName()+"."+f.suffix());
        return hr;
    }

    REFERENCE_TIME bufferDuration = 100000;
    hr = outputAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
                                 0,
                                 bufferDuration,
                                 0,
                                 outputAudioFmt,
                                 nullptr);

    if (FAILED(hr)) {
        QFileInfo f(filePath);
        qDebug() << QString("Failed to initialize audioClient for sound instance: %1").arg(f.completeBaseName()+"."+f.suffix());
        return hr;
    }

    return 0;
}