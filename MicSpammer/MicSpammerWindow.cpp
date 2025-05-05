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

    AudioPlayer *audioPlayer = new AudioPlayer(this);

    // Toolbar & Button
    toolbar = new QToolBar(this);
    playButton = new QPushButton("Play", this);
    stopButton = new QPushButton("Stop", this);
    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100); // Volume range 0-100
    volumeSlider->setFixedWidth(100);
    volumeSlider->setValue(80);
    openFolderButton = new QPushButton("Open Folder", this);

    // Spacer Widget (Flexible Space)
    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // toolbar layout container
    QWidget *rightContainer  = new QWidget(this);
    QHBoxLayout *rightLayout  = new QHBoxLayout();
    rightLayout->addWidget(playButton);
    rightLayout->addSpacing(10); // Spacing between play & stop
    rightLayout->addWidget(stopButton);
    rightLayout->addSpacing(15); // Spacing between stop & volume slider
    rightLayout->addWidget(volumeSlider);
    rightLayout->setContentsMargins(0, 0, 0, 0); // Removes extra margins
    rightContainer->setLayout(rightLayout);

    // Add widgets to toolbar
    toolbar->addWidget(openFolderButton);
    toolbar->addWidget(spacer);
    toolbar->addWidget(rightContainer);
    addToolBar(Qt::TopToolBarArea, toolbar);
    //QDir(QDir::homePath()).filePath("Music")
    browser = new FileBrowserWidget(this);

    mainLayout->addWidget(toolbar);
    mainLayout->addWidget(browser);
    centralWidget()->setLayout(mainLayout);

    // Toolbar buttons actions
    connect(openFolderButton, &QPushButton::clicked, this, &MicSpammerWindow::onOpenFolder);
    connect(playButton, &QPushButton::clicked, this, &MicSpammerWindow::onPlay);
    connect(stopButton, &QPushButton::clicked, this, &MicSpammerWindow::onStop);
    connect(volumeSlider, &QSlider::valueChanged, this, &MicSpammerWindow::onVolumeChanged);

    // File actions
    connect(browser, &FileBrowserWidget::fileSelected, this, &MicSpammerWindow::onFileSelected);
    connect(browser, &FileBrowserWidget::playSound, this, &MicSpammerWindow::onPlay);

}

void MicSpammerWindow::onOpenFolder() {
    QString folder = QFileDialog::getExistingDirectory(this, "Select a folder", QDir::homePath());
    if (!folder.isEmpty()) {
        browser->setRootDirectory(folder);  // Update browser widget
    }
}

void MicSpammerWindow::onPlay() {
    if (!selectedFilePath.isEmpty()) {
        audioPlayer->play(selectedFilePath);  // Play last selected file
        qDebug() << "Play file:" << selectedFilePath;
    }
}

void MicSpammerWindow::onStop() {
    audioPlayer->stop();
}

void MicSpammerWindow::onVolumeChanged(int volume) {
    audioPlayer->setVolume(volume / 100.0f);  // Scale to 0-1
}

void MicSpammerWindow::onFileSelected(const QString &filePath) {
    selectedFilePath = filePath;
}


MicSpammerWindow::~MicSpammerWindow() = default;