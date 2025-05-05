//
// Created by Andrei on 5/4/2025.
//

#include "FileBrowserWidget.h"
#include <QVBoxLayout>
#include <QDir>
#include <QDebug>
#include <QHeaderView>
#include <QSplitter>

FileBrowserWidget::FileBrowserWidget(QWidget *parent)
    : QWidget(parent)
{
    // Model shared between both views
    treeModel = new QFileSystemModel(this);
    treeModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    treeModel->setRootPath(QDir::rootPath());

    // --- Tree View (Left)
    treeView = new QTreeView(this);
    treeView->setModel(treeModel);

    treeView->setRootIndex(QModelIndex());
    treeView->setHeaderHidden(false); // ✅ Show headers
    treeView->header()->setSectionResizeMode(QHeaderView::Interactive); // ✅ Make resizable
    treeView->header()->setStretchLastSection(true); // last column expands
    treeView->setColumnHidden(1, true); // hide Size
    treeView->setColumnHidden(2, true); // hide Type
    treeView->setColumnHidden(3, true); // hide Modified
    treeView->hide();
    treeView->setEnabled(false);  // Start disabled
    //treeView->setMaximumWidth(300); // optional

    // --- List View (Right)
    dirModel = new QFileSystemModel(this);
    dirModel->setRootPath(QDir::rootPath());
    dirModel->setFilter(QDir::Files | QDir::NoDotAndDotDot);

    listView = new QListView(this);
    listView->setRootIndex(QModelIndex());
    listView->setModel(dirModel);
    listView->hide();
    listView->setEnabled(false);  // Start disabled

    // Change listView
    connect(treeView, &QTreeView::clicked, this, [this](const QModelIndex &index){
        if (listView->isHidden()) {
            listView->show();  // Hide list view
            listView->setEnabled(true);  // Disable list view
        }
        QString path = treeModel->fileInfo(index).absoluteFilePath();
        listView->setRootIndex(dirModel->setRootPath(path));
        qDebug() << "Folder selected:" << path;
    });

    // File Selected
    connect(listView, &QListView::clicked, this, [this](const QModelIndex &index){
        QString selectedFilePath = dirModel->fileInfo(index).absoluteFilePath();
        emit fileSelected(selectedFilePath);
    });

    // File Selected and playback
    connect(listView, &QListView::doubleClicked, this, [this](const QModelIndex &index){
        emit playSound(); // Trigger playback
    });

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(treeView);
    splitter->addWidget(listView);
    splitter->setStretchFactor(1, 1); // make right pane grow more

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(splitter);
    setLayout(layout);
}

void FileBrowserWidget::setRootDirectory(const QString &path) {
    if (path.isEmpty()) {
        treeView->hide();  // Hide tree view
        treeView->setEnabled(false);  // Disable tree view
        listView->hide();
        listView->setEnabled(false);
        return;
    }
    listView->hide();
    listView->setEnabled(false);

    treeView->show();
    treeView->setEnabled(true);  // Enable tree view

    QModelIndex treeIndex = treeModel->index(path);
    treeView->setRootIndex(treeIndex);

    QModelIndex listIndex = dirModel->index(path);
    listView->setRootIndex(listIndex);
}