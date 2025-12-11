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
    explicit AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer() override;
    void play(const QString &filePath);
    void stop();
    void setVolume(float volume);
    void loadAudioFile(const QString &filePath);


private:
    void setupAudioStream();
    void writeAudioData();

    IAudioClient* pAudioClient = nullptr;
    IAudioRenderClient* pRenderClient = nullptr;
    AudioLoader *loader;
    LoadedWav loadedFile;

};


#endif //AUDIOPLAYER_H
