

#include "NumpadWidget.h"

#include <map>

NumpadWidget::NumpadWidget(QWidget *parent) : QWidget(parent) {

    mainLayout = new QVBoxLayout(this);

    // Page indicator bar
    indicatorLayout = new QHBoxLayout(this);
    prevPageButton = new QPushButton(this);
    nextPageButton = new QPushButton(this);
    QPixmap leftArrow(":/icons/assets/long_arrow_left_icon.png");
    QPixmap rightArrow(":/icons/assets/long_arrow_right_icon.png");
    qDebug() << "Pixmap valid?" << !leftArrow.isNull()
         << "Size:" << leftArrow.size();

    // Get the current text option

    prevPageButton->setIcon(QIcon(leftArrow));
    nextPageButton->setIcon(QIcon(rightArrow));

    // Control icon size separately
    //prevPageButton->setIconSize(QSize(24, 24));
    //nextPageButton->setIconSize(QSize(24, 24));
    prevPageButton->setIconSize(leftArrow.rect().size()/2);
    nextPageButton->setIconSize(rightArrow.rect().size()/2);

    // Optional: make buttons flat so they don’t look bulky
    //prevPageButton->setFlat(true);
    //nextPageButton->setFlat(true);

    //prevPageButton->setObjectName("PrevPageButton");
    //nextPageButton->setObjectName("PextPageButton");
    pageIndicator = new QLabel(this);

    pageIndicator->setAlignment(Qt::AlignCenter);
    pageIndicator->setStyleSheet("background-color: #333; color: white; padding: 4px;");
    pageIndicator->setTextFormat(Qt::PlainText);

    indicatorLayout->addWidget(prevPageButton);
    indicatorLayout->addWidget(pageIndicator, 1); // stretch
    indicatorLayout->addWidget(nextPageButton);

    gridLayout = new QGridLayout();
    setupButtons();

    mainLayout->addLayout(indicatorLayout);
    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);

    setAcceptDrops(true);
    updatePageIndicator();

    // Connect navigation buttons
    connect(prevPageButton, &QPushButton::clicked, this, &NumpadWidget::prevPage);
    connect(nextPageButton, &QPushButton::clicked, this, &NumpadWidget::nextPage);
}


void NumpadWidget::setupButtons() {
    int key = 1;
    for (int row = 3; row > 0; --row) {
        for (int col = 0; col < 3; ++col) {
            QPushButton *btn = new QPushButton(QString::number(key), this);
            btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            btn->setAcceptDrops(false);
            gridLayout->addWidget(btn, row, col);
            buttons[key] = btn;

            connect(btn, &QPushButton::clicked, this, [this, key]() {
                if (pageMappings[currentPage].contains(key)) {
                    emit numpadTriggered(key, pageMappings[currentPage][key].filePath);
                }
            });
            btn->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(btn, &QPushButton::customContextMenuRequested, this,
                    [this, key](const QPoint &pos) {
                        showContextMenu(pos, key);
                    });

            ++key;
        }
    }
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);


}

void NumpadWidget::updatePageIndicator() const {
    pageIndicator->setText(
        QString("Page %1 / %2   (  Use   + / -   or   🠄 🠆  )")
        .arg(currentPage).arg(maxPages)
    );

    // Disable arrows at limits
    prevPageButton->setEnabled(currentPage > 1);
    nextPageButton->setEnabled(currentPage < maxPages);
}

void NumpadWidget::setMapping(int numpadKey, const QString &filePath) {
    NumpadItem item;
    item.filePath = filePath;
    item.customLabel = QFileInfo(filePath).fileName(); // default label
    pageMappings[currentPage][numpadKey] = item;

    if (buttons.contains(numpadKey)) {
        buttons[numpadKey]->setText(QString::number(numpadKey) + "\n" + item.customLabel);
    }
}



QString NumpadWidget::getMapping(int numpadKey) const {
    if (pageMappings[currentPage].contains(numpadKey)) {
        return pageMappings[currentPage][numpadKey].filePath;
    }
    return QString();
}

// NumpadWidget.cpp
void NumpadWidget::triggerKey(int key) {
    animateButtonPress(key);  // visual feedback
    if (pageMappings[currentPage].contains(key)) {
        emit numpadTriggered(key, pageMappings[currentPage][key].filePath);
    }
}

void NumpadWidget::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();   // Always accept URLs
    } else {
        event->ignore();
    }
}

