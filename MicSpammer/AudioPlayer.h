//
// Created by Andrei on 5/4/2025.
//
#pragma once
#include <Audioclient.h>
#include <QThread>

#include <Mmdeviceapi.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

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

    IMFSourceReader* pSourceReader = nullptr;  // Media Foundation reader
    IMFMediaType* pMediaType = nullptr;
    IAudioClient* pAudioClient = nullptr;
    IAudioRenderClient* pRenderClient = nullptr;
};


#endif //AUDIOPLAYER_H
