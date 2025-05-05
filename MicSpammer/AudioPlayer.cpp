//
// Created by Andrei on 5/4/2025.
//
#include "AudioPlayer.h"
#include <windows.h>
#include <iostream>

AudioPlayer::AudioPlayer(QObject *parent)
    : QObject(parent){

}

void AudioPlayer::initializeWasapi() {
    // Initialize WASAPI and set up audio rendering similar to your main.cpp initialization
}

void AudioPlayer::play(const QString &filePath) {
    //loadAudioFile(filePath);
    // Start playback using WASAPI stream
}

void AudioPlayer::stop() {
    // Stop playback
}

void AudioPlayer::setVolume(float volume) {
    // Adjust volume via WASAPI if supported
}
