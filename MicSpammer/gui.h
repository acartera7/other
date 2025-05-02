//
// Created by Andrei on 4/29/2025.
//
#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>


#ifndef GUI_H
#define GUI_H

class MicSpammerWindow final : public QMainWindow {
Q_OBJECT
public:
    explicit MicSpammerWindow(QWidget *parent = nullptr);
    ~MicSpammerWindow();
private:
    int _window_x, _window_y;

    // GUI Elements
    QWidget     *mainWidget;
    QHBoxLayout *mainLayout;
    QHBoxLayout *top_barHLayout;
    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
};

#endif //GUI_H
