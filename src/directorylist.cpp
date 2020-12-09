#include "directorylist.h"
#include <QMimeData>
#include <QDragEnterEvent>

DirectoryList::DirectoryList(QWidget * parent):QListWidget(parent)
{
    this->setAcceptDrops(true);
}

void DirectoryList::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->accept();
    }
}

void DirectoryList::dragMoveEvent(QDragMoveEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->accept();
    }
}


void DirectoryList::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();

    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        addDirectory(urlList, true);
    }
}

void DirectoryList::addDirectory(QList<QUrl> urlList, bool recursiveSearch)
{
    foreach(QUrl url, urlList)
    {
        QString directory = url.toLocalFile();
        bool recursive = recursiveSearch;
        addDirectory(directory, recursive);
    }
}

void DirectoryList::removeSelectedDirectory()
{
    for(int i=0; i<this->count(); i++ ) {
        QListWidgetItem * item = this->item(i);
        if( item->isSelected())
            delete item;
    }
}

void DirectoryList::setItemData(QListWidgetItem *item, QString directory, bool recursiveSearch)
{
    item->setText(directory);
    item->setData(ROLE_RECURSIVE, recursiveSearch);

    if(recursiveSearch)
        item->setIcon(QIcon(":/icons/search_recursive"));
    else
        item->setIcon(QIcon(":/icons/search_folder"));

}

void DirectoryList::addDirectory(QStringList directoryList, bool recursiveSearch)
{
    foreach (QString directoriy,  directoryList)
    {
        addDirectory(directoriy, recursiveSearch);
    }
}


void DirectoryList::addDirectory(QString directory, bool recursiveSearch)
{
    QListWidgetItem * item = new QListWidgetItem(this,QListWidgetItem::Type);
    this->setItemData(item,  directory, recursiveSearch);
}


