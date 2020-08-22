#ifndef ACTIONSWIDGET_H
#define ACTIONSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class ActionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ActionsWidget(QWidget *parent = 0);
     ~ActionsWidget();
    enum Action{Add, Remove, Edit, Search};

signals:
    void actionAdd();
    void actionRemove();
    void actionEdit();
    void actionSearch();

public slots:

private:
    QPushButton * addDirectoryButton;
    QPushButton * editDirectoryButton;
    QPushButton * removeDirectoryButton;
    QPushButton * startSearchButton;
    QVBoxLayout * buttonsLayout;
    void setUpUI();
    void makeConnections();
    
};

#endif // ACTIONSWIDGET_H
