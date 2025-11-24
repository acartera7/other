//
// Created by Victus on 11/15/2025.
//

#include "NumpadWidget.h"

NumpadWidget::NumpadWidget(QWidget *parent) : QWidget(parent) {
    mainLayout = new QVBoxLayout(this);

    // Page indicator bar
    indicatorLayout = new QHBoxLayout();
    prevPageButton = new QPushButton("◀", this);
    nextPageButton = new QPushButton("▶", this);
    pageIndicator = new QLabel(this);

    pageIndicator->setAlignment(Qt::AlignCenter);
    pageIndicator->setStyleSheet("background-color: #333; color: white; padding: 4px;");

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
        QString("Page %1 / %2   (Use + / - or ◀ ▶)")
        .arg(currentPage).arg(maxPages)
    );
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

    QAction *selected = menu.exec(buttons[key]->mapToGlobal(pos));
    if (selected == renameAction) {
        renameButton(key);
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