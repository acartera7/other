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
}

AudioPlayer::~AudioPlayer() {
    stopAll();
}

void AudioPlayer::play(const QString& filePath) {

    //qDebug() << "AudioPlayer: Play file:" << filePath;
    if (!monitorDevice && !outputDevice)  {
        qDebug() << "AudioPlayer: Cannot play file, no monitor device or output device is set";
        return;
    }

    if (!monitorDevice) qDebug() << "AudioPlayer: Warning, no monitor device is set";
    if (!outputDevice) qDebug() << "AudioPlayer: Warning, no output device is set";

    auto* instance = new SoundInstance(filePath, monitorDevice, outputDevice, _volume, this);
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

void AudioPlayer::setMonitorDevice(const std::wstring &id) {
    releaseMonitorDevice();
    if (id.empty()) return;
    HRESULT hr = WasapiManager::getInstance().getEnumerator()->GetDevice(id.c_str(), &monitorDevice);
    if (FAILED(hr)) {
        qDebug() << "AudioPlayer: Failed to set Monitor device";
        return;
    }
}

void AudioPlayer::setOutputDevice(const std::wstring &id) {
    releaseOutputDevice();
    if (id.empty()) return;
    HRESULT hr = WasapiManager::getInstance().getEnumerator()->GetDevice(id.c_str(), &outputDevice);
    if (FAILED(hr)) {
        qDebug() << "AudioPlayer: Failed to set Output device";
        return;
    }
}

void AudioPlayer::releaseMonitorDevice() {
    if (monitorDevice) { monitorDevice->Release(); monitorDevice = nullptr; }
}

void AudioPlayer::releaseOutputDevice() {
    if (outputDevice) { outputDevice->Release(); outputDevice = nullptr; }
}


