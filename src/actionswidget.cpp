#include "actionswidget.h"

ActionsWidget::ActionsWidget(QWidget *parent) :
    QWidget(parent)
{
    setUpUI();
    makeConnections();
}

ActionsWidget::~ActionsWidget()
{

}

void ActionsWidget::setUpUI()
{
    addDirectoryButton = new QPushButton(QIcon(":/icons/add_folder"),tr("Add"),this);
    editDirectoryButton = new QPushButton(QIcon(":/icons/edit_folder"),tr("Edit"),this);
    removeDirectoryButton = new QPushButton(QIcon(":/icons/remove_folder"),tr("Remove"),this);
    startSearchButton = new QPushButton(QIcon(":/icons/search_files"),tr("Search"),this);

    buttonsLayout = new QVBoxLayout(this);

    buttonsLayout->addWidget(addDirectoryButton);
    buttonsLayout->addWidget(editDirectoryButton);
    buttonsLayout->addWidget(removeDirectoryButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(startSearchButton);

}

void ActionsWidget::makeConnections()
{
    connect(addDirectoryButton, SIGNAL(clicked()), this, SIGNAL(actionAdd()));
    connect(removeDirectoryButton, SIGNAL(clicked()), this, SIGNAL(actionRemove()));
    connect(editDirectoryButton, SIGNAL(clicked()), this, SIGNAL(actionEdit()));
    connect(startSearchButton, SIGNAL(clicked()), this, SIGNAL(actionSearch()));
}

void ActionsWidget::disableWidgets()
{
    addDirectoryButton->setEnabled(false);
    editDirectoryButton->setEnabled(false);
    removeDirectoryButton->setEnabled(false);
    startSearchButton->setEnabled(false);
}

void ActionsWidget::enableWidgets()
{
    addDirectoryButton->setEnabled(true);
    editDirectoryButton->setEnabled(true);
    removeDirectoryButton->setEnabled(true);
    startSearchButton->setEnabled(true);
}
