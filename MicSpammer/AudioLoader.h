

#ifndef MICSPAMMER_AUDIOLOADER_H
#define MICSPAMMER_AUDIOLOADER_H
#include <QAudioDecoder>
#include <QAudioBuffer>
#include <QDebug>

class AudioLoader : public QObject {
    Q_OBJECT
public:
    explicit AudioLoader(QObject* parent = nullptr);
    void loadFile(const QString& path);
    void setDecoderFormat(const QAudioFormat &audioFormat);
    ~AudioLoader() override;

    signals:
    void pcmReady(const QByteArray& data, const QAudioFormat& format);

private slots:
    void onBufferReady();
    void onFinished();

private:
    QAudioDecoder* decoder;
    QByteArray accumulatedData;
    QAudioFormat _audioFormat;
};
#endif //MICSPAMMER_AUDIOLOADER_H