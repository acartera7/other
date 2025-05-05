//
// Created by Andrei on 5/4/2025.
//
#pragma once
#ifndef FILEBROWSERWIDGET_H
#define FILEBROWSERWIDGET_H


#include <QListView>
#include <QFileSystemModel>
#include <QTreeView>

class FileBrowserWidget : public QWidget {
    Q_OBJECT
public:
    explicit FileBrowserWidget(QWidget *parent = nullptr);
    void setRootDirectory(const QString &path);

    signals:
        void fileSelected(const QString &filePath);
        void playSound();

private:
    QFileSystemModel *treeModel, *dirModel;
    QTreeView *treeView;
    QListView *listView;
};
#endif //FILEBROWSERWIDGET_H
