//
// Created by Victus on 11/15/2025.
//

#ifndef MICSPAMMER_NUMPADWIDGET_H
#define MICSPAMMER_NUMPADWIDGET_H
#pragma once
#include <QWidget>
#include <QFileInfo>
#include <QMimeData>
#include <QKeyEvent>
#include <QMenu>
#include <QPushButton>
#include <QInputDialog>
#include <QGridLayout>
#include <QMap>

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

    signals:
        void numpadTriggered(int numpadKey, const QString &filePath);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QGridLayout *gridLayout;
    QMap<int, QPushButton*> buttons;
    QMap<int, QString> mappings;
    // TODO add session persistence
    QMap<int, NumpadItem> items;

    void setupButtons();

private slots:
    void showContextMenu(QPoint pos, int key);
    void renameButton(int key);
};

#endif //MICSPAMMER_NUMPADWIDGET_H