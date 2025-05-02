//
// Created by Andrei on 4/29/2025.
//
#include "gui.h"

MicSpammerWindow::MicSpammerWindow(QWidget *parent)
    : QMainWindow(parent), _window_x(500),_window_y(300),
    mainLayout(), top_barHLayout(), btn1(), btn2(), btn3(){

    mainWidget = new QWidget();
    setCentralWidget(mainWidget);

    mainLayout = new QHBoxLayout();
    setWindowTitle("MicSpammer");
    setGeometry(100,100, _window_x, _window_y);
    top_barHLayout = new QHBoxLayout();
    top_barHLayout->setSpacing(5);
    btn1 = new QPushButton("MicSpammer1");
    btn2 = new QPushButton("MicSpammer1");
    btn3 = new QPushButton("MicSpammer1");
    top_barHLayout->addWidget(btn1);
    top_barHLayout->addWidget(btn2);
    top_barHLayout->addWidget(btn3);

    centralWidget()->setLayout(mainLayout);

    /*
    setStyleSheet(R"(
    QWidget {
        background-color: white;
        color: black;
        font-family: Segoe UI;
    }
    QPushButton {
        background-color: lightgray;
        border: 1px solid gray;
    }
    QMainWindow {
        background-color: darkgray;
    }
    )");*/
}

MicSpammerWindow::~MicSpammerWindow() {
    delete btn1;
    delete btn2;
    delete btn3;
    delete top_barHLayout;
    delete mainLayout;
    delete mainWidget;

}
