/*
 * fileinfowidget.h
 *
 * declaration of FileInfoWidget class
 * panel showing information about file
 *
 * Boris Kumpar 24.01.2013
 *
 */
#ifndef FILEINFOWIDGET_H
#define FILEINFOWIDGET_H

#include <QPushButton>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QString>
#include <QMessageBox>
#include <QTreeWidgetItem>

#include "clickablelabel.h"
#include "filemanager.h"
#include "popupbutton.h"

class FileInfoWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit  FileInfoWidget(QWidget *parent = 0);
    ~FileInfoWidget();

public slots:
    void showInfo(QTreeWidgetItem * item);
    void clearData();
    void openContainingDir(QString fileName = "");
    void trashFile(bool confirm = true);
    void deleteFile(bool confirm = true);
    void moveFile();
    void updateCompareFilesButton(QList<QTreeWidgetItem *> similarItems);
    void disableWidgets();
    void enableWidgets();

signals:
    void progressSetupSignal(qint64 fileSize);
    void progressUpdateSignal(qint64 value);
    void progressShowSignal();
    void progressHideSignal();

private slots:
    void compareFiles(QString otherFileName);

private:
    QHBoxLayout * fileInfoLayout;
    QVBoxLayout * fileActionsLayout;
    QGridLayout * gridLayout;
    QLabel * fileNameLabel;
    QLabel * fileSizeLabel;
    QLabel * dateCreatedLabel;
    QLabel * dateModifiedLabel;
    QLabel * ownerLabel;
    QLabel * permissionsLabel;

    ClickableLabel * fileNameText;
    QLabel * fileSizeText;
    QLabel * dateCreatedText;
    QLabel * dateModifiedText;

    QPushButton * trashFileButton;
    QPushButton * deleteFileButton;
    QPushButton * moveFileButton;
    PopUpButton * compareFilesButton;

    FileManager * m_fileManager;
    QString m_fileName;
    QTreeWidgetItem * m_treeWidgetItem;

    void setUpUI();
    void showFileInfo();
    void makeConnections();
    void refresh(QTreeWidgetItem * m_treeWidgetItem);
    QString extractPath(QString fileName);
    QString extractFileName(QString fileName);
    QString extractFileExtension(QString fileName);

};

#endif // FILEINFOWIDGET_H
