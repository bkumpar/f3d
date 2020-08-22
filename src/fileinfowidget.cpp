/*
 * fileinfowidget.h
 *
 * implementation of FileInfoWidget class
 * panel showing information about file
 *
 * Boris Kumpar 24.01.2013
 *
 */

#include <QDebug>
#include <QDateTime>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

#include "fileinfowidget.h"
#include "filebrowser.cpp"
#include "duplicatestreeview.h"
#include "browsedirectorydialog.h"

FileInfoWidget::FileInfoWidget(QWidget *parent) :
    QWidget(parent)
{
    m_fileManager = new FileManager();
    setUpUI();
    makeConnections();

}

FileInfoWidget::~FileInfoWidget()
{
}


void FileInfoWidget::clearData()
{
    fileNameText->setText("");
    fileNameText->setLinked(false);
    fileSizeText->setText("");
    dateCreatedText->setText("");
    dateModifiedText->setText("");
}

void FileInfoWidget::showInfo(QTreeWidgetItem * item)
{
    m_treeWidgetItem = item;
    int cc = item->columnCount();
    if(!item || cc == 2)
    {
        return;
    }
    QString fileName =  item->data(0,Qt::DisplayRole).toString();
    m_fileName = fileName;
    QFileInfo fileInfo(fileName);
    bool exists = fileInfo.exists();
    fileNameText->setText(fileName);
    fileNameText->setLinked(exists);
    fileSizeText->setText(exists ? QString("%1").arg(fileInfo.size()) : QString(""));
    dateCreatedText->setText(exists ? QString("%1").arg(fileInfo.created().toString("dd.MM.yyyy hh:mm:ss.zzz")):QString(""));
    dateModifiedText->setText(exists ? QString("%1").arg(fileInfo.lastModified().toString("dd.MM.yyyy hh:mm:ss.zzz")):QString(""));
    QList<QTreeWidgetItem *> similarItems = DuplicatesTreeWidget::similarFiles(item);
    updateCompareFilesButton(similarItems);
}

void FileInfoWidget::setUpUI()
{


    fileInfoLayout = new QHBoxLayout(this);
    fileActionsLayout = new QVBoxLayout();
    gridLayout = new QGridLayout();

    fileInfoLayout->addLayout(gridLayout, 0);
    fileInfoLayout->addLayout(fileActionsLayout,0);


    trashFileButton = new QPushButton(QIcon(":/icons/trash"),tr("Trash"),this);
    deleteFileButton = new QPushButton(QIcon(":/icons/delete"),tr("Delete"),this);
    moveFileButton = new QPushButton(QIcon(":/icons/move"),tr("Move"),this);
    compareFilesButton = new PopUpButton(QIcon(":/icons/compare"),tr("Compare with..."),this);

    fileActionsLayout->addWidget(trashFileButton);
    fileActionsLayout->addWidget(deleteFileButton);
    fileActionsLayout->addWidget(moveFileButton);
    fileActionsLayout->addWidget(compareFilesButton);

    fileNameLabel = new QLabel(tr("File name:"),this);
    QFont labelFont=fileNameLabel->font();
    labelFont.setPointSize(labelFont.pointSize()-2);
    fileNameLabel->setFont(labelFont);
    fileNameLabel->setAlignment(Qt::AlignRight);
    gridLayout->addWidget(fileNameLabel,0,0);

    fileSizeLabel = new QLabel(tr("File size:"),this);
    fileSizeLabel->setFont(labelFont);
    fileSizeLabel->setAlignment(Qt::AlignRight);
    gridLayout->addWidget(fileSizeLabel,1,0);

    dateCreatedLabel = new QLabel(tr("Date created:"),this);
    dateCreatedLabel->setFont(labelFont);
    dateCreatedLabel->setAlignment(Qt::AlignRight);
    gridLayout->addWidget(dateCreatedLabel,2,0);

    dateModifiedLabel = new QLabel(tr("Date modified:"),this);
    dateModifiedLabel->setFont(labelFont);
    dateModifiedLabel->setAlignment(Qt::AlignRight);
    gridLayout->addWidget(dateModifiedLabel,3,0);

    fileNameText = new ClickableLabel(this);
    fileNameText->setFont(labelFont);
    fileNameText->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    fileNameText->setToolTip(tr("Click to open directory containig file"));
    gridLayout->addWidget(fileNameText,0,1);

    fileSizeText = new QLabel(this);
    fileSizeText->setFont(labelFont);
    gridLayout->addWidget(fileSizeText,1,1);

    dateCreatedText = new QLabel(this);
    dateCreatedText->setFont(labelFont);
    gridLayout->addWidget(dateCreatedText,2,1);

    dateModifiedText = new QLabel(this);
    dateModifiedText->setFont(labelFont);
    gridLayout->addWidget(dateModifiedText,3,1);

    gridLayout->setColumnStretch(0,25);
    gridLayout->setColumnStretch(1,75);

}

void FileInfoWidget::openContainingDir(QString fileName)
{
    if (fileName == "")
    {
        fileName = fileNameText->text();
    }
    select(fileName);

    return;
}

void FileInfoWidget::makeConnections()
{
    connect(fileNameText, SIGNAL(clicked()), this, SLOT(openContainingDir()));
    connect(trashFileButton, SIGNAL(clicked()), this, SLOT(trashFile()));
    connect(deleteFileButton, SIGNAL(clicked()), this, SLOT(deleteFile()));
    connect(moveFileButton, SIGNAL(clicked()), this, SLOT(moveFile()));
    connect(compareFilesButton, SIGNAL(choosed(QString)), this, SLOT(compareFiles(QString)));
    connect(m_fileManager, SIGNAL(progressSetupSignal(qint64)), this, SIGNAL(progressSetupSignal(qint64)));
    connect(m_fileManager, SIGNAL(progressUpdateSignal(qint64)), this, SIGNAL(progressUpdateSignal(qint64)));
}

void FileInfoWidget::compareFiles(QString otherFileName)
{
    emit progressShowSignal();
    bool identical = m_fileManager->binaryEqual(otherFileName, m_fileName);
    emit progressHideSignal();
    if(identical)
    {
        QMessageBox::information(this
                                 , tr("Compare files")
                                 , tr("Files\n %1 and \n %2 \n ARE IDENTICAL!").arg(m_fileName).arg(otherFileName)
                                 ,QMessageBox::Ok );
    }
    else
    {
        QMessageBox::information(this
                                 , tr("Compare files")
                                 , tr("Files are NOT IDENTICAL!")
                                 , QMessageBox::Ok);
    }
}

void FileInfoWidget::refresh(QTreeWidgetItem * treeWidgetItem)
{
    showInfo(treeWidgetItem);
}

void FileInfoWidget::trashFile(bool confirm)
{

    QString fileName = this->fileNameText->text();
    bool remove = true;
    if(confirm)
    {
        QMessageBox::StandardButton button =  QMessageBox::question (this
                                                                     , tr("Move to trash")
                                                                     , tr("Move file %1 to trash?").arg(fileName)
                                                                     , QMessageBox::Yes|QMessageBox::No );
        remove = ( button == QMessageBox::Yes  );
    }
    if( remove )
    {
        FileManager fm;
        fm.moveToTrash(fileName);
        refresh(m_treeWidgetItem);
    }
}

void FileInfoWidget::deleteFile(bool confirm)
{
    QString fileName = this->fileNameText->text();
    bool remove = true;
    if(confirm)
    {
        QMessageBox::StandardButton button =  QMessageBox::question(this
                                                                    , tr("Delete file")
                                                                    , tr("Delete file %1?").arg(fileName)
                                                                    ,QMessageBox::Yes|QMessageBox::No );
        remove = (button == QMessageBox::Yes);
    }
    if( remove )
    {
        m_fileManager->remove(fileName);
        delete m_treeWidgetItem;
        refresh(m_treeWidgetItem);
    }

}

void FileInfoWidget::moveFile()
{
    QString sourceFileName = this->fileNameText->text();
    QFileInfo fi(sourceFileName);
    QString fileName = fi.fileName();
    QString directoryName = fi.absolutePath();
    BrowseDirectoryDialog dialog(this);
    dialog.setDirectory(directoryName );
    dialog.setRecursive(false);

    if(dialog.exec()== QDialog::Accepted)
    {
        QString newDirectoryName = dialog.directory();
        QMessageBox::StandardButton button =  QMessageBox::question(this
                                                                    , tr("Move file")
                                                                    , tr("Move file %1 to %2?").arg(fileName).arg(newDirectoryName)
                                                                    ,QMessageBox::Yes|QMessageBox::No );
        if (button == QMessageBox::Yes)
        {
            QFile newFileName (newDirectoryName + "/" + fileName );
            if(newFileName.exists())
            {
                QMessageBox::StandardButton button =  QMessageBox::question(this
                                                                            , tr("Warning")
                                                                            , tr("Destination file exists. Overwrite?")
                                                                            , QMessageBox::Yes|QMessageBox::No );
                if(button == QMessageBox::No)
                {
                    return;
                }
            }
            if (QFile::copy(sourceFileName, newFileName.fileName()))
            {
                QFile::remove(sourceFileName);
                refresh(m_treeWidgetItem);
            }
        }
    }
}

void FileInfoWidget::updateCompareFilesButton(QList<QTreeWidgetItem *> similarItems)
{
    compareFilesButton->clearItems();

    foreach (QTreeWidgetItem * item, similarItems)
    {
        QString fileName = item->data(0,Qt::DisplayRole).toString();
        compareFilesButton->addItem(fileName);
    }
}