void NumpadWidget::dropEvent(QDropEvent *event) {
    if (!event->mimeData()->hasUrls()) {
        event->ignore();
        return;
    }

    QString filePath = event->mimeData()->urls().first().toLocalFile();

    // Translate position into the grid layout area
    QPoint pos = event->position().toPoint();
    QWidget *child = childAt(pos);

    if (auto *btn = qobject_cast<QPushButton*>(child)) {
        for (auto it = buttons.begin(); it != buttons.end(); ++it) {
            if (it.value() == btn) {
                setMapping(it.key(), filePath);
                event->acceptProposedAction();
                return;
            }
        }
    }

    // If not dropped on a button, ignore
    event->ignore();
}

void NumpadWidget::showContextMenu(QPoint pos, int key) {
    QMenu menu;
    QAction *renameAction = menu.addAction("Rename");
    QAction *removeAction = menu.addAction("Remove");

    QAction *selected = menu.exec(buttons[key]->mapToGlobal(pos));
    if (selected == renameAction) {
        renameButton(key);
    } else if (selected == removeAction) {
        removeButtonMapping(key);
    }
}


void NumpadWidget::renameButton(int key) {
    bool ok;
    QString text = QInputDialog::getText(this,
        tr("Rename Button"),
        tr("Enter new label:"),
        QLineEdit::Normal,
        pageMappings[currentPage][key].customLabel,
        &ok);

    if (ok && !text.isEmpty()) {
        pageMappings[currentPage][key].customLabel = text;
        buttons[key]->setText(QString::number(key) + "\n" + text);
    }
}

void NumpadWidget::removeButtonMapping(int key) {
    if (pageMappings[currentPage].contains(key)) {
        pageMappings[currentPage].remove(key);
        buttons[key]->setText(QString::number(key)); // reset to default label
    }
}


void NumpadWidget::loadPage(int page) {
    // Clear button labels
    for (auto it = buttons.begin(); it != buttons.end(); ++it) {
        int key = it.key();
        QPushButton *btn = it.value();
        if (pageMappings[page].contains(key)) {
            const NumpadItem &item = pageMappings[page][key];
            QString label = item.customLabel.isEmpty()
                ? QFileInfo(item.filePath).fileName()
                : item.customLabel;
            btn->setText(QString::number(key) + "\n" + label);
        } else {
            btn->setText(QString::number(key));
        }
    }
    updatePageIndicator();
}

// NumpadWidget.cpp
void NumpadWidget::nextPage() {
    if (currentPage < maxPages) {
        ++currentPage;
        loadPage(currentPage);
        emit pageChanged(currentPage);
    }
}

void NumpadWidget::prevPage() {
    if (currentPage > 1) {
        --currentPage;
        loadPage(currentPage);
        emit pageChanged(currentPage);
    }
}

void NumpadWidget::animateButtonPress(int key) {
    if (buttons.contains(key)) {
        QPushButton *btn = buttons[key];
        btn->setDown(true);  // show pressed state
        QTimer::singleShot(150, btn, [btn]() {
            btn->setDown(false);  // release after 150ms
        });
    }
}

void NumpadWidget::resetMappings() {

    AudioPlayer::getInstance().stopAll();

    for (auto &map : pageMappings) {
        map.clear();
    }

    for (int i = 1; i <= buttons.size(); ++i) {
        if (buttons[i]) {
            buttons[i]->setText(QString::number(i + 1));
        }
    }

    currentPage = 1;
    loadPage(currentPage);
    emit pageChanged(currentPage);

}

QJsonObject NumpadWidget::saveState() {
    QJsonObject state;
    state["currentPage"] = currentPage;

    QJsonArray pagesArray;
    for (const auto &pageMap : pageMappings) {
        QJsonObject pageObj;
        for (auto it = pageMap.constBegin(); it != pageMap.constEnd(); ++it) {
            pageObj[QString::number(it.key())] = it.value().toJson();
        }
        pagesArray.append(pageObj);
    }
    state["pages"] = pagesArray;

    return state;
}

void NumpadWidget::loadState(QJsonObject state) {
    if (state.contains("currentPage")) {
        currentPage = state["currentPage"].toInt();
    }

    if (state.contains("pages")) {
        QJsonArray pagesArray = state["pages"].toArray();
        pageMappings.clear();

        for (int i = 0; i < pagesArray.size(); ++i) {
            QJsonObject pageObj = pagesArray[i].toObject();
            QMap<int, NumpadItem> btnMap;

            for (auto it = pageObj.begin(); it != pageObj.end(); ++it) {
                NumpadItem item;
                int btnKey = it.key().toInt();
                item.fromJson(it.value().toObject());
                btnMap[btnKey] = item;
                if (buttons.contains(btnKey)) {
                    buttons[btnKey]->setText(QString::number(btnKey) + "\n" + item.customLabel);
                }

            }
            pageMappings[i+1] = btnMap;

        }
    }

    loadPage(currentPage);
}
