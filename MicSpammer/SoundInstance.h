//
// Created by Victus on 2/8/2026.
//

#ifndef MICSPAMMER_SOUNDINSTANCE_H
#define MICSPAMMER_SOUNDINSTANCE_H
#include <qtmetamacros.h>
#include <qobject.h>
#include <qqueue.h>
#include <audioclient.h>

#include "AudioLoader.h"
#include "AudioPlayer.h"

class SoundInstance : public QObject {
    Q_OBJECT
public:
    explicit SoundInstance(const QString& path, IAudioClient* client, QObject *parent);
    ~SoundInstance();

    void start();   // start loading + playback
    void stop();    // request stop

signals:
    void finished(SoundInstance *self); // clean up;

private slots:
    void onPcmReady(const QByteArray& data, const QAudioFormat& format);

private:
    void startPlaybackThread();
    void writeAudioData(IAudioRenderClient* renderClient);

    QString filePath;
    AudioLoader* loader = nullptr;

    //QQueue<QByteArray> pcmQueue; //TODO make audio streaming
    std::vector<BYTE> pcmData;
    WAVEFORMATEX wfx{};
    bool loadedOK = false;

    std::atomic<bool> stopFlag{false};
    QThread* playbackThread = nullptr;
    IAudioClient* audioClient = nullptr;

};


#endif //MICSPAMMER_SOUNDINSTANCE_H