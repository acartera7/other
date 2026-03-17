

#include "FileBrowserWidget.h"


FileBrowserWidget::FileBrowserWidget(QWidget *parent)
    : QWidget(parent)
{

    setFocusPolicy(Qt::NoFocus);
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
    //treeView->setMaximumWidth(300); // optional

    // --- List View (Right)
    dirModel = new QFileSystemModel(this);
    dirModel->setRootPath(QDir::rootPath());
    dirModel->setFilter(QDir::Files | QDir::NoDotAndDotDot);

    listView = new QListView(this);
    listView->setRootIndex(QModelIndex());
    listView->setModel(dirModel);
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    listView->setDragEnabled(true);
    listView->setDragDropMode(QAbstractItemView::DragOnly);
    listView->setDefaultDropAction(Qt::CopyAction);
    listView->hide();
    listView->setEnabled(false);  // Start disable

    splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(treeView);
    splitter->addWidget(listView);
    splitter->setStretchFactor(1, 1); // make right pane grow more

    layout = new QVBoxLayout(this);
    layout->addWidget(splitter);
    setLayout(layout);

    setFocusProxy(treeView);
    setFocusProxy(listView);

    // Change listView
    connect(treeView, &QTreeView::clicked, this, &FileBrowserWidget::listFolder);

    // File Selected
    connect(listView, &QListView::clicked, this, [this](const QModelIndex &index){
        QString selectedFilePath = dirModel->fileInfo(index).absoluteFilePath();
        emit fileSelected(selectedFilePath);
        qDebug() << "File selected:" << selectedFilePath;
    });

    // File Selected and playback
    connect(listView, &QListView::doubleClicked, this, [this](const QModelIndex &index){
        emit playSound(); // Trigger playback
    });
}

void FileBrowserWidget::setRootDirectory(const QString &path) {
    if (path.isEmpty()) {
        listView->hide();
        listView->setEnabled(false);
        return;
    }
    listView->hide();
    listView->setEnabled(false);

    QModelIndex treeIndex = treeModel->index(path);
    treeView->setRootIndex(treeIndex);

    QModelIndex listIndex = dirModel->index(path);
    listView->setRootIndex(listIndex);
}

void FileBrowserWidget::listFolder(const QModelIndex &index) {
    if (listView->isHidden()) {
        listView->show();
        listView->setEnabled(true);
    }
    QString path = treeModel->fileInfo(index).absoluteFilePath();
    listView->setRootIndex(dirModel->setRootPath(path));
    //qDebug() << "Folder selected:" << path;
}

QJsonObject FileBrowserWidget::saveState() {
    QJsonObject state;
    state["curr-path"] = treeModel->fileInfo(treeView->currentIndex()).absoluteFilePath();
    // TODO if (!listView->isHidden())
    state["root-path"] = treeModel->fileInfo(treeView->rootIndex()).absoluteFilePath();
    return state;
}

void FileBrowserWidget::loadState(QJsonObject state) {

    if ( state.contains("root-path") ) {
        QString rootPath = state["root-path"].toString();
        if (!rootPath.isEmpty()) {
            setRootDirectory(rootPath);
        }
    } else {

    }
    if ( state.contains("curr-path") ) {
        QString currPath = state["curr-path"].toString();
        if (!currPath.isEmpty()) {
            if (listView->isHidden()) {
                listView->show();
                listView->setEnabled(true);
            }
            listView->setRootIndex(dirModel->setRootPath(currPath));
        }
    }

}

//void FileBrowserWidget::focusInEvent(QFocusEvent *event) {
//    qDebug() << "FileBrowserWidget got focus";
//    QWidget::focusInEvent(event);
//}
//
//void FileBrowserWidget::focusOutEvent(QFocusEvent *event) {
//    qDebug() << "FileBrowserWidget lost focus";
//    QWidget::focusOutEvent(event);
//}