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
    fileVisitor = new FileVisitor();
    duplicatesFinder = new DuplicatesFinder();
    collector = new Collector();
    lastAddedDirectory = QDir::homePath();
    lastMoveDestination = QDir::homePath();

    setUpUI();
    makeConnections();
}

MainWindow::~MainWindow()
{
    delete fileVisitor;
    delete duplicatesFinder;
    delete collector;
}

void MainWindow::setUpUI()
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
    connect(actionsWidget,SIGNAL(actionSearch()), this, SLOT(searchDuplicates()));

    connect(fileVisitor, SIGNAL(start(qint64)), this, SLOT(setupProgressBar(qint64)));
    connect(fileVisitor, SIGNAL(onFileFound(QFileInfo)), collector, SLOT(add(QFileInfo)));

    connect(fileInfoWidget, SIGNAL(progressSetupSignal(qint64)), this, SLOT(setupProgressBar(qint64)));
    connect(fileInfoWidget, SIGNAL(progressUpdateSignal(qint64)), this, SLOT(updateProgressBar(qint64)));
    connect(fileInfoWidget, SIGNAL(progressShowSignal()), this, SLOT(showProgressBar()));
    connect(fileInfoWidget, SIGNAL(progressHideSignal()), this, SLOT(hideProgressBar()));

}

void MainWindow::setupProgressBar(qint64 value)
{
    progressBar->setInvertedAppearance(false);
    progressBar->setMinimum(0);
    progressBar->setMaximum(value);
    progressBar->setValue(0);
}

void MainWindow::updateProgressBar(qint64 value, bool indefinite)
{

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

void MainWindow::addDirectoryToList(QString directory, bool recursive)
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

void MainWindow::searchDuplicates()
{
    TimeDisplay timeDisplay;
    timeDisplay.start();
    collector->clear();
    duplicatesTreeWidget->clear();
    duplicatesFinder->clear();
    duplicatesFinder->setStrength(optionsWidget->strength());
    fileVisitor->setFilePattern(optionsWidget->pattern());
    fileVisitor->setSizeLimit(optionsWidget->minSize(), optionsWidget->maxSize() );

    showStatus(tr("Collecting files ..."));
    this->showProgressBar();
    m_progressIncrement = 1;
    QApplication::processEvents();
    for(int row=0; row< directoryList->count(); row++  )
    {
        QListWidgetItem * listItem = (QListWidgetItem *) directoryList->item(row);
        QString path = listItem->text();
        bool recursive = listItem->data(ROLE_RECURSIVE).toBool();
        fileVisitor->processPath(path,recursive);
    }

    Candidates & candidates = collector->candidates();
    duplicatesFinder->clear();
    showStatus(tr("Searching for duplicates ..."));
    this->setupProgressBar(candidates.size() );
    int processed = 0;
    for( Candidates::iterator it = candidates.begin(); it != candidates.end(); it++)
    {
        qint64 fileSize = it->first;
        SameSizeFilesSet & files = it->second;
        if(files.size() > 1)
        {
            for(SameSizeFilesSet::iterator it2 = files.begin(); it2 != files.end(); it2++ )
            {
                duplicatesFinder->add(fileSize, *it2);
            }
        }
        this->updateProgressBar(++processed);
    }
    timeDisplay.read();
    int totalFilesCompared = duplicatesFinder->numberOfFiles();
    int totalFilesProcessed = collector->numberOfFiles();
    int numberOfDuplicates = duplicatesFinder->numberOfDuplicates();
    qint64 wastedSpace = duplicatesFinder->wastedSpace();

    QList<QTreeWidgetItem *> items;
    SizeMap & duplicatesMap = duplicatesFinder->duplicates();
    this->setupProgressBar(duplicatesMap.size() );
    processed = 0;
    for(SizeMap::iterator sizeMapIterator = duplicatesMap.begin(); sizeMapIterator != duplicatesMap.end(); sizeMapIterator++)
    {
        QTreeWidgetItem *sizeNode = Q_NULLPTR;
        HashMap & hashMap = sizeMapIterator->second;
        for(HashMap::iterator hashMapIterator = hashMap.begin(); hashMapIterator != hashMap.end(); hashMapIterator++)
        {
            QString hash = hashMapIterator->first;
            NameSet & nameSet = hashMapIterator->second;
            if(nameSet.size()>1)
            {
                if(!sizeNode)
                {
                    QStringList sl1;
                    qint64 fileSize = sizeMapIterator->first;
                    sl1 << QString("%1").arg(fileSize) << QString("%1").arg(nameSet.size());
                    sizeNode = new QTreeWidgetItem(duplicatesTreeWidget, sl1,QTreeWidgetItem::Type);
                }
                for(NameSet::iterator nameSetIterator = nameSet.begin(); nameSetIterator != nameSet.end(); nameSetIterator++)
                {
                    QString fileName = *nameSetIterator;
                    QStringList sl2;
                    sl2 <<  fileName << "" << hash ;
                    items.append(new QTreeWidgetItem(sizeNode, sl2,QTreeWidgetItem::Type));
                }
            }
        }
        sizeNode = Q_NULLPTR;
        this->updateProgressBar(++processed);
    }
    duplicatesTreeWidget->insertTopLevelItems(0, items);
    timeDisplay.read();

    showStatus(tr("Done! %1 files processed, %2 files compared,  %3 duplicates found in %4. %5 wasted space.")
               .arg(totalFilesProcessed)
               .arg(totalFilesCompared)
               .arg(numberOfDuplicates)
               .arg(timeDisplay.text())
               .arg(humanReadableFileSize(wastedSpace)));
    this->hideProgressBar();
}

void MainWindow::showStatus(QString text)
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
        }
        else
        {
            fileInfoWidget->clearData();
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
