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
#include "FileBrowserWidget.h"



#ifndef MICSPAMMERWINDOW_H
#define MICSPAMMERWINDOW_H

class MicSpammerWindow final : public QMainWindow {
Q_OBJECT
public:
    explicit MicSpammerWindow(QWidget *parent = nullptr);
    ~MicSpammerWindow();
private:
    int _window_x, _window_y;

    // Handler Functions
    void onOpenFolder();
    // GUI Elements
    QWidget     *mainWidget;
    QVBoxLayout *mainLayout;
    FileBrowserWidget *browser;
    QToolBar *toolbar;
    QPushButton *openFolderButton;
};

#endif //MICSPAMMERWINDOW_H
