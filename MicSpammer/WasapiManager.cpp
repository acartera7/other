//
// Created by Andrei on 5/4/2025.
//

#include "WasapiManager.h"
#include <setupapi.h>
#include <initguid.h>  // Put this in to get rid of linker errors.
#include <Functiondiscoverykeys_devpkey.h>
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

    // Enumerate both render and capture devices
    for (EDataFlow flow : { eRender, eCapture }) {
        IMMDeviceCollection* pDevices = nullptr;
        hr = pEnumerator->EnumAudioEndpoints(flow, DEVICE_STATE_ACTIVE, &pDevices);
        if (SUCCEEDED(hr)) {
            UINT count = 0;
            pDevices->GetCount(&count);

            for (UINT i = 0; i < count; i++) {
                IMMDevice* pDev = nullptr;
                if (SUCCEEDED(pDevices->Item(i, &pDev))) {
                    LPWSTR deviceId = nullptr;
                    pDev->GetId(&deviceId);

                    IPropertyStore* pProps = nullptr;
                    if (SUCCEEDED(pDev->OpenPropertyStore(STGM_READ, &pProps))) {
                        PROPVARIANT varName;
                        PropVariantInit(&varName);
                        if (SUCCEEDED(pProps->GetValue(PKEY_Device_FriendlyName, &varName))) {
                            AudioDeviceInfo info;
                            info.id = deviceId;
                            info.name = varName.pwszVal;
                            info.flow = flow;
                            deviceList.push_back(info);

                            std::wcout << L"Device: " << info.name << L" (ID=" << info.id << L")" << std::endl;
                        }
                        PropVariantClear(&varName);
                        pProps->Release();
                    }
                    CoTaskMemFree(deviceId);
                    pDev->Release();
                }
            }
            pDevices->Release();
        }
    }

    // initialize current output device with the default
    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pCurrentDevice);
    if (FAILED(hr)) return hr;

    hr = pCurrentDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,
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
    if (pCurrentDevice) pCurrentDevice->Release();
    if (pEnumerator) pEnumerator->Release();
    CoUninitialize();
}

HRESULT WasapiManager::setDeviceById(const std::wstring& deviceId)
{
    if (!pEnumerator) return E_FAIL;

    IMMDevice* pNewDevice = nullptr;
    HRESULT hr = pEnumerator->GetDevice(deviceId.c_str(), &pNewDevice);
    if (FAILED(hr)) return hr;

    IAudioClient* pNewClient = nullptr;
    hr = pNewDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,
                              reinterpret_cast<void**>(&pNewClient));
    if (FAILED(hr)) {
        pNewDevice->Release();
        return hr;
    }

    // Release old
    if (pAudioClient) pAudioClient->Release();
    if (pCurrentDevice) pCurrentDevice->Release();

    // Update current
    pCurrentDevice = pNewDevice;
    pAudioClient = pNewClient;

    return S_OK;
}

std::wstring WasapiManager::getCurrentDeviceName() const {
    if (!pCurrentDevice) return L"(no device)";

    IPropertyStore* pProps = nullptr;
    HRESULT hr = pCurrentDevice->OpenPropertyStore(STGM_READ, &pProps);
    if (FAILED(hr)) return L"(error)";

    PROPVARIANT varName;
    PropVariantInit(&varName);
    hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
    std::wstring name = L"(unknown)";
    if (SUCCEEDED(hr)) {
        name = varName.pwszVal;
    }
    PropVariantClear(&varName);
    pProps->Release();

    return name;
}

WasapiManager::~WasapiManager() {
    cleanup();
}

IAudioClient* WasapiManager::getAudioClient() const { return pAudioClient; }
const std::vector<AudioDeviceInfo>& WasapiManager::getDevices() const { return deviceList; }