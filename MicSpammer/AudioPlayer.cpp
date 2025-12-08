//
// Created by Andrei on 5/4/2025.
//
#include "AudioPlayer.h"
#include "WasapiManager.h"
//#include "MediaManager.h" DEPRECATED

AudioPlayer::AudioPlayer(QObject *parent)
    : QObject(parent){

    pAudioClient = WasapiManager::getInstance().getAudioClient();
    if (pAudioClient != nullptr)
    {
        pAudioClient->GetMixFormat();
    }
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
}


void AudioPlayer::stop() {
    // Stop playback
}

void AudioPlayer::setVolume(float volume) {
    // Adjust volume via WASAPI if supported
}

void AudioPlayer::loadAudioFile(const QString &filePath) {

}

void AudioPlayer::setupAudioStream() {

}

void AudioPlayer::writeAudioData() {

}