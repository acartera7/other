
#pragma once
#ifndef FILEBROWSERWIDGET_H
#define FILEBROWSERWIDGET_H

#include <QDir>
#include <QDebug>
#include <QHeaderView>
#include <QListView>
#include <QFileSystemModel>
#include <QSplitter>
#include <QTreeView>
#include <QVBoxLayout>
#include <QJsonObject>


class FileBrowserWidget : public QWidget {
    Q_OBJECT
public:
    explicit FileBrowserWidget(QWidget *parent = nullptr);
    void setRootDirectory(const QString &path);

    // session persistence
    QJsonObject saveState();
    void loadState(QJsonObject);

signals:
    void fileSelected(const QString &filePath);
    void playSound();

protected:
    //void focusInEvent(QFocusEvent *event) override;
    //void focusOutEvent(QFocusEvent *event) override;

private:
    void listFolder(const QModelIndex &index);

    QFileSystemModel *treeModel, *dirModel;
    QTreeView *treeView;
    QListView *listView;
    QSplitter *splitter;
    QVBoxLayout *layout;
};
#endif //FILEBROWSERWIDGET_H
