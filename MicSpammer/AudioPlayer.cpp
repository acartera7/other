//
// Created by Andrei on 5/4/2025.
//
#include "AudioPlayer.h"
#include "WasapiManager.h"

AudioPlayer::AudioPlayer(QObject *parent)
    : QObject(parent){

    pAudioClient = WasapiManager::getInstance().getAudioClient();
}

void AudioPlayer::play(const QString &filePath) {
    loadAudioFile(filePath);
    // Start playback using WASAPI stream
}

void AudioPlayer::stop() {
    // Stop playback
}

void AudioPlayer::setVolume(float volume) {
    // Adjust volume via WASAPI if supported
}

void AudioPlayer::loadAudioFile(const QString &filePath) {

}