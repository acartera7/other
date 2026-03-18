
#include "MicSpammerWindow.h"

MicSpammerWindow::MicSpammerWindow(QWidget *parent) :
        QMainWindow(parent),  _window_x(800),_window_y(500),
        audioPlayer(AudioPlayer::getInstance()),
        micCapture(MicCapture::getInstance()),
        deviceList(WasapiManager::getInstance().getDevices()) {


    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    mainVLayout = new QVBoxLayout(this);
    setWindowTitle("MicSpammer");
    setGeometry(100,100, _window_x, _window_y);

    // profile toolbar for loading profiles
    profile_toolbar = new QToolBar(this);

    profileLabel = new QLabel("Profile: None", this);
    profileLabel->setFixedWidth(100);
    loadProfileButton = new QPushButton("Load", this);
    saveProfileButton = new QPushButton("Save", this);
    deleteProfileButton = new QPushButton("Delete", this);
    resetButton = new QPushButton("Reset", this);

    // Spacer Widget (Flexible Space)
    profileSpacer = new QWidget(this);
    profileSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    profile_toolbar->addWidget(profileLabel);
    profile_toolbar->addWidget(profileSpacer);
    profile_toolbar->addWidget(loadProfileButton);
    profile_toolbar->addWidget(saveProfileButton);
    profile_toolbar->addWidget(resetButton);
    profile_toolbar->addWidget(deleteProfileButton);

    //addToolBar(Qt::TopToolBarArea, profile_toolbar);
    // Main Toolbar & Button
    toolbar = new QToolBar(this);
    playButton = new QPushButton("Play", this);
    stopButton = new QPushButton("Stop", this);

    openFolderButton = new QPushButton("Open Folder", this);

    // Spacer Widget (Flexible Space)
    toolbarSpacer = new QWidget(this);
    toolbarSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Devices selection dropdown
    micComboBox = new QComboBox(this);
    monitorComboBox = new QComboBox(this);
    sendComboBox = new QComboBox(this);

    micComboBox->addItem("--None--", QVariant("None"));
    micComboBox->setCurrentIndex(0);
    monitorComboBox->addItem("--None--", QVariant("None"));
    monitorComboBox->setCurrentIndex(0);
    sendComboBox->addItem("--None--", QVariant("None"));
    sendComboBox->setCurrentIndex(0);

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

    micVolumeSlider = new QSlider(Qt::Horizontal, this);
    monitorVolumeSlider = new QSlider(Qt::Horizontal, this);
    sendVolumeSlider = new QSlider(Qt::Horizontal, this);
    sendVolumeSlider->setRange(0, 100);
    sendVolumeSlider->setFixedWidth(100);
    sendVolumeSlider->setObjectName("SendVS");
    monitorVolumeSlider->setRange(0, 100);
    monitorVolumeSlider->setFixedWidth(100);
    monitorVolumeSlider->setObjectName("MonitorVS");
    micVolumeSlider->setRange(0, 100);
    micVolumeSlider->setFixedWidth(100);
    micVolumeSlider->setObjectName("MicVS");

    toolbar_devicesGridLayout = new QGridLayout(this);

    micDeviceLabel = new QLabel("Microphone Device:",this);
    monitorDeviceLabel = new QLabel("Monitoring Device:",this);
    sendDeviceLabel = new QLabel("Output Device",this);

    toolbar_devicesGridLayout->addWidget(micDeviceLabel, 0,0);
    toolbar_devicesGridLayout->addWidget(micComboBox, 0,1);
    toolbar_devicesGridLayout->addWidget(micVolumeSlider, 0,2);

    toolbar_devicesGridLayout->addWidget(monitorDeviceLabel, 1,0);
    toolbar_devicesGridLayout->addWidget(monitorComboBox, 1,1);
    toolbar_devicesGridLayout->addWidget(monitorVolumeSlider, 1,2);

    toolbar_devicesGridLayout->addWidget(sendDeviceLabel, 2,0);
    toolbar_devicesGridLayout->addWidget(sendComboBox, 2,1);
    toolbar_devicesGridLayout->addWidget(sendVolumeSlider, 2,2);

    //devices left part devices drop down container
    toolbar_devicesContainer = new QWidget(this);
    toolbar_devicesContainer->setLayout(toolbar_devicesGridLayout);

    // toolbar right part layout container
    toolbar_rightContainer  = new QWidget(this);
    toolbar_rightHLayout  = new QHBoxLayout(this);
    toolbar_rightHLayout->addWidget(playButton);
    toolbar_rightHLayout->addSpacing(10); // Spacing between play & stop
    toolbar_rightHLayout->addWidget(stopButton);
    toolbar_rightHLayout->setContentsMargins(0, 0, 0, 0); // Removes extra margins
    toolbar_rightContainer->setLayout(toolbar_rightHLayout);

    // Add widgets to toolbar
    toolbar->addWidget(openFolderButton);
    toolbar->addWidget(toolbar_devicesContainer);
    toolbar->addWidget(toolbarSpacer);
    toolbar->addWidget(toolbar_rightContainer);
    //addToolBar(Qt::TopToolBarArea, toolbar);
    // add toolbar to the window
    mainVLayout->addWidget(profile_toolbar);
    mainVLayout->addWidget(toolbar);

    //QDir(QDir::homePath()).filePath("Music")

    //create and fill horizontal main_content widget below toolbar
    mainContent_container  = new QWidget(this);
    mainContent_HLayout  = new QHBoxLayout(this);
    //create left hand browser
    browser = new FileBrowserWidget(this);

    browser->setRootDirectory(QDir::homePath());

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
    connect(loadProfileButton, &QPushButton::clicked, this, &MicSpammerWindow::onLoadProfile);
    connect(saveProfileButton, &QPushButton::clicked, this, &MicSpammerWindow::onSaveProfile);
    connect(deleteProfileButton, &QPushButton::clicked, this, &MicSpammerWindow::onDeleteProfile);
    connect(resetButton, &QPushButton::clicked, this, &MicSpammerWindow::onReset);

    connect(openFolderButton, &QPushButton::clicked, this, &MicSpammerWindow::onOpenFolder);
    connect(playButton, &QPushButton::clicked, this, &MicSpammerWindow::onPlay);
    connect(stopButton, &QPushButton::clicked, this, &MicSpammerWindow::onStop);
    connect(micVolumeSlider, &QSlider::valueChanged, this,
        [this](int volume) {
            onVolumeChanged(micVolumeSlider->objectName(), volume);
        });
    connect(monitorVolumeSlider, &QSlider::valueChanged, this,
        [this](int volume) {
            onVolumeChanged(monitorVolumeSlider->objectName(), volume);
        });
    connect(sendVolumeSlider, &QSlider::valueChanged, this,
        [this](int volume) {
            onVolumeChanged(sendVolumeSlider->objectName(), volume);
        });
    // File actions
    connect(browser, &FileBrowserWidget::fileSelected, this, &MicSpammerWindow::onFileSelected);
    connect(browser, &FileBrowserWidget::playSound, this, &MicSpammerWindow::onPlay);
    connect(numpad, &NumpadWidget::numpadTriggered,this,
        [this](int key, const QString &filePath) {
            audioPlayer.play(filePath);
        });

    connect(numpad, &NumpadWidget::pageChanged, this, [](int page) {
        qDebug() << "Switched to page:" << page;
    });

    connect(micComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MicSpammerWindow::onMicDeviceChanged);
    connect(monitorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MicSpammerWindow::onMonitorDeviceChanged);
    connect(sendComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MicSpammerWindow::onSendDeviceChanged);

    // TODO check for last profile

    monitorVolumeSlider->setValue(80);
    sendVolumeSlider->setValue(80);
    micVolumeSlider->setValue(80);

}

