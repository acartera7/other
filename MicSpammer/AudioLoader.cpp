

#include "AudioLoader.h"

#include <qurl.h>

AudioLoader::AudioLoader(QObject *parent) {
    decoder = new QAudioDecoder(this);

    connect(decoder, &QAudioDecoder::bufferReady, this, &AudioLoader::onBufferReady);
    connect(decoder, &QAudioDecoder::finished, this, &AudioLoader::onFinished);

}

void AudioLoader::loadFile(const QString &path) {
    qDebug() << QString("Loading file: %1.").arg(path);
    decoder->setSource(QUrl::fromLocalFile(path));
    decoder->start();
}

void AudioLoader::setDecoderFormat(const QAudioFormat &audioFormat) {
    decoder->setAudioFormat(audioFormat);
    if (!decoder->audioFormat().isValid()) {
        qDebug() << "Decoder rejected format, will fall back.";
    }
}

AudioLoader::~AudioLoader() {
    delete decoder;
}

void AudioLoader::onBufferReady() {
    QAudioBuffer buffer = decoder->read();
    if (!buffer.isValid()) return;

    //qDebug() << "Buffer format:"
    //     << buffer.format().sampleRate()
    //     << "Hz,"
    //     << buffer.format().channelCount()
    //     << "ch,"
    //     << buffer.format().bytesPerSample()*8
    //     << "bits,"
    //     << "byteCount=" << buffer.byteCount();

    QByteArray pcm(reinterpret_cast<const char*>(buffer.constData<char>()),
                   buffer.byteCount());

    accumulatedData.append(pcm);
    if (!_audioFormat.isValid())
        _audioFormat = buffer.format();
}

void AudioLoader::onFinished() {
    qDebug() << "File Loaded.";
    emit pcmReady(accumulatedData, _audioFormat);
    accumulatedData.clear();
}
