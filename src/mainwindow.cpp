/*
 * mainwindow.h
 *
 * implementation of MainWindow class
 *
 * Boris Kumpar 08.12.2012
 *
 */
#include <QDebug>
#include <QListWidgetItem>
#include <QIcon>
#include <QApplication>
#include <QList>
#include <QMimeData>
#include <QDragEnterEvent>

#include "mainwindow.h"
#include "browsedirectorydialog.h"
#include "timedisplay.h"
#include "fileinfowidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    lastAddedDirectory = QDir::homePath();
    lastMoveDestination = QDir::homePath();

    initialize();
    makeConnections();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initialize()
{

    this->setWindowIcon(QIcon(":/icons/main"));
    splitter = new QSplitter(this);
    splitter->setObjectName(QString::fromUtf8("splitter"));
    splitter->setOrientation(Qt::Vertical);

    topWidget = new QWidget(this);
    topWidget->setObjectName(QString::fromUtf8("topWidget"));
    bottomWidget = new QWidget(this);
    bottomWidget->setObjectName(QString::fromUtf8("bottomWidget"));
    splitter->addWidget(topWidget);
    splitter->addWidget(bottomWidget);

    topLayout = new QVBoxLayout(topWidget);
    topLayout->setObjectName(QString::fromUtf8("topLayout"));
    bottomLayout = new QVBoxLayout(bottomWidget);
    bottomLayout->setObjectName(QString::fromUtf8("bottomLayout"));

    topHorLayout = new QHBoxLayout();
    topHorLayout->setObjectName(QString::fromUtf8("topHorLayout"));

    directoryList = new DirectoryList(topWidget);
    directoryList->setObjectName(QString::fromUtf8("directoryList"));

    actionsWidget =  new ActionsWidget(topWidget)  ;
    actionsWidget->setObjectName(QString::fromUtf8("actionsWidget"));

    topHorLayout->addWidget(directoryList);
    topHorLayout->addWidget(actionsWidget);

    optionsWidget = new OptionsWidget(topWidget);
    optionsWidget->setObjectName(QString::fromUtf8("optonsWidget"));


    topLayout->addLayout(topHorLayout);
    topLayout->addWidget(optionsWidget);

    duplicatesTreeWidget = new DuplicatesTreeWidget(bottomWidget);
    duplicatesTreeWidget->setObjectName(QString::fromUtf8("duplicatesDisplay"));

    fileInfoWidget = new FileInfoWidget(this);
    fileInfoWidget->setObjectName(QString::fromUtf8("fileInfoWidget"));

    bottomLayout->addWidget(duplicatesTreeWidget);
    bottomLayout->addWidget(fileInfoWidget);

    progressBar = new QProgressBar(this);
    progressBar->setObjectName(QString::fromUtf8("progressBar"));
    progressBar->setVisible(false);
    progressBar->setInvertedAppearance(true);

    statusBar = new QStatusBar(this);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    statusBar->insertPermanentWidget(0, progressBar);

    this->setStatusBar(statusBar);
    this->setCentralWidget(splitter);

    searchThread = new SearchThread();
    searchThread->setObjectName(tr("f3dSearchTread"));

}

void  MainWindow::makeConnections()
{
    connect(duplicatesTreeWidget,SIGNAL(itemSelectionChanged()), this, SLOT(showFileInfo()));
    connect(duplicatesTreeWidget,SIGNAL(openContainingDir(QTreeWidgetItem*)), this, SLOT(openContainingDir(QTreeWidgetItem*)));
    connect(duplicatesTreeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(openContainingDir(QTreeWidgetItem*)));
    connect(duplicatesTreeWidget,SIGNAL(deleteSelected(bool)), fileInfoWidget, SLOT(deleteFile(bool)));

    connect(actionsWidget,SIGNAL(actionAdd()), this, SLOT(showBrowseDirectoryDialog()));
    connect(actionsWidget,SIGNAL(actionEdit()), this, SLOT(editSelectedDirectory()));
    connect(actionsWidget,SIGNAL(actionRemove()), this, SLOT(removeSelectedDirectory()));
    connect(actionsWidget,SIGNAL(actionSearch()), this, SLOT(searchDuplicatesInThread()));

    connect(fileInfoWidget, SIGNAL(progressSetupSignal(qint64)), this, SLOT(setupProgressBar(qint64)));
    connect(fileInfoWidget, SIGNAL(progressUpdateSignal(qint64)), this, SLOT(updateProgressBar(qint64)));

    connect(searchThread, SIGNAL(showStatus(QString)), this, SLOT(showStatus(QString)));
    connect(searchThread, SIGNAL(setupProgressBar(qint64)), this, SLOT(setupProgressBar(qint64)));
    connect(searchThread, SIGNAL(updateProgressBar(qint64)), this, SLOT(updateProgressBar(qint64)));
    connect(searchThread, SIGNAL(finished()), this, SLOT(searchFinished()));
    connect(searchThread, SIGNAL(started()), this, SLOT(searchStarted()));
}

