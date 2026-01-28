//
// Created by Andrei on 5/4/2025.
//
#include "AudioPlayer.h"

AudioPlayer & AudioPlayer::getInstance() {
    static AudioPlayer instance;
    return instance;
}

AudioPlayer::AudioPlayer(QObject *parent)
    : QObject(parent){

    loader = new AudioLoader(this);
    connect(loader, &AudioLoader::pcmReady, this, [this](const QByteArray& data, const QAudioFormat& format) {
        // Here you have PCM data and format info
        // Convert if needed, then push into WASAPI render buffer

        WAVEFORMATEX wfx;
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

        loadedFile.data = std:: vector<BYTE>(data.begin(), data.end());
        loadedFile.format = wfx;
        loadedFile.ok = true;

    });


    _pAudioClient = WasapiManager::getInstance().getAudioClient();
    _pAudioClient->Start();

    //if (pAudioClient != nullptr)
    //{
    //    WAVEFORMATEX *ppDeviceFormat;
    //    pAudioClient->GetMixFormat(&ppDeviceFormat);
    //}
}

AudioPlayer::~AudioPlayer() {
    delete loader;
    _pAudioClient->Stop();

}

void AudioPlayer::play(const QString& filePath) {
    qDebug() << "AudioPlayer: Play file:" << filePath;
    if (!loadedFile.ok) {
        qDebug() << "AudioPlayer: No File Loaded";
        return;
    }

    QThread* t = QThread::create([this]() {
        // Each thread needs it's on render client
        IAudioRenderClient* renderClient = nullptr;
        HRESULT hr = _pAudioClient->GetService(__uuidof(IAudioRenderClient), (void**)&renderClient);

        if (FAILED(hr)) return;


        writeAudioData(renderClient); // blocking loop

        renderClient->Release();
    });

    connect(t, &QThread::finished, this, [this, t]() {
        activeThreads.erase(std::remove(activeThreads.begin(), activeThreads.end(), t),
            activeThreads.end());
        t->deleteLater();
    });
    activeThreads.push_back(t);
    t->start();
}


void AudioPlayer::stop() {
    // Stop playback
    stopAllAudio = true;
    for (auto t : activeThreads) {
        t->quit();
        t->wait();
        delete t;
    }
    activeThreads.clear();
    stopAllAudio = false;
}

void AudioPlayer::setVolume(float volume) {
    // Adjust volume via WASAPI if supported
}

void AudioPlayer::setupAudioStream() {

}

void AudioPlayer::writeAudioData(IAudioRenderClient* renderClient) {
    if (!loadedFile.ok || !renderClient)
        return;

    UINT32 bufferFrameCount = 0;
    _pAudioClient->GetBufferSize(&bufferFrameCount);

    UINT32 bytesPerFrame = loadedFile.format.nBlockAlign;
    size_t totalBytes = loadedFile.data.size();
    size_t bytesConsumed = 0;

    while (!stopAllAudio && bytesConsumed < totalBytes) {
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

        size_t bytesToWrite = framesAvailable * bytesPerFrame;;
        size_t bytesRemaining = totalBytes - bytesConsumed;
        if (bytesToWrite > bytesRemaining) {
            bytesToWrite = bytesRemaining;
            framesAvailable = (UINT32)(bytesToWrite / bytesPerFrame);
        }

        memcpy(pData, loadedFile.data.data() + bytesConsumed, bytesToWrite);
        bytesConsumed += bytesToWrite;

        renderClient->ReleaseBuffer(framesAvailable, 0);
    }
}

void AudioPlayer::loadAudioFile(const QString &filePath) {
    loader->loadFile(filePath);
}

