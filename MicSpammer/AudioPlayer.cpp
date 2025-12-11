//
// Created by Andrei on 5/4/2025.
//
#include "AudioPlayer.h"

AudioPlayer::AudioPlayer(QObject *parent)
    : QObject(parent){

    loader = new AudioLoader(this);
    connect(loader, &AudioLoader::pcmReady, this, [this](const QByteArray& data, const QAudioFormat& format) {
        // Here you have PCM data and format info
        // Convert if needed, then push into WASAPI render buffer

        WAVEFORMATEX wfx;
        wfx.nSamplesPerSec  = format.sampleRate();
        wfx.nChannels       = format.channelCount();
        wfx.wBitsPerSample  = format.bytesPerSample() * 8;
        wfx.nBlockAlign     = (wfx.nChannels * wfx.wBitsPerSample) / 8;
        wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
        wfx.cbSize          = 0;

        if (format.sampleFormat() == QAudioFormat::Float)
            wfx.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
        else
            wfx.wFormatTag = WAVE_FORMAT_PCM;

        loadedFile.data = std::vector<BYTE>(data.begin(), data.end());
        loadedFile.format = wfx;
        loadedFile.ok = true;

    });
    pAudioClient = WasapiManager::getInstance().getAudioClient();
    //if (pAudioClient != nullptr)
    //{
    //    WAVEFORMATEX *ppDeviceFormat;
    //    pAudioClient->GetMixFormat(&ppDeviceFormat);
    //}
}

AudioPlayer::~AudioPlayer() {
    delete loader;
}

void AudioPlayer::play(const QString& filePath) {
    qDebug() << "AudioPlayer: Play file:" << filePath;
    //MediaManager& media = MediaManager::getInstance();
    //IMFSourceReader* stream = media.getFileStream(filePath);

    //if (!stream) return;
//
    //pAudioClient->Start();
    // Write streamed data to buffer...
    pRenderClient = nullptr;
    HRESULT hr = pAudioClient->GetService(__uuidof(IAudioRenderClient),
                                          (void**)&pRenderClient);
    if (FAILED(hr)) return;
    pAudioClient->Start();
}


void AudioPlayer::stop() {
    // Stop playback
}

void AudioPlayer::setVolume(float volume) {
    // Adjust volume via WASAPI if supported
}

void AudioPlayer::setupAudioStream() {

}

void AudioPlayer::writeAudioData() {

}

void AudioPlayer::loadAudioFile(const QString &filePath) {
    loader->loadFile(filePath);
}

