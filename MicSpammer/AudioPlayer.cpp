//
// Created by Andrei on 5/4/2025.
//
#include "AudioPlayer.h"
#include <QStandardPaths>

AudioPlayer & AudioPlayer::getInstance() {
    static AudioPlayer instance;
    return instance;
}

AudioPlayer::AudioPlayer(QObject *parent) :
        QObject(parent),
        _volume(1.0f){
    pCurrentDevice = WasapiManager::getInstance().getCurrentDevice(); // TODO account for device change
}

AudioPlayer::~AudioPlayer() {
    stopAll();
}

void AudioPlayer::play(const QString& filePath) {

    qDebug() << "AudioPlayer: Play file:" << filePath;

    auto* instance = new SoundInstance(filePath, pCurrentDevice, _volume, this);
    connect(instance, &SoundInstance::finished, this, &AudioPlayer::onInstanceFinished);

    activeInstances.push_back(instance);
    instance->start();
}


void AudioPlayer::stopAll() {
    qDebug() << "AudioPlayer: Stop all sounds";
    for (auto* instance : activeInstances) {
        instance->stop();
        instance->deleteLater();
    }
    activeInstances.clear();
}

void AudioPlayer::onInstanceFinished(SoundInstance* instance) {
    //qDebug() << "AudioPlayer: Instance " << instance->getFileName() << " finished.";
    activeInstances.erase(
        std::remove(activeInstances.begin(), activeInstances.end(), instance),
        activeInstances.end()
    );
    instance->deleteLater();
}

void AudioPlayer::setVolume(float volume) {
    _volume = volume;
    for (auto* instance : activeInstances) {
        instance->setVolume(volume);
    }
}
