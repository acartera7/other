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
                                  __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&_pEnumerator));
    if (FAILED(hr)) return hr;

    // Enumerate both render and capture devices
    for (EDataFlow flow : { eRender, eCapture }) {
        IMMDeviceCollection* pDevices = nullptr;
        hr = _pEnumerator->EnumAudioEndpoints(flow, DEVICE_STATE_ACTIVE, &pDevices);
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

    std::cout << "WASAPI Initialized" << std::endl;
    return hr;
}

void WasapiManager::cleanup() {
    if (_pEnumerator) _pEnumerator->Release();
    CoUninitialize();
}

IMMDeviceEnumerator* WasapiManager::getEnumerator() const {
    return _pEnumerator;
}

WasapiManager::~WasapiManager() {
    cleanup();
}

WAVEFORMATEXTENSIBLE WasapiManager::getMainFormat() {
    WAVEFORMATEXTENSIBLE wfext = {};
    wfext.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
    wfext.Format.nChannels = 2;
    wfext.Format.nSamplesPerSec = 48000;
    wfext.Format.wBitsPerSample = 32;
    wfext.Format.nBlockAlign = wfext.Format.nChannels * (wfext.Format.wBitsPerSample / 8);
    wfext.Format.nAvgBytesPerSec = wfext.Format.nSamplesPerSec * wfext.Format.nBlockAlign;
    wfext.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
    wfext.Samples.wValidBitsPerSample = 32;
    wfext.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT;
    wfext.SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;
    return wfext;
}

const std::vector<AudioDeviceInfo>& WasapiManager::getDevices() const { return deviceList; }
