//
// Created by Victus on 3/6/2026.
//
#pragma once
#include "MicCapture.h"
#include "AudioPlayer.h"
#include "FileBrowserWidget.h"
#include "NumpadWidget.h"

#ifndef MICSPAMMER_PROFILEMANAGER_H
#define MICSPAMMER_PROFILEMANAGER_H


class ProfileManager {
public:
    static ProfileManager& getInstance();

    void saveProfile(const QString& name,
                            MicCapture& mic,
                            AudioPlayer& player,
                            FileBrowserWidget& browser,
                            NumpadWidget& numpad,
                            const QRect& windowGeometry);

    void loadProfile(const QString& path,
                     MicCapture& mic,
                     AudioPlayer& player,
                     FileBrowserWidget& browser,
                     NumpadWidget& numpad,
                     const QRect& windowGeometry);

    QStringList listProfiles();

private:
    ProfileManager() = default;
    ~ProfileManager() = default;
    ProfileManager(const ProfileManager&) = delete;
    ProfileManager& operator=(const ProfileManager&) = delete;
};


#endif //MICSPAMMER_PROFILEMANAGER_H