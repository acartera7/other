//
// Created by Andrei on 4/29/2025.
//
#include "MicSpammerWindow.h"



MicSpammerWindow::MicSpammerWindow(QWidget *parent)
    : QMainWindow(parent),  _window_x(800),_window_y(500), audioPlayer(AudioPlayer::getInstance()), mainVLayout() {


    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    mainVLayout = new QVBoxLayout(this);
    setWindowTitle("MicSpammer");
    setGeometry(100,100, _window_x, _window_y);

    // Toolbar & Button
    toolbar = new QToolBar(this);
    playButton = new QPushButton("Play", this);
    stopButton = new QPushButton("Stop", this);
    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100); // Volume range 0-100
    volumeSlider->setFixedWidth(100);
    openFolderButton = new QPushButton("Open Folder", this);

    // Spacer Widget (Flexible Space)
    spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Devices selection dropdown
    micComboBox = new QComboBox(this);
    monitorComboBox = new QComboBox(this);
    sendComboBox = new QComboBox(this);

    micComboBox->addItem("--None--", QVariant("None"));
    micComboBox->setCurrentIndex(-1);
    monitorComboBox->addItem("--None--", QVariant("None"));
    monitorComboBox->setCurrentIndex(-1);
    sendComboBox->addItem("--None--", QVariant("None"));
    sendComboBox->setCurrentIndex(-1);

    deviceList = WasapiManager::getInstance().getDevices();
    for (size_t i = 0; i < deviceList.size(); i++) {
        const auto& info = deviceList.at(i);
        QString name = QString::fromStdWString(info.name);
        if (info.flow == eCapture) {
            micComboBox->addItem(name, QVariant(QString::fromStdWString(info.id)));
        } else if (info.flow == eRender) {
            monitorComboBox->addItem(name, QVariant(QString::fromStdWString(info.id)));
            sendComboBox->addItem(name, QVariant(QString::fromStdWString(info.id)));
        }
    }

    toolbar_devicesFLayout = new QFormLayout(this);

    micDeviceLabel = new QLabel("Microphone Device:",this);
    monitorDeviceLabel = new QLabel("Monitoring Device:",this);
    sendDeviceLabel = new QLabel("Output Device",this);

    toolbar_devicesFLayout->addRow(micDeviceLabel, micComboBox);
    toolbar_devicesFLayout->addRow(monitorDeviceLabel, monitorComboBox);
    toolbar_devicesFLayout->addRow(sendDeviceLabel, sendComboBox);

    //devices left part devices drop down container
    toolbar_devicesContainer = new QWidget(this);
    toolbar_devicesContainer->setLayout(toolbar_devicesFLayout);

    // toolbar right part layout container
    toolbar_rightContainer  = new QWidget(this);
    toolbar_rightHLayout  = new QHBoxLayout(this);
    toolbar_rightHLayout->addWidget(playButton);
    toolbar_rightHLayout->addSpacing(10); // Spacing between play & stop
    toolbar_rightHLayout->addWidget(stopButton);
    toolbar_rightHLayout->addSpacing(15); // Spacing between stop & volume slider
    toolbar_rightHLayout->addWidget(volumeSlider);
    toolbar_rightHLayout->setContentsMargins(0, 0, 0, 0); // Removes extra margins
    toolbar_rightContainer->setLayout(toolbar_rightHLayout);

    // Add widgets to toolbar
    toolbar->addWidget(openFolderButton);
    toolbar->addWidget(toolbar_devicesContainer);
    toolbar->addWidget(spacer);
    toolbar->addWidget(toolbar_rightContainer);
    addToolBar(Qt::TopToolBarArea, toolbar);
    // add toolbar to the window
    mainVLayout->addWidget(toolbar);

    //QDir(QDir::homePath()).filePath("Music")

    //create and fill horizontal main_content widget below toolbar
    mainContent_container  = new QWidget(this);
    mainContent_HLayout  = new QHBoxLayout(this);
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
    connect(numpad, &NumpadWidget::numpadTriggered,
        this, [this](int key, const QString &filePath) {
            audioPlayer.play(filePath);
        });

    connect(numpad, &NumpadWidget::pageChanged, this, [](int page) {
        qDebug() << "Switched to page:" << page;
    });

    volumeSlider->setValue(80);
}

void MicSpammerWindow::onOpenFolder() {
    QString folder = QFileDialog::getExistingDirectory(this, "Select a folder", QDir::homePath());
    if (!folder.isEmpty()) {
        browser->setRootDirectory(folder);  // Update browser widget
    }
}

void MicSpammerWindow::onPlay() {
    if (!selectedFilePath.isEmpty()) {
        audioPlayer.play(selectedFilePath);  // Play last selected file
    }
}

void MicSpammerWindow::onStop() {
    audioPlayer.stopAll();
}

void MicSpammerWindow::onVolumeChanged(int volume) {
    // Clamp to avoid log(0)
    if (volume <= 0) {
        audioPlayer.setVolume(0.0f);
        return;
    }

    // Map 0–100 slider to -40 dB .. 0 dB range
    float minDb = -40.0f;   // silence threshold
    float maxDb = 0.0f;

    float db = minDb + (volume / 100.0f) * (maxDb - minDb);

    // Convert dB to linear gain
    float gain = powf(10.0f, db / 20.0f);

    audioPlayer.setVolume(gain);
}

void MicSpammerWindow::onFileSelected(const QString &filePath) {
    selectedFilePath = filePath;
}

MicSpammerWindow::~MicSpammerWindow() = default;

// MicSpammerWindow.cpp
void MicSpammerWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Plus:  numpad->nextPage();     break;
    case Qt::Key_Minus: numpad->prevPage();     break;
    case Qt::Key_1:     numpad->triggerKey(1);  break;
    case Qt::Key_2:     numpad->triggerKey(2);  break;
    case Qt::Key_3:     numpad->triggerKey(3);  break;
    case Qt::Key_4:     numpad->triggerKey(4);  break;
    case Qt::Key_5:     numpad->triggerKey(5);  break;
    case Qt::Key_6:     numpad->triggerKey(6);  break;
    case Qt::Key_7:     numpad->triggerKey(7);  break;
    case Qt::Key_8:     numpad->triggerKey(8);  break;
    case Qt::Key_9:     numpad->triggerKey(9);  break;

    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}

void MicSpammerWindow::focusInEvent(QFocusEvent *event) {
    qDebug() << "MicSpammerWindow got focus";
    QWidget::focusInEvent(event);
}

void MicSpammerWindow::focusOutEvent(QFocusEvent *event) {
    qDebug() << "MicSpammerWindow lost focus";
    QWidget::focusOutEvent(event);
}