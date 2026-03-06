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