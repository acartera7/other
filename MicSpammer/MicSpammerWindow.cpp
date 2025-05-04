//
// Created by Andrei on 4/29/2025.
//
#include "MicSpammerWindow.h"
#include <QFileSystemModel>
#include <QSplitter>

MicSpammerWindow::MicSpammerWindow(QWidget *parent)
    : QMainWindow(parent), _window_x(800),_window_y(500), mainLayout(){

    mainWidget = new QWidget();
    setCentralWidget(mainWidget);

    mainLayout = new QVBoxLayout();
    setWindowTitle("MicSpammer");
    setGeometry(100,100, _window_x, _window_y);

    // Toolbar & Button
    toolbar = new QToolBar(this);
    openFolderButton = new QPushButton("Open Folder", this);
    toolbar->addWidget(openFolderButton);
    addToolBar(Qt::TopToolBarArea, toolbar);
    //QDir(QDir::homePath()).filePath("Music")
    browser = new FileBrowserWidget(this);

    mainLayout->addWidget(toolbar);
    mainLayout->addWidget(browser);
    centralWidget()->setLayout(mainLayout);

    // Connect button to folder selection
    connect(openFolderButton, &QPushButton::clicked, this, &MicSpammerWindow::onOpenFolder);

    connect(browser, &FileBrowserWidget::folderSelected, this, [](const QString &path){
        qDebug() << "Main window received folder:" << path;
    });
}

void MicSpammerWindow::onOpenFolder() {
    QString folder = QFileDialog::getExistingDirectory(this, "Select a folder", QDir::homePath());
    if (!folder.isEmpty()) {
        browser->setRootDirectory(folder);  // Update browser widget
    }
}

MicSpammerWindow::~MicSpammerWindow() = default;