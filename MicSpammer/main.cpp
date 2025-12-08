
#include <iostream>
#include "MicSpammerWindow.h"
#include "WasapiManager.h"
#include <QApplication>


int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    auto result = WasapiManager::getInstance().initialize();
    if (FAILED(result)) {
        std::cerr << "Failed to initialize audio." << std::endl;
        return -20;
    }

    MicSpammerWindow w;

    w.show();
    return a.exec();
}
