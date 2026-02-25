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

    qDebug() << "AudioPlayer: Play file:" << filePath;
    //TODO make sure device and clients are valid
    auto* instance = new SoundInstance(filePath, monitorDevice, _volume, this);
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
    releaseMonitorClient();

    if (id.empty()) return;

    IMMDevice* device = nullptr;
    HRESULT hr = WasapiManager::getInstance().getEnumerator()->GetDevice(id.c_str(), &device);
    if (FAILED(hr)) {
        qDebug() << "AudioPlayer: Failed to set Monitor device";
        return;
    }
    initOutputClient(device);
}

void AudioPlayer::setOutputDevice(const std::wstring &id) {
    releaseMonitorClient();

    if (id.empty()) return;

    IMMDevice* device = nullptr;
    HRESULT hr = WasapiManager::getInstance().getEnumerator()->GetDevice(id.c_str(), &device);
    if (FAILED(hr)) {
        qDebug() << "AudioPlayer: Failed to set Output device";
        return;
    }
    initOutputClient(device);
}

void AudioPlayer::releaseMonitorClient() {
    if (monitorRenderClient) { monitorRenderClient->Release(); monitorRenderClient = nullptr; }
    if (monitorAudioClient) { monitorAudioClient->Release(); monitorAudioClient = nullptr; }
    if (monitorDevice) { monitorDevice->Release(); monitorDevice = nullptr; }
}

void AudioPlayer::releaseOutputClient() {
    if (outputRenderClient) { outputRenderClient->Release(); outputRenderClient = nullptr; }
    if (outputAudioClient) { outputAudioClient->Release(); outputAudioClient = nullptr; }
    if (outputDevice) { outputDevice->Release(); outputDevice = nullptr; }
}

void AudioPlayer::initMonitorClient(IMMDevice *device) {
    HRESULT hr = device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,
                                  (void**)&monitorAudioClient);
    if (FAILED(hr) || !monitorAudioClient) {
        qDebug() << "AudioPlayer: Failed to activate monitorAudioClient.";
        return;
    }

    WAVEFORMATEX* mixFormat = nullptr;
    hr = monitorAudioClient->GetMixFormat(&mixFormat);
    if (FAILED(hr) || !mixFormat) {
        qDebug() << "AudioPlayer: Failed to get mix format in initMonitorClient.";
        return;
    }

    REFERENCE_TIME bufferDuration = 100000;
    hr = monitorAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
                                        0,
                                        bufferDuration,
                                        0,
                                        mixFormat,
                                        nullptr);
    if (FAILED(hr)) {
        qDebug() << "AudioPlayer: Failed to initialize monitorAudioClient.";
        CoTaskMemFree(mixFormat);
        return;
    }

    hr = monitorAudioClient->GetService(__uuidof(IAudioRenderClient),
                                        (void**)&monitorRenderClient);
    if (FAILED(hr) || !monitorRenderClient) {
        qDebug() << "AudioPlayer: Failed to get monitorRenderClient.";
    }

    CoTaskMemFree(mixFormat);
}

void AudioPlayer::initOutputClient(IMMDevice *device) {
    HRESULT hr = device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,
                                    (void**)&outputAudioClient);
    if (FAILED(hr) || !outputAudioClient) {
        qDebug() << "AudioPlayer: Failed to activate outputAudioClient.";
        return;
    }

    WAVEFORMATEX* mixFormat = nullptr;
    hr = outputAudioClient->GetMixFormat(&mixFormat);
    if (FAILED(hr) || !mixFormat) {
        qDebug() << "AudioPlayer: Failed to get mix format in initOutputClient.";
        return;
    }

    REFERENCE_TIME bufferDuration = 100000;
    hr = outputAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
                                       0,
                                       bufferDuration,
                                       0,
                                       mixFormat,
                                       nullptr);
    if (FAILED(hr)) {
        qDebug() << "AudioPlayer: Failed to initialize outputAudioClient.";
        CoTaskMemFree(mixFormat);
        return;
    }

    hr = outputAudioClient->GetService(__uuidof(IAudioRenderClient),
                                       (void**)&outputRenderClient);
    if (FAILED(hr) || !outputRenderClient) {
        qDebug() << "AudioPlayer: Failed to get monitorRenderClient.";
    }

    CoTaskMemFree(mixFormat);
}
