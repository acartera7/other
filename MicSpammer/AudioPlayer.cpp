
#include "AudioPlayer.h"
#include <QStandardPaths>

AudioPlayer & AudioPlayer::getInstance() {
    static AudioPlayer instance;
    return instance;
}

AudioPlayer::AudioPlayer(QObject *parent) : QObject(parent),
        _monitorVolume(1.0f), _outputVolume(0) {
}

AudioPlayer::~AudioPlayer() {
    stopAll();
}

void AudioPlayer::play(const QString& filePath) {

    //qDebug() << "AudioPlayer: Play file:" << filePath;
    DWORD mon, out;
    monitorDevice->GetState(&mon);
    outputDevice->GetState(&out);
    if (mon != DEVICE_STATE_ACTIVE && out != DEVICE_STATE_ACTIVE)  {
        qDebug() << "AudioPlayer: Cannot play file, no monitor device or output device is valid";
        return;
    }

    if (mon != DEVICE_STATE_ACTIVE) qDebug() << "AudioPlayer: Warning, no monitor device is valid";
    if (out != DEVICE_STATE_ACTIVE) qDebug() << "AudioPlayer: Warning, no output device is vaild";

    auto* instance = new SoundInstance(filePath, monitorDevice, outputDevice, _monitorVolume, _outputVolume, this);
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

void AudioPlayer::setMonitorVolume(float volume) {
    _monitorVolume = volume;
    for (auto* instance : activeInstances) {
        instance->setMonitorVolume(volume);
    }
}

void AudioPlayer::setOutputVolume(float volume) {
    _outputVolume = volume;
    for (auto* instance : activeInstances) {
        instance->setOutputVolume(volume);
    }
}

void AudioPlayer::setMonitorDevice(QString id) {
    releaseMonitorDevice();
    if (id.isEmpty() || id == "None") return;
    HRESULT hr = WasapiManager::getInstance().getEnumerator()->GetDevice(id.toStdWString().c_str(), &monitorDevice);
    if (FAILED(hr)) {
        qDebug() << "AudioPlayer: Failed to set Monitor device";
    }
}

void AudioPlayer::setOutputDevice(QString id) {
    releaseOutputDevice();
    if (id.isEmpty() || id == "None") return;
    HRESULT hr = WasapiManager::getInstance().getEnumerator()->GetDevice(id.toStdWString().c_str(), &outputDevice);
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


