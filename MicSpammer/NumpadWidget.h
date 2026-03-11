//
// Created by Andrew on 11/15/2025.
//

#ifndef MICSPAMMER_NUMPADWIDGET_H
#define MICSPAMMER_NUMPADWIDGET_H
#pragma once
#include "AudioPlayer.h"

#include <QWidget>
#include <QFileInfo>
#include <QMimeData>
#include <QKeyEvent>
#include <QMenu>
#include <QPushButton>
#include <QInputDialog>
#include <QGridLayout>
#include <QMap>
#include <QLabel>
#include <QTimer>
#include <QPixmap>
#include <QIcon>
#include <QJsonObject>


struct NumpadItem {
    QString filePath;
    QString customLabel;
};

class NumpadWidget : public QWidget {
    Q_OBJECT
public:
    explicit NumpadWidget(QWidget *parent = nullptr);

    void setMapping(int numpadKey, const QString &filePath);
    [[nodiscard]] QString getMapping(int numpadKey) const;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void nextPage();
    void prevPage();
    [[nodiscard]] int currentPageNumber() const { return currentPage; }
    void triggerKey(int key);
    void animateButtonPress(int key);

    // session persistence
    QJsonObject saveState();
    void loadState(QJsonObject);

signals:
    void numpadTriggered(int numpadKey, const QString &filePath);
    void pageChanged(int currentPage);

private:
    int currentPage = 1;
    const int maxPages = 9;

    QVBoxLayout *mainLayout;
    QGridLayout *gridLayout;
    QHBoxLayout *indicatorLayout;
    QLabel *pageIndicator;
    QPushButton *prevPageButton;
    QPushButton *nextPageButton;

    // Store mappings per page
    // page -> (key -> item)
    QMap<int, QMap<int, NumpadItem>> pageMappings;

    QMap<int, QPushButton*> buttons;

    void setupButtons();
    void updatePageIndicator() const;
    void loadPage(int page);

private slots:
    void showContextMenu(QPoint pos, int key);
    void renameButton(int key);
    void removeButtonMapping(int key);
};

#endif //MICSPAMMER_NUMPADWIDGET_H