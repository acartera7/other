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

    _pAudioClient = WasapiManager::getInstance().getAudioClient();
    _pAudioClient->Start();

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


    activeThreads.push_back(t); //TODO remove
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

}

void AudioPlayer::loadAudioFile(const QString &filePath) {
    loader->loadFile(filePath);
}