template<typename>
constexpr auto MicSpammerWindow::qt_create_metaobjectdata() {
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

void MicSpammerWindow::onVolumeChanged(QString name, int volume) {

    float gain = 0.0f;
    // Clamp to avoid log(0)
    if (volume > 0) {
        // Map 0–100 slider to -40 dB .. 0 dB range
        float minDb = -40.0f;   // silence threshold
        float maxDb = 0.0f;

        float db = minDb + (volume / 100.0f) * (maxDb - minDb);

        // Convert dB to linear gain
        gain = powf(10.0f, db / 20.0f);
    }

    if ( name == "MonitorVS") {
        audioPlayer.setMonitorVolume(gain);
    } else if (name == "SendVS") {
        audioPlayer.setOutputVolume(gain);
    } else if (name == "MicVS") {
        micCapture.setVolume(gain);
    }
}

void MicSpammerWindow::onFileSelected(const QString &filePath) {
    selectedFilePath = filePath;
}

void MicSpammerWindow::onLoadProfile() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Load Profile",
        QDir("saves").absolutePath(),
        "Profile Files (*.json)"
    );

    if (!fileName.isEmpty()) {

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) return;

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isNull()) return;

        // load states for browser and numpad widgets
        QJsonObject root = doc.object();
        browser->loadState(root["browser"].toObject());
        numpad->loadState(root["numpad"].toObject());

        // load state for the window size
        QJsonObject winObj = root["window"].toObject();
        QSize winSize(winObj["w"].toInt(),
                   winObj["h"].toInt());
        resize(winSize.width(), winSize.height());

        // load states for the devices and volumes
        QJsonObject devicesObj = root["devices"].toObject();

        // mic device
        QString micDeviceName = devicesObj["micdevice-id"].toString();
        if (isDeviceValid(micDeviceName)) {
            micCapture.setInputDevice(micDeviceName);
            micComboBox->setCurrentIndex(micComboBox->findData(micDeviceName));
        } else {
            QString deviceText = devicesObj["micdevice-text"].toString();
            QMessageBox::information(this,"Error","Error: failed to load Microphone Device \"" + deviceText +"\". Device not found or is disabled.");
        }

        //monitor device
        QString monitorDeviceName = devicesObj["monitoringdevice-id"].toString();
        if (isDeviceValid(monitorDeviceName)) {
            audioPlayer.setMonitorDevice(monitorDeviceName);
            monitorComboBox->setCurrentIndex(monitorComboBox->findData(monitorDeviceName));
        } else {
            QString deviceText = devicesObj["monitoringdevice-text"].toString();
            QMessageBox::information(this,"Error","Error: failed to load Monitoring Device \"" + deviceText +"\". Device not found or is disabled.");
        }


        //output device
        QString outputDeviceName = devicesObj["outputdevice-id"].toString();
        if (isDeviceValid(outputDeviceName)) {
            micCapture.setOutputDevice(devicesObj["outputdevice-id"].toString());
            audioPlayer.setOutputDevice(devicesObj["outputdevice-id"].toString());
            sendComboBox->setCurrentIndex(sendComboBox->findData(outputDeviceName));
        } else {
            QString deviceText = devicesObj["outputdevice-text"].toString();
            QMessageBox::information(this,"Error","Error: failed to load Output Device \"" + deviceText +"\". Device not found or is disabled.");
        }

        QJsonObject volumeObj = root["volume"].toObject();
        micVolumeSlider->setValue(volumeObj["mic-volume"].toInt());
        monitorVolumeSlider->setValue(volumeObj["monitor-volume"].toInt());
        sendVolumeSlider->setValue(volumeObj["output-volume"].toInt());

        profileLabel->setText("Profile: " + QFileInfo(fileName).baseName());
        currentProfilePath = fileName;

    }
}

