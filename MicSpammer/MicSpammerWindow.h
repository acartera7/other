//
// Created by Andrei on 4/29/2025.
//
#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QBoxLayout>
#include <QToolBar>
#include <QPushButton>
#include <QFileDialog>
#include <QSlider>
#include <QFileSystemModel>
#include <QSplitter>
#include <QComboBox>
#include <QJsonObject>
#include <QMessageBox>

#include "FileBrowserWidget.h"
#include "NumpadWidget.h"
#include "AudioPlayer.h"
#include "MicCapture.h"
#include "ProfileManager.h"
#include "WasapiManager.h"


#ifndef MICSPAMMERWINDOW_H
#define MICSPAMMERWINDOW_H

class MicSpammerWindow final : public QMainWindow {
Q_OBJECT
public:
    explicit MicSpammerWindow(QWidget *parent = nullptr);
    ~MicSpammerWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    //void focusInEvent(QFocusEvent *event) override;
    //void focusOutEvent(QFocusEvent *event) override;

private:
    int _window_x, _window_y;
    QString currentProfilePath;

// Handler Functions
    void onOpenFolder();
    void onPlay();
    void onStop();
    void onVolumeChanged(QString name, int volume);
    void onFileSelected(const QString &filePath);
    void onLoadProfile();
    void onSaveProfile();
    void onDeleteProfile();
    void onReset();
    void resetProfileSettings();

    void onMicDeviceChanged(int index);
    void onMonitorDeviceChanged(int index);
    void onSendDeviceChanged(int index);

    QString selectedFilePath;
    AudioPlayer& audioPlayer;
    MicCapture& micCapture;
    ProfileManager& profileManager;
    FileBrowserWidget *browser;
    NumpadWidget *numpad;

    // GUI Elements
    QWidget     *mainWidget;
    QVBoxLayout *mainVLayout;

    QToolBar *toolbar;
    QToolBar *profile_toolbar;

    QPushButton *openFolderButton, *playButton, *stopButton, *loadProfileButton, *saveProfileButton, *deleteProfileButton, *resetButton;
    QWidget *toolbarSpacer, *profileSpacer, *toolbar_rightContainer, *toolbar_devicesContainer, *mainContent_container;
    QHBoxLayout *toolbar_rightHLayout, *mainContent_HLayout;
    QSlider *micVolumeSlider, *monitorVolumeSlider, *sendVolumeSlider;
    QSplitter *mainContent_splitter;

    QLabel* micDeviceLabel, *monitorDeviceLabel, *sendDeviceLabel, *profileLabel;

    QComboBox* micComboBox,*monitorComboBox, *sendComboBox;

    QGridLayout *toolbar_devicesGridLayout;

    std::vector<AudioDeviceInfo> deviceList;
};



#endif //MICSPAMMERWINDOW_H
