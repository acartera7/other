//
// Created by Andrei on 4/29/2025.
//
#include "MicSpammerWindow.h"
#include <QFileSystemModel>
#include <QSplitter>

MicSpammerWindow::MicSpammerWindow(QWidget *parent)
    : QMainWindow(parent), _window_x(800),_window_y(500), mainVLayout(){

    mainWidget = new QWidget();
    setCentralWidget(mainWidget);

    mainVLayout = new QVBoxLayout();
    setWindowTitle("MicSpammer");
    setGeometry(100,100, _window_x, _window_y);

    //AudioPlayer *audioPlayer = new AudioPlayer(this);

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
    spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // toolbar layout container
    toolbar_rightContainer  = new QWidget(this);
    toolbar_rightHLayout  = new QHBoxLayout();
    toolbar_rightHLayout->addWidget(playButton);
    toolbar_rightHLayout->addSpacing(10); // Spacing between play & stop
    toolbar_rightHLayout->addWidget(stopButton);
    toolbar_rightHLayout->addSpacing(15); // Spacing between stop & volume slider
    toolbar_rightHLayout->addWidget(volumeSlider);
    toolbar_rightHLayout->setContentsMargins(0, 0, 0, 0); // Removes extra margins
    toolbar_rightContainer->setLayout(toolbar_rightHLayout);

    // Add widgets to toolbar
    toolbar->addWidget(openFolderButton);
    toolbar->addWidget(spacer);
    toolbar->addWidget(toolbar_rightContainer);
    addToolBar(Qt::TopToolBarArea, toolbar);
    // add toolbar to the window
    mainVLayout->addWidget(toolbar);

    //QDir(QDir::homePath()).filePath("Music")

    //create and fill horizontal main_content widget below toolbar
    mainContent_container  = new QWidget(this);
    mainContent_HLayout  = new QHBoxLayout();
    //create left hand browser
    browser = new FileBrowserWidget(this);
    // create right hand numpad
    numpad = new NumpadWidget(this);

    mainContent_splitter = new QSplitter(Qt::Horizontal, this);
    mainContent_splitter->show();
    mainContent_splitter->setHandleWidth(2);
    mainContent_splitter->setObjectName("mainContent_splitter");
    mainContent_splitter->setStyleSheet(
        "QSplitter#mainContent_splitter::handle { background-color: lightgray; } "
    );
    mainContent_splitter->setSizes({400, 400});
    mainContent_splitter->addWidget(browser);
    mainContent_splitter->addWidget(numpad);
    mainContent_HLayout->addWidget(mainContent_splitter);

    mainContent_container->setLayout(mainContent_HLayout);
    mainVLayout->addWidget(mainContent_container);
    centralWidget()->setLayout(mainVLayout);

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