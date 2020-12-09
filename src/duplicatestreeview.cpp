
#include <QHeaderView>
#include <QAction>
#include <QMenu>

#include "duplicatestreeview.h"

DuplicatesTreeWidget::DuplicatesTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setUpUI();
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu(const QPoint &)));
}

void DuplicatesTreeWidget::setUpUI()
{
    setColumnCount(2);
    setColumnWidth(0,300);
    QStringList headerLabels = QStringList();
    headerLabels << tr("Size/File name") << tr("Count") << tr("Checksum");
    setHeaderLabels(headerLabels);
}

void DuplicatesTreeWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Delete:
    {
        Qt::KeyboardModifiers keyboardModifiers = event->modifiers();
        if(keyboardModifiers  == Qt::NoModifier) // prompt to delete
        {
            emitDeleteSignal();
        }
        if((keyboardModifiers & Qt::ShiftModifier) == Qt::ShiftModifier) // don't prompt to delete
        {
            emitQuickDeleteSignal();
        }
        break;
    }
    case Qt::Key_Enter:
    case Qt::Key_Return:
    {
        emitOpenContainingDir();
        break;
    }
    default:
        QTreeWidget::keyPressEvent(event);
    }
}

void DuplicatesTreeWidget::showContextMenu(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction action1("Delete", this);
    connect(&action1, SIGNAL(triggered()), this, SLOT(emitDeleteSignal()));
    contextMenu.addAction(&action1);

    QAction action2("Quick Delete", this);
    connect(&action2, SIGNAL(triggered()), this, SLOT(emitQuickDeleteSignal()));
    contextMenu.addAction(&action2);

    QAction action3("Move to trash", this);
    connect(&action3, SIGNAL(triggered()), this, SLOT(emitMoveToTrashSignal()));
    contextMenu.addAction(&action3);

    QAction action4(this);
    action4.setSeparator(true);
    contextMenu.addAction(&action4);

    QAction action5("Hide from this list", this);
    connect(&action5, SIGNAL(triggered()), this, SLOT(emitHideFromListSignal()));
    contextMenu.addAction(&action5);


    contextMenu.exec(mapToGlobal(pos));
}

void DuplicatesTreeWidget::emitDeleteSignal()
{
    emit deleteSelected(true);
    QTreeWidgetItem *item = this->selectedItems()[0];
    removeIfSingle(item);
}

void DuplicatesTreeWidget::emitQuickDeleteSignal()
{
    emit deleteSelected(false);
    QTreeWidgetItem *item = this->selectedItems()[0];
    removeIfSingle(item);
}

void DuplicatesTreeWidget::emitMoveToTrashSignal()
{
    emit trashSelected(true);
}

void DuplicatesTreeWidget::emitHideFromListSignal()
{
    emit hideFromList();
    QTreeWidgetItem *item = this->selectedItems()[0];
    delete item;
    if(item->parent())
    {
        removeItemIfEmpty(item->parent());
    }
}

void DuplicatesTreeWidget::emitOpenContainingDir()
{
    QTreeWidgetItem *item = this->selectedItems()[0];
    if( item)
    {
        emit openContainingDir(item);
    }


}

QList<QTreeWidgetItem*> DuplicatesTreeWidget::similarFiles(QTreeWidgetItem * item)
{
    QList<QTreeWidgetItem*> similarFiles;
    QTreeWidgetItem * parent = item->parent();
    QString hash = item->data(2,Qt::DisplayRole).toString();
    for(int i = 0; i< parent->childCount(); i++)
    {
        QTreeWidgetItem * child = parent->child(i);
        if(child == item)
        {
            continue;
        }
        QString h = child->data(2,Qt::DisplayRole).toString();
        if( h==hash)
        {
            similarFiles << child;
        }
    }
    return similarFiles;
}

bool DuplicatesTreeWidget::removeIfSingle(QTreeWidgetItem * item)
{
    QList<QTreeWidgetItem*> items = similarFiles(item);
    if( items.size() == 0)
    {
        delete item;
        removeItemIfEmpty(item->parent());
    }
    return (items.size() == 1);
}

bool DuplicatesTreeWidget::removeItemIfEmpty(QTreeWidgetItem * item)
{
    bool isEmpty = (item->childCount() == 0);
    if( isEmpty)
    {
        delete item;
    }
    return (isEmpty);
}

void DuplicatesTreeWidget::refreshTree()
{


}