void MicSpammerWindow::onSaveProfile() {
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save Profile",
        QDir("saves").absolutePath(),
        "Profile Files (*.json)"
    );
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.exists()) {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this,
                "Overwrite Profile",
                "The file already exists. Do you want to overwrite it?",
                QMessageBox::Yes | QMessageBox::No
            );

            if (reply == QMessageBox::No) {
                return; // user cancelled overwrite
            }
        }

        // save states for browser and numpad widgets
        QJsonObject root;
        root["browser"] = browser->saveState();
        root["numpad"] = numpad->saveState();

        // save state for the window size
        root["window"] = QJsonObject {
            {"w", window()->size().width()},
            {"h", window()->size().height()}
        };
        // save states for the devices and volumes
        root["devices"] = QJsonObject {
            {"micdevice-id", micComboBox->itemData(micComboBox->currentIndex()).toString()},
            {"micdevice-text", micComboBox->itemText(micComboBox->currentIndex())},
            {"monitoringdevice-id", monitorComboBox->itemData(monitorComboBox->currentIndex()).toString()},
            {"monitoringdevice-text", monitorComboBox->itemText(monitorComboBox->currentIndex())},
            {"outputdevice-id", sendComboBox->itemData(sendComboBox->currentIndex()).toString()},
            {"outputdevice-text", sendComboBox->itemText(sendComboBox->currentIndex())},
        };
        root["volume"] = QJsonObject {
            {"mic-volume", micVolumeSlider->value()},
            {"monitor-volume", monitorVolumeSlider->value()},
            {"output-volume", sendVolumeSlider->value()}
        };

        if (QDir().mkpath("saves")) {
            if (file.open(QIODevice::WriteOnly)) {
                file.write(QJsonDocument(root).toJson());
            }
        } else {
            qDebug() << "Error Save Profile: failed to make /saves folder" ;
        }
        profileLabel->setText("Profile: " + QFileInfo(fileName).baseName());
        currentProfilePath = fileName;
    }
}

