//
// Created by Andrei on 5/4/2025.
//
#pragma once
#include <QWidget>
#include <QFile>
#include <QDataStream>

#include <QAudioDecoder>
#include <QAudioBuffer>
#include <QDebug>
#include <QThread>

#include <vector>
#include <cstring>

#define NO_DSHOW_STRSAFE

#include <Mmdeviceapi.h>
#include <Audioclient.h>

#include "AudioLoader.h"
#include "WasapiManager.h"

#undef TIMECODE_SAMPLE

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H


struct LoadedWav {
    std::vector<BYTE> data;
    WAVEFORMATEX format;
    bool ok = false;
};

class AudioPlayer : public QObject {
    Q_OBJECT
public:
    static AudioPlayer& getInstance();

    AudioPlayer(const AudioPlayer&) = delete;
    AudioPlayer& operator=(const AudioPlayer&) = delete;

    void play(const QString &filePath);
    void stop();
    void setVolume(float volume);
    void loadAudioFile(const QString &filePath);


private:
    AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer() override;
    void setupAudioStream();
    void writeAudioData(IAudioRenderClient* renderClient = nullptr);

    IAudioClient* _pAudioClient = nullptr;
    IAudioRenderClient* _pRenderClient = nullptr;
    AudioLoader *loader;
    LoadedWav loadedFile;

    std::vector<QThread*> activeThreads;      // keep track of threads playing sound
    std::atomic<bool> stopAllAudio{false};  // shared flag for stopping audio

};


#endif //AUDIOPLAYER_H
