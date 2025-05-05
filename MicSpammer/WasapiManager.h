//
// Created by Andrei on 5/4/2025.
//
#include <Audioclient.h>
#include <Mmdeviceapi.h>

#ifndef WASPIMANAGER_H
#define WASPIMANAGER_H


class WasapiManager {
public:
    static WasapiManager& getInstance();  // Singleton instance
    HRESULT initialize();
    void cleanup();
    IMMDevice* getAudioDevice() const;
    IAudioClient* getAudioClient() const;

private:
    WasapiManager() = default;  // Private constructor (singleton)
    ~WasapiManager();

    IMMDeviceEnumerator* pEnumerator = nullptr;
    IMMDevice* pDevice = nullptr;
    IAudioClient* pAudioClient = nullptr;
};


#endif //WASPIMANAGER_H
