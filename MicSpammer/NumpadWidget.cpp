//
// Created by Victus on 11/15/2025.
//

#include "NumpadWidget.h"

NumpadWidget::NumpadWidget(QWidget *parent) : QWidget(parent) {
    gridLayout = new QGridLayout(this);
    setAcceptDrops(true);
    setupButtons();
    setLayout(gridLayout);
}

void NumpadWidget::setupButtons() {
    int key = 1;
    for (int row = 3; row > 0; --row) {
        for (int col = 0; col < 3; ++col) {
            QPushButton *btn = new QPushButton(QString::number(key), this);
            btn->setFixedSize(50,50);
            btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            gridLayout->addWidget(btn, row, col);
            buttons[key] = btn;

            connect(btn, &QPushButton::clicked, this, [this, key]() {
                if (mappings.contains(key)) {
                    emit numpadTriggered(key, mappings[key]);
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
    //gridLayout->setSpacing(0);
    //gridLayout->setContentsMargins(0, 0, 0, 0);
    //for (int i = 0; i < 3; ++i) {
    //    gridLayout->setColumnStretch(i, 1);
    //    gridLayout->setRowStretch(i, 1);
    //}
}

void NumpadWidget::setMapping(int numpadKey, const QString &filePath) {
    mappings[numpadKey] = filePath;
    if (buttons.contains(numpadKey)) {
        buttons[numpadKey]->setText(QString::number(numpadKey) + "\n" + QFileInfo(filePath).fileName());
    }
}

QString NumpadWidget::getMapping(int numpadKey) const {
    return mappings.value(numpadKey, QString());
}

void NumpadWidget::keyPressEvent(QKeyEvent *event) {
    int key = -1;
    switch (event->key()) {
        case Qt::Key_1: key = 1; break;
        case Qt::Key_2: key = 2; break;
        case Qt::Key_3: key = 3; break;
        case Qt::Key_4: key = 4; break;
        case Qt::Key_5: key = 5; break;
        case Qt::Key_6: key = 6; break;
        case Qt::Key_7: key = 7; break;
        case Qt::Key_8: key = 8; break;
        case Qt::Key_9: key = 9; break;
        default:
        break;
    }
    if (key != -1 && mappings.contains(key)) {
        emit numpadTriggered(key, mappings[key]);
    }

}

void NumpadWidget::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void NumpadWidget::dropEvent(QDropEvent *event) {
    if (event->mimeData()->hasUrls()) {
        QString filePath = event->mimeData()->urls().first().toLocalFile();
        QWidget *child = childAt(event->position().toPoint());
        for (auto it = buttons.begin(); it != buttons.end(); ++it) {
            if (it.value() == child) {
                setMapping(it.key(), filePath);
                break;
            }
        }
    }
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
        buttons[key]->text(),
        &ok);

    if (ok && !text.isEmpty()) {
        buttons[key]->setText(text);
        // Optionally store separately from file mapping
        mappings[key] = mappings[key]; // keep file path
        // You might want a separate map for custom labels later
    }
}