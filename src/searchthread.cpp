#include "searchthread.h"
#include "timedisplay.h"


SearchThread::SearchThread()
{
    qRegisterMetaType<QFileInfo>(); // must be registered to enable pass parameter of this type from signal to slot
    fileVisitor = new FileVisitor();
    duplicatesFinder = new DuplicatesFinder();
    collector = new Collector();
    makeConnections();
}

SearchThread::~SearchThread()
{
    delete fileVisitor;
    delete duplicatesFinder;
    delete collector;
}

void SearchThread::makeConnections()
{
    bool success = connect(fileVisitor, SIGNAL(onFileFound(QFileInfo)), this, SLOT(addFileToCollector(QFileInfo)));
    Q_ASSERT(success);
}

void SearchThread::addFileToCollector(const QFileInfo &fileInfo)
{
    this->collector->add(fileInfo);
}

void SearchThread::setTreeWidget(QTreeWidget * treeWidget)
{
    duplicatesTreeWidget = treeWidget;
}

void SearchThread::setStrength(int strength){
    duplicatesFinder->setStrength(strength);
}
void SearchThread::setFilePattern(const QString &pattern)
{
    fileVisitor->setFilePattern(pattern);
}
void SearchThread::setSizeLimit(int minSize, int maxSize)
{
    fileVisitor->setSizeLimit(minSize, maxSize);
}

void SearchThread::setDirectoryList(DirectoryList * directoryList)
{
    this->directoryList = directoryList;
}

QString  SearchThread::humanReadableFileSize(qint64 valueInBytes)
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

void SearchThread::run()
{
    TimeDisplay timeDisplay;
    timeDisplay.start();
    collector->clear();
    duplicatesTreeWidget->clear();
    duplicatesFinder->clear();
    emit showStatus(tr("Collecting files ..."));
    emit setupProgressBar(0);
    for(int row=0; row< directoryList->count(); row++  )
    {
        QListWidgetItem * listItem = (QListWidgetItem *) directoryList->item(row);
        QString path = listItem->text();
        bool recursive = listItem->data(ROLE_RECURSIVE).toBool();
        fileVisitor->processPath(path,recursive);
    }
    if(this->isInterruptionRequested())
    {
        emit showStatus(tr("Canceled!"));
        return;
    }
    Candidates & candidates = collector->candidates();
    duplicatesFinder->clear();
    emit showStatus(tr("Searching for duplicates ..."));
    emit setupProgressBar(candidates.size()) ;
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
        emit updateProgressBar(++processed);

        if(this->isInterruptionRequested())
        {
            emit showStatus(tr("Canceled!"));
            return;
        }

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
        if(this->isInterruptionRequested())
        {
            emit showStatus(tr("Canceled!"));
            return;
        }

    }
    duplicatesTreeWidget->insertTopLevelItems(0, items);
    timeDisplay.read();

    showStatus(tr("Done! %1 files processed, %2 files compared,  %3 duplicates found in %4. %5 wasted space.")
               .arg(totalFilesProcessed)
               .arg(totalFilesCompared)
               .arg(numberOfDuplicates)
               .arg(timeDisplay.text())
               .arg(humanReadableFileSize(wastedSpace)));

}


