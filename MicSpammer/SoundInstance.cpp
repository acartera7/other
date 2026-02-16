//
// Created by Victus on 2/8/2026.
//

#include "SoundInstance.h"
#include <QDebug>

SoundInstance::SoundInstance(const QString& path, IAudioClient* client, QObject *parent)
    :   QObject(parent),
        filePath(path),
        audioClient(client) {

    loader = new AudioLoader(this);

    connect(loader, &AudioLoader::pcmReady, this, &SoundInstance::onPcmReady);

}
SoundInstance::~SoundInstance() {
    stop();
}

void SoundInstance::start() {
    loader->loadFile(filePath);
}
void SoundInstance::stop() {
    stopFlag = true;

    if (playbackThread) {
        playbackThread->quit();
        playbackThread->wait();
        playbackThread = nullptr;
        //playbackThread->deleteLater();
    }
}

void SoundInstance::finished(SoundInstance *self) {

}

void SoundInstance::onPcmReady(const QByteArray& data, const QAudioFormat& format) {
    // called once when file fully decoded

    wfx.nSamplesPerSec  = format.sampleRate();          // sample rate
    wfx.nChannels       = format.channelCount();        // num channels
    wfx.wBitsPerSample  = format.bytesPerSample() * 8;  // 16-bit or 8-bit
    wfx.nBlockAlign     = (wfx.nChannels * wfx.wBitsPerSample) / 8; //frame size in bytes
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.cbSize          = 0;

    if (format.sampleFormat() == QAudioFormat::Float)
        wfx.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
    else
        wfx.wFormatTag = WAVE_FORMAT_PCM;

    pcmData = std:: vector<BYTE>(data.begin(), data.end());
    loadedOK = true;

    startPlaybackThread();
}

void SoundInstance::startPlaybackThread() {
    if (loadedOK || !audioClient)
        return;

    playbackThread = QThread::create([this]() {
        // Each thread needs it's on render client
        IAudioRenderClient* renderClient = nullptr;
        HRESULT hr = audioClient->GetService(__uuidof(IAudioRenderClient), (void**)&renderClient);

        if (FAILED(hr) || !renderClient)
            return;

        writeAudioData(renderClient); // blocking loop
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

    UINT32 bufferFrameCount = 0;
    audioClient->GetBufferSize(&bufferFrameCount);

    UINT32 bytesPerFrame = wfx.nBlockAlign;
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

        size_t bytesToWrite = framesAvailable * bytesPerFrame;;
        size_t bytesRemaining = totalBytes - bytesConsumed;
        if (bytesToWrite > bytesRemaining) {
            bytesToWrite = bytesRemaining;
            framesAvailable = (UINT32)(bytesToWrite / bytesPerFrame);
        }

        memcpy(pData, pcmData.data() + bytesConsumed, bytesToWrite);
        bytesConsumed += bytesToWrite;

        renderClient->ReleaseBuffer(framesAvailable, 0);
    }
}

