#include <Audioclient.h>
#include <Mmdeviceapi.h>
#include <iostream>
#include <string>
#include "MicSpammerWindow.h"

#include <QApplication>


HRESULT InitializeAudio() {
    IMMDeviceEnumerator* pEnumerator = nullptr;
    IMMDevice* pDevice = nullptr;
    IAudioClient* pAudioClient = nullptr;

    // Initialize COM
    CoInitialize(nullptr);
    // Get default audio device
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
                                  __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&pEnumerator));
    if (FAILED(hr)) return hr;

    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    if (FAILED(hr)) return hr;

    // Activate audio client
    hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,
                           reinterpret_cast<void**>(&pAudioClient));
    if (FAILED(hr)) return hr;

    WAVEFORMATEX* pwfx = nullptr;
    hr = pAudioClient->GetMixFormat(&pwfx);
    if (FAILED(hr)) return hr;

    // Initialize audio client
    hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0,
                                  10000000, 0, pwfx, nullptr);
    if (FAILED(hr)) return hr;

    std::cout << "Audio Initialized!" << std::endl;

    // Cleanup
    pEnumerator->Release();
    pDevice->Release();
    pAudioClient->Release();
    CoUninitialize();

    return hr;
}

int main(int argc, char *argv[]) {
    std::cout << ">>> Entered main()\n";  // Confirm reach
    QApplication a(argc, argv);
    MicSpammerWindow w;
    w.show();
    if (FAILED(InitializeAudio())) {
        std::cerr << "Failed to initialize audio." << std::endl;
        return -1;
    }
    return a.exec();
}
