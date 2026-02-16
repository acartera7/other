//
// Created by Victus on 2/8/2026.
//

#include <QDebug>
#include <QFileInfo>
#include "SoundInstance.h"

SoundInstance::SoundInstance(const QString& path, IMMDevice* device, float volume, QObject *parent) :
        QObject(parent),
        filePath(path),
        stopFlag(false),
        _volume(volume){

    HRESULT hr = device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,(void**)&_pAudioClient);

    if (FAILED(hr) || !_pAudioClient) {
        QFileInfo f(filePath);
        qDebug() << QString("Failed to activate audio client for sound instance: %1").arg(f.completeBaseName()+"."+f.suffix());
        return;
    }

    // Initialize audio client in shared mode

    WAVEFORMATEX* audioClientFmt = nullptr;
    _pAudioClient->GetMixFormat(&audioClientFmt);
    if (FAILED(hr) || !audioClientFmt) {
        QFileInfo f(filePath);
        qDebug() << QString("Failed to get mix format for sound instance: %1").arg(f.completeBaseName()+"."+f.suffix());
    }

    REFERENCE_TIME bufferDuration = 100000;
    hr = _pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
                                 0,
                                 bufferDuration,
                                 0,
                                 audioClientFmt,
                                 nullptr);

    if (FAILED(hr)) {
        QFileInfo f(filePath);
        qDebug() << QString("Failed to initialize audioClient for sound instance: %1").arg(f.completeBaseName()+"."+f.suffix());
    }

    _pAudioClient->GetBufferSize(&bufferFrameCount);
    bytesPerFrame = audioClientFmt->nBlockAlign;

    QAudioFormat qFormat;
    qFormat.setSampleRate(audioClientFmt->nSamplesPerSec);
    qFormat.setChannelCount(audioClientFmt->nChannels);
    qFormat.setChannelConfig(QAudioFormat::ChannelConfigStereo);
    //qFormat.setSampleFormat(QAudioFormat::Float);

    if (audioClientFmt->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
        WAVEFORMATEXTENSIBLE* wfext = reinterpret_cast<WAVEFORMATEXTENSIBLE*>(audioClientFmt);
        if (wfext->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT) {
            qFormat.setSampleFormat(QAudioFormat::Float);
        } else if (wfext->SubFormat == KSDATAFORMAT_SUBTYPE_PCM) {
            qFormat.setSampleFormat(QAudioFormat::Int16);
        }
    } else if (audioClientFmt->wFormatTag == WAVE_FORMAT_IEEE_FLOAT) {
        qFormat.setSampleFormat(QAudioFormat::Float);
    } else {
        qFormat.setSampleFormat(QAudioFormat::Int16);
    }

    loader = new AudioLoader(this);
    loader->setDecoderFormat(qFormat);
    _qFormat = qFormat;


    connect(loader, &AudioLoader::pcmReady, this, &SoundInstance::onPcmReady);
    CoTaskMemFree(audioClientFmt);
}
SoundInstance::~SoundInstance() {
    stop();
    _pAudioClient->Release();
}

void SoundInstance::start() {
    loader->loadFile(filePath);
}

void SoundInstance::stop() {
    stopFlag = true;

    if (_pAudioClient)
        _pAudioClient->Stop(); // <-- instant halt

    if (playbackThread) {
        playbackThread->quit();
        playbackThread->wait();
        playbackThread = nullptr;
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
    if (!loadedOK || !_pAudioClient)
        return;

    playbackThread = QThread::create([this]() {
        // Each thread needs it's on render client
        IAudioRenderClient* renderClient = nullptr;
        HRESULT hr = _pAudioClient->GetService(__uuidof(IAudioRenderClient), (void**)&renderClient);

        if (FAILED(hr) || !renderClient)
            return;

        hr = _pAudioClient->Start();
        if (FAILED(hr)) {
            QFileInfo f(filePath);
            qDebug() << QString("Failed to start the audioClient for sound instance: %1").arg(f.completeBaseName()+"."+f.suffix());
        }

        writeAudioData(renderClient); // blocking loop

        _pAudioClient->Stop();
        renderClient->Release();
    });

    connect(playbackThread, &QThread::finished, this, [this]() {
        emit finished(this);
    });
    playbackThread->start();
}

void SoundInstance::writeAudioData(IAudioRenderClient* renderClient) {
    if (!renderClient)
        return;

    _pAudioClient->GetBufferSize(&bufferFrameCount);

    size_t totalBytes = pcmData.size();
    size_t bytesConsumed = 0;

    while (!stopFlag && bytesConsumed < totalBytes) {
        UINT32 padding = 0;
        _pAudioClient->GetCurrentPadding(&padding);
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

        // Apply volume scaling here
        float volume = _volume.load();

        if (_qFormat.sampleFormat() == QAudioFormat::Float) {
            const float* src = reinterpret_cast<const float*>(pcmData.data() + bytesConsumed);
            float* dst = reinterpret_cast<float*>(pData);
            size_t samples = bytesToWrite / sizeof(float);

            for (size_t i = 0; i < samples; ++i) {
                dst[i] = src[i] * volume;
            }
        } else {
            const int16_t* src = reinterpret_cast<const int16_t*>(pcmData.data() + bytesConsumed);
            int16_t* dst = reinterpret_cast<int16_t*>(pData);
            size_t samples = bytesToWrite / sizeof(int16_t);

            for (size_t i = 0; i < samples; ++i) {
                int sample = static_cast<int>(src[i] * volume);
                if (sample > 32767) sample = 32767;
                if (sample < -32768) sample = -32768;
                dst[i] = static_cast<int16_t>(sample);
            }
        }
        bytesConsumed += bytesToWrite;

        hr = renderClient->ReleaseBuffer(framesAvailable, 0);
        if (FAILED(hr)) {
            QFileInfo f(filePath);
            qDebug() << QString("Failed to release buffer for sound instance: %1").arg(f.completeBaseName()+"."+f.suffix());
        }
    }
}