void MainWindow::setupProgressBar(qint64 value)
{
    progressBar->setInvertedAppearance(false);
    progressBar->setMinimum(0);
    progressBar->setMaximum(value);
    progressBar->setValue(0);
}

void MainWindow::updateProgressBar(qint64 value)
{
    bool indefinite = false;
    progressBar->setValue(value);
    if(!indefinite)
    {
        return;
    }
    if(    progressBar->value() == progressBar->maximum()
           || progressBar->value() == progressBar->minimum() )
    {
        m_progressIncrement = -m_progressIncrement;
        progressBar->setInvertedAppearance(!progressBar->invertedAppearance());
    }
}

void MainWindow::addDirectoryToList(const QString & directory, bool recursive)
{
    directoryList->addDirectory(directory, recursive);
    lastAddedDirectory = directory;
}

void MainWindow::showBrowseDirectoryDialog()
{
    BrowseDirectoryDialog dialog(this);
    dialog.setDirectory(lastAddedDirectory);
    if(dialog.exec()== QDialog::Accepted)
    {
        addDirectoryToList(dialog.directory(), dialog.recursive());
    }
}

void MainWindow::removeSelectedDirectory()
{
    directoryList->removeSelectedDirectory();
}

void MainWindow::editSelectedDirectory()
{
    if(directoryList->selectedItems().count()==1) {
        BrowseDirectoryDialog dialog(this);
        QListWidgetItem * item = directoryList->selectedItems().first();
        dialog.setDirectory(item->text());
        dialog.setRecursive(item->data(ROLE_RECURSIVE).toBool());
        if(dialog.exec()== QDialog::Accepted) {
            directoryList->setItemData(item, dialog.directory(), dialog.recursive());
        }
    }
}

void MainWindow::searchDuplicatesInThread()
{
    if(searchThread->isRunning())
    {
        searchThread->requestInterruption();
        showStatus("Stopping...");
    }
    else
    {
        searchThread->setStrength(optionsWidget->strength());
        searchThread->setSizeLimit(optionsWidget->minSize(), optionsWidget->maxSize());
        searchThread->setFilePattern(optionsWidget->pattern());
        searchThread->setTreeWidget(this->duplicatesTreeWidget);
        searchThread->setDirectoryList(this->directoryList);
        searchThread->start();
    }
}
void MainWindow::showStatus(const QString &text)
{
    statusBar->showMessage(text);
    statusBar->repaint();
}

void MainWindow::showFileInfo()
{
    QList<QTreeWidgetItem *> selectedItems = duplicatesTreeWidget->selectedItems();
    if(selectedItems.size()>0)
    {
        QTreeWidgetItem * item = selectedItems[0];
        if(item->columnCount()>2)
        {
            fileInfoWidget->showInfo(item);
            duplicatesTreeWidget->enableContextmenu();
        }
        else
        {
            fileInfoWidget->clearData();
            duplicatesTreeWidget->disableContextmenu();
        }
    }
}

void MainWindow::openContainingDir(QTreeWidgetItem * item)
{
    QString fileName =  item->data(0,Qt::DisplayRole).toString();
    fileInfoWidget->openContainingDir(fileName);
}

void MainWindow::progressIndefiniteMove()
{
    progressBar->setValue(progressBar->value() + m_progressIncrement);
    if(progressBar->value()==progressBar->maximum() || progressBar->value()==progressBar->minimum())
    {
        m_progressIncrement = -m_progressIncrement;
        progressBar->setInvertedAppearance( progressBar->invertedAppearance());
    }
}

QString  MainWindow::humanReadableFileSize(qint64 valueInBytes)
{
    QVector<QString> sizeUnits;
    sizeUnits.append("B");
    sizeUnits.append("KB");
    sizeUnits.append("MB");
    sizeUnits.append("GB");
    sizeUnits.append("TB");
    qint64 value  = valueInBytes;
    qint64 fract = 0;
    int i = 0;
    QString sizeUnit = sizeUnits[i];
    while (value > 1024)
    {
        fract = (value % 1024) / 100;
        value = value / 1024;
        i++;
        sizeUnit = sizeUnits[i];
    }
    QString retval = QString("%1.%2%3").arg(value).arg(fract).arg(sizeUnit);
    return retval;
}

void MainWindow::showProgressBar()
{
    this->progressBar->setVisible(true);
}

void MainWindow::hideProgressBar()
{
    this->progressBar->setVisible(false);
}

void MainWindow::disableWidgets()
{
    optionsWidget->disableWidgets();
    fileInfoWidget->disableWidgets();
    actionsWidget->disableWidgets();
}

void MainWindow::enableWidgets()
{
    optionsWidget->enableWidgets();
    actionsWidget->enableWidgets();
}

void MainWindow::searchFinished()
{
    this->hideProgressBar();
    this->actionsWidget->restoreSearchButtonRole();
    this->enableWidgets();
}

void MainWindow::searchStarted()
{
    this->disableWidgets();
    this->actionsWidget->changeSearchButtonRole();
    showStatus(tr("Collecting files ..."));
    this->showProgressBar();
}
