//
// Created by Victus on 2/8/2026.
//

#ifndef MICSPAMMER_SOUNDINSTANCE_H
#define MICSPAMMER_SOUNDINSTANCE_H
#include <qtmetamacros.h>
#include <qobject.h>
#include <qqueue.h>
#include <audioclient.h>
#include <Mmdeviceapi.h>
#include <qaudioformat.h>

#include <QThread>
#include <QMutex>
#include <atomic>

#include  "AudioLoader.h"

class SoundInstance : public QObject {
    Q_OBJECT
public:
    explicit SoundInstance(const QString& path, IMMDevice* monitorDevice, IMMDevice* outputDevice, float volume, QObject *parent);
    ~SoundInstance() override;

    void start();   // start loading + playback
    void stop();    // request stop
    QString getFileName();
    void setVolume(float volume);

signals:
    void finished(SoundInstance *self); // clean up;

private slots:
    void onPcmReady(const QByteArray& data);

private:
    HRESULT initMonitorAudioClient();
    HRESULT initOutputAudioClient();

    void startPlaybackThread();
    void writeAudioData(IAudioClient *audioClient, IAudioRenderClient *renderClient);

    QString filePath;
    AudioLoader* loader = nullptr;

    // =========== PRELOAD MODEL =========== (current implementation)
    std::vector<BYTE> pcmData;
    bool loadedOK = false;

    // =========== STREAMING MODEL =========== //TODO make audio streaming
    //QQueue<QByteArray> pcmQueue;
    //QMutex queueMutex
    // Format will be set once from the first buffer
    // QAudioFormat streamFormat;
    // bool streamingMode = false; // flag to switch between preload vs streaming

    std::atomic<bool> stopFlag;
    std::atomic<float> _volume;

    QThread* playbackThreadMonitor = nullptr;
    QThread* playbackThreadOutput = nullptr;

    IAudioClient* monitorAudioClient = nullptr;
    IAudioClient* outputAudioClient = nullptr;

    QAudioFormat _qFormat;

    UINT32 bytesPerFrame = 0;
    WAVEFORMATEX *monitorAudioFmt = nullptr;
    WAVEFORMATEX *outputAudioFmt = nullptr;
};


#endif //MICSPAMMER_SOUNDINSTANCE_H