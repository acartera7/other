
#pragma once

#include <QString>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <unordered_map>

#ifndef MEDIAMANAGER_H
#define MEDIAMANAGER_H

class MediaManager {
public:
    static MediaManager& getInstance();  // Singleton instance
    void preloadFile(const QString& filePath);  // Load into memory
    IMFSourceReader* getFileStream(const QString& filePath);  // Stream from disk
    void cleanup();  // Cleanup resources
    MediaManager(MediaManager&&) = delete;  // Prevent moving (OK)
    MediaManager(const MediaManager&) = delete;  // Prevent copying (OK)
    MediaManager& operator=(const MediaManager&) = delete;  // Prevent assignment (OK)

private:
    MediaManager() = default;
    ~MediaManager();

    struct AudioData {
        BYTE* buffer;   // Stored raw PCM data
        DWORD size;     // Size of audio buffer
        AudioData() = default;  // Default constructor
        AudioData(BYTE* buf, DWORD sz) : buffer(buf), size(sz) {}
    };

    std::unordered_map<std::string, AudioData> cachedFiles;
    std::unordered_map<std::string, IMFSourceReader*> streamFiles;
};

#endif // MEDIAMANAGER_H