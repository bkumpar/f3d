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

signals:
    void deleteSelected(bool);
    void trashSelected(bool);
    void hideFromList();
    void openContainingDir(QTreeWidgetItem * item);
    void refreshTree();

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
    void setUpUI();

};

#endif // DUPLICATESTREEWIDGET_H
