//
// Created by Victus on 3/6/2026.
//

#include "ProfileManager.h"

ProfileManager& ProfileManager::getInstance() {
    static ProfileManager instance;
    return instance;
}

void ProfileManager::saveProfile(const QString& name,
                                 MicCapture& mic,
                                 AudioPlayer& player,
                                 FileBrowserWidget& browser,
                                 NumpadWidget& numpad,
                                 const QRect& windowGeometry) {
    //QJsonObject root;
    //root["mic"] = mic.saveState();
    //root["player"] = player.saveState();
    //root["browser"] = browser.saveState();
    //root["numpad"] = numpad.saveState();
    //root["window"] = QJsonObject {
    //    {"x", windowGeometry.x()},
    //    {"y", windowGeometry.y()},
    //    {"w", windowGeometry.width()},
    //    {"h", windowGeometry.height()}
    //};

    //QDir().mkpath("saves");
    //QFile file("saves/" + name + ".json");
    //if (file.open(QIODevice::WriteOnly)) {
    //    file.write(QJsonDocument(root).toJson());
    //}

}

void ProfileManager::loadProfile(const QString& path,
                 MicCapture& mic,
                 AudioPlayer& player,
                 FileBrowserWidget& browser,
                 NumpadWidget& numpad,
                 const QRect& windowGeometry) {

}

QStringList ProfileManager::listProfiles() {
    return QStringList();
}