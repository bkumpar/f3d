#ifndef DUPLICATESTREEWIDGET_H
#define DUPLICATESTREEWIDGET_H

#include <QTreeWidget>
#include <QKeyEvent>
#include <QPoint>
#include <QList>



class DuplicatesTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit DuplicatesTreeWidget(QWidget *parent = 0);
    static bool removeIfSingle(QTreeWidgetItem * item);
    static bool removeItemIfEmpty(QTreeWidgetItem * item);
    static QList<QTreeWidgetItem*> similarFiles(QTreeWidgetItem * item);
    void enableContextmenu();
    void disableContextmenu();

signals:
    void deleteSelected(bool);
    void trashSelected(bool);
    void hideFromList();
    void openContainingDir(QTreeWidgetItem * item);

private slots:
    void showContextMenu(const QPoint &pos);
    void emitDeleteSignal();
    void emitQuickDeleteSignal();
    void emitMoveToTrashSignal();
    void emitHideFromListSignal();
    void emitOpenContainingDir();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    void initialize();
    void makeConnections();
    QMenu * contextMenu;
    QAction * actionDelete;
    QAction * actionQuickDelete;
    QAction * actionTrash;
    QAction * actionSeparator;
    QAction * actionHide;

};

#endif // DUPLICATESTREEWIDGET_H
