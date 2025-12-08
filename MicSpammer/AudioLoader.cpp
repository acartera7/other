//
// Created by Victus on 12/8/2025.
//

#include "AudioLoader.h"

#include <qurl.h>

AudioLoader::AudioLoader(QObject *parent) {
    decoder = new QAudioDecoder(this);

    connect(decoder, &QAudioDecoder::bufferReady, this, &AudioLoader::onBufferReady);
    connect(decoder, &QAudioDecoder::finished, this, &AudioLoader::onFinished);

}

void AudioLoader::loadFile(const QString &path) {
    decoder->setSource(QUrl(path));
    decoder->start();
}

void AudioLoader::onBufferReady() {
    QAudioBuffer buffer = decoder->read();
    if (!buffer.isValid()) return;

    QByteArray pcm(reinterpret_cast<const char*>(buffer.constData<char>()),
                   buffer.byteCount());

    emit pcmReady(pcm, buffer.format());
}

void AudioLoader::onFinished() {
    qDebug() << "Decoding finished.";
}
