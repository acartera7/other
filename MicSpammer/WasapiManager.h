//
// Created by Andrei on 5/4/2025.
//

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
    HRESULT initialize();
    void cleanup();

    const std::vector<AudioDeviceInfo>& getDevices() const;
    IMMDevice* getCurrentDevice() const;
    HRESULT setDeviceById(const std::wstring& deviceId);
    std::wstring getCurrentDeviceName() const;

private:
    WasapiManager() = default;  // Private constructor (singleton)
    ~WasapiManager();

    IMMDeviceEnumerator* _pEnumerator = nullptr;
    IMMDevice* _pCurrentDevice = nullptr;

    std::vector<AudioDeviceInfo> deviceList;
};


#endif //WASPIMANAGER_H
