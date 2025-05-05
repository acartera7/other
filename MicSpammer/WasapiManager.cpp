//
// Created by Andrei on 5/4/2025.
//

#include "WasapiManager.h"
#include <iostream>

WasapiManager& WasapiManager::getInstance() {
    static WasapiManager instance;
    return instance;
}

HRESULT WasapiManager::initialize() {
    CoInitialize(nullptr);
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
                                  __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&pEnumerator));
    if (FAILED(hr)) return hr;

    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    if (FAILED(hr)) return hr;

    hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,
                           reinterpret_cast<void**>(&pAudioClient));
    if (FAILED(hr)) return hr;

    WAVEFORMATEX* pwfx = nullptr;
    hr = pAudioClient->GetMixFormat(&pwfx);
    if (FAILED(hr)) return hr;

    hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 10000000, 0, pwfx, nullptr);
    if (FAILED(hr)) return hr;

    std::cout << "WASAPI Initialized" << std::endl;
    return hr;
}

void WasapiManager::cleanup() {
    if (pAudioClient) pAudioClient->Release();
    if (pDevice) pDevice->Release();
    if (pEnumerator) pEnumerator->Release();
    CoUninitialize();
}

WasapiManager::~WasapiManager() {
    cleanup();
}

IMMDevice* WasapiManager::getAudioDevice() const { return pDevice; }
IAudioClient* WasapiManager::getAudioClient() const { return pAudioClient; }
