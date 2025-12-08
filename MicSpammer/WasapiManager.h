//
// Created by Andrei on 5/4/2025.
//
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
    HRESULT initialize();
    void cleanup();

    IAudioClient* getAudioClient() const;
    const std::vector<AudioDeviceInfo>& getDevices() const;
    HRESULT setDeviceById(const std::wstring& deviceId);
    std::wstring getCurrentDeviceName() const;

private:
    WasapiManager() = default;  // Private constructor (singleton)
    ~WasapiManager();

    IMMDeviceEnumerator* pEnumerator = nullptr;
    IMMDevice* pCurrentDevice = nullptr;
    IAudioClient* pAudioClient = nullptr;

    std::vector<AudioDeviceInfo> deviceList;
};


#endif //WASPIMANAGER_H
