#ifndef DIRECTORYLIST_H
#define DIRECTORYLIST_H

#include <QListWidget>
#include <QDropEvent>

#define ROLE_RECURSIVE Qt::UserRole+1

class DirectoryList : public QListWidget
{
public:
    DirectoryList(QWidget * parent);
    void dropEvent(QDropEvent* event) ;
    void dragEnterEvent(QDragEnterEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void addDirectory(QString path, bool recursiveSearch);
    void addDirectory(QStringList pathList, bool recursiveSearch);
    void addDirectory(QList<QUrl> urlList , bool recursiveSearch);
    void removeSelectedDirectory();
    void setItemData(QListWidgetItem * item, QString directory, bool recursiveSearch);
};

#endif // DIRECTORYLIST_H