void MicSpammerWindow::onDeleteProfile() {
    if (currentProfilePath.isEmpty()) {
        QMessageBox::information(this,"Delete Profile","No profile currently loaded");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Delete Profile",
        "Are you sure you want to delete " + QFileInfo(currentProfilePath).fileName() + "?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        QFile::remove(currentProfilePath);
        currentProfilePath.clear();
        profileLabel->setText("Profile: None");

        reply = QMessageBox::question(
            this,
            "Reset Settings",
            "Would you like to reset current settings to default?",
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::Yes) {
            resetProfileSettings();
        }
    }
}

void MicSpammerWindow::onReset() {

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Reset Profile",
        "Resetting will clear all settings. Do you want to save the current profile first?",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
    );

    if (reply == QMessageBox::Cancel) return;

    if (reply == QMessageBox::Yes) {
        onSaveProfile();
    }

    resetProfileSettings();
    currentProfilePath.clear();
    profileLabel->setText("Profile: None");
}

void MicSpammerWindow::resetProfileSettings() {

    // Reset devices
    micComboBox->setCurrentIndex(0);
    monitorComboBox->setCurrentIndex(0);
    sendComboBox->setCurrentIndex(0);

    micCapture.setInputDevice("None");
    micCapture.setOutputDevice("None");
    audioPlayer.setMonitorDevice("None");
    audioPlayer.setOutputDevice("None");

    // Reset volume sliders
    micVolumeSlider->setValue(80);
    monitorVolumeSlider->setValue(80);
    sendVolumeSlider->setValue(80);

    // Reset file browser
    browser->setRootDirectory(QDir::homePath());
    selectedFilePath.clear();

    // Reset numpad mappings and page
    numpad->resetMappings();
    setGeometry(100, 100, _window_x, _window_y);

    // Clear profile label
    profileLabel->setText("Profile: None");

    qDebug() << "Profile settings reset to defaults.";
}

void MicSpammerWindow::onMicDeviceChanged(int index) {
    QString id = micComboBox->itemData(index).toString();
    micCapture.setInputDevice(id);
}

void MicSpammerWindow::onMonitorDeviceChanged(int index) {
    QString id = monitorComboBox->itemData(index).toString();
    audioPlayer.setMonitorDevice(id);
}

void MicSpammerWindow::onSendDeviceChanged(int index) {
    QString id = sendComboBox->itemData(index).toString();
    micCapture.setOutputDevice(id);
    audioPlayer.setOutputDevice(id);
}

bool MicSpammerWindow::isDeviceValid(QString deviceName) {
    if (deviceName.isEmpty())
        return false;
    std::vector<AudioDeviceInfo>::const_iterator it = std::find_if(deviceList.begin(), deviceList.end(),
            [=](const AudioDeviceInfo& device) {
                std::wstring wname = deviceName.toStdWString();
                if (device.id == wname)
                    return true;
                return false;
            });
    if (it != deviceList.end())
        return true;
    return false;
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

//void MicSpammerWindow::focusInEvent(QFocusEvent *event) {
//    qDebug() << "MicSpammerWindow got focus";
//    QWidget::focusInEvent(event);
//}
//
//void MicSpammerWindow::focusOutEvent(QFocusEvent *event) {
//    qDebug() << "MicSpammerWindow lost focus";
//    QWidget::focusOutEvent(event);
//}