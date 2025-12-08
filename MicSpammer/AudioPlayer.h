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
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <Audioclient.h>

#undef TIMECODE_SAMPLE

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H


struct LoadedWav {
    std::vector<BYTE> pcm;
    WAVEFORMATEX format;
    bool ok = false;
};

class AudioPlayer : public QObject {
    Q_OBJECT
public:
    explicit AudioPlayer(QObject *parent = nullptr);
    void play(const QString &filePath);
    void stop();
    void setVolume(float volume);

private:
    void loadAudioFile(const QString &filePath);
    void setupAudioStream();
    void writeAudioData();
    LoadedWav loadWavFile(const QString &filePath);

    IMFSourceReader* pSourceReader = nullptr;  // Media Foundation reader
    IMFMediaType* pMediaType = nullptr;
    IAudioClient* pAudioClient = nullptr;
    IAudioRenderClient* pRenderClient = nullptr;
};


#endif //AUDIOPLAYER_H
