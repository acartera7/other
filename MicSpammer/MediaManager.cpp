//
// Created by Andrei on 5/5/2025.
//

#include "MediaManager.h"


MediaManager& MediaManager::getInstance() {
    static MediaManager instance;
    return instance;
}

void MediaManager::preloadFile(const QString& filePath) {
    IMFSourceReader* pReader = nullptr;
    HRESULT hr = MFCreateSourceReaderFromURL(filePath.toStdWString().c_str(), nullptr, &pReader);
    if (FAILED(hr)) return;

    IMFSample* pSample = nullptr;
    DWORD flags = 0;
    hr = pReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flags, nullptr, &pSample);
    if (FAILED(hr)) return;

    IMFMediaBuffer* pBuffer = nullptr;
    hr = pSample->ConvertToContiguousBuffer(&pBuffer);
    if (FAILED(hr)) return;

    // Lock buffer to access audio data
    BYTE* pData = nullptr;
    DWORD bufferSize = 0;
    hr = pBuffer->Lock(&pData, nullptr, &bufferSize);
    if (FAILED(hr)) return;


    cachedFiles[filePath.toStdString()] = { pData, bufferSize };
}

IMFSourceReader* MediaManager::getFileStream(const QString& filePath) {
    if (streamFiles.find(filePath.toStdString()) != streamFiles.end()) {
        return streamFiles[filePath.toStdString()];  // ✅ Return existing stream
    }

    IMFSourceReader* pReader = nullptr;
    HRESULT hr = MFCreateSourceReaderFromURL(filePath.toStdWString().c_str(), nullptr, &pReader);
    if (SUCCEEDED(hr)) {
        streamFiles[filePath.toStdString()] = pReader;  // ✅ Cache stream for reuse
    }

    return pReader;
}

void MediaManager::cleanup() {
    for (auto& entry : cachedFiles) {
        delete[] entry.second.buffer;
    }
    cachedFiles.clear();

    for (auto& entry : streamFiles) {
        entry.second->Release();
    }
    streamFiles.clear();
}

MediaManager::~MediaManager() {
    cleanup();
}


