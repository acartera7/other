
#include <iostream>
#include "MicSpammerWindow.h"
#include "WasapiManager.h"
#include <QApplication>


int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    MicSpammerWindow w;

    w.show();
    auto result = WasapiManager::getInstance().initialize();
    if (FAILED(result)) {
        std::cerr << "Failed to initialize audio." << std::endl;
        return -20;
    }
    return a.exec();
}
