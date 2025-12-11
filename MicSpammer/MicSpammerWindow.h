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

#include "FileBrowserWidget.h"
#include "NumpadWidget.h"
#include "AudioPlayer.h"
#include "AudioLoader.h"
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
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    int _window_x, _window_y;

    // Handler Functions
    void onOpenFolder();
    void onPlay();
    void onStop();
    void onVolumeChanged(int volume);
    void onFileSelected(const QString &filePath);

    QString selectedFilePath;
    AudioPlayer *audioPlayer;

    // GUI Elements
    QWidget     *mainWidget;
    QVBoxLayout *mainVLayout;

    FileBrowserWidget *browser;
    NumpadWidget *numpad;
    QToolBar *toolbar;

    QPushButton *openFolderButton, *playButton, *stopButton;
    QWidget *spacer, *toolbar_rightContainer, *mainContent_container;
    QHBoxLayout *toolbar_rightHLayout, *mainContent_HLayout;
    QSlider *volumeSlider;
    QSplitter *mainContent_splitter;
    QLabel *deviceLabel;
};



#endif //MICSPAMMERWINDOW_H
