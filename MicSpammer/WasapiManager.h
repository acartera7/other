

#pragma once
#include <Audioclient.h>
#include <Mmdeviceapi.h>
#include <string>
#include <vector>

#ifndef WASPIMANAGER_H
#define WASPIMANAGER_H

struct AudioDeviceInfo {
    std::wstring id;       // Device ID string
    std::wstring name;     // Friendly name
    EDataFlow flow;        // eRender or eCapture
};

class WasapiManager {
public:
    static WasapiManager& getInstance();  // Singleton instance
    WasapiManager(const WasapiManager&) = delete;
    WasapiManager& operator=(const WasapiManager&) = delete;

    HRESULT initialize();
    void cleanup();
    [[nodiscard]] IMMDeviceEnumerator* getEnumerator() const;
    [[nodiscard]] const std::vector<AudioDeviceInfo>& getDevices() const;

    static WAVEFORMATEXTENSIBLE getMainFormat();

private:
    WasapiManager() = default;  // Private constructor (singleton)
    ~WasapiManager();

    IMMDeviceEnumerator* _pEnumerator = nullptr;
    std::vector<AudioDeviceInfo> deviceList;
};


#endif //WASPIMANAGER_H
