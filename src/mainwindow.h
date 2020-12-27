/*
 * mainwindow.h
 *
 * declaration of MainWindow
 *
 * Boris Kumpar 08.12.2012
 *
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QListWidget>
#include <QTreeWidget>
#include <QPushButton>
#include <QStatusBar>
#include <QRadioButton>
#include <QProgressBar>
#include <QStyleOptionProgressBarV2>
#include <QDropEvent>
#include <QDragEnterEvent>
#include "duplicatestreeview.h"
#include "fileinfowidget.h"
#include "optionswidget.h"
#include "actionswidget.h"
#include "duplicatesfinder.h"
#include "filevisitor.h"
#include "collector.h"
#include "directorylist.h"
#include "searchthread.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

private:
    QWidget     * topWidget;        // upper part of centralWidget
    QWidget     * bottomWidget;
    QVBoxLayout * topLayout;
    QVBoxLayout * bottomLayout;
    QHBoxLayout * topHorLayout;
    QSplitter   * splitter;
    QStatusBar * statusBar;
    QProgressBar  * progressBar;
    DirectoryList * directoryList;
    DuplicatesTreeWidget * duplicatesTreeWidget;
    FileInfoWidget  * fileInfoWidget;
    OptionsWidget * optionsWidget;
    ActionsWidget * actionsWidget;
    SearchThread * searchThread;
    void initialize();
    void makeConnections();
    QString humanReadableFileSize(qint64 valueInBytes);
    void disableWidgets();
    void enableWidgets();

    int m_progressIncrement;
    void progressIndefiniteMove();
    QString lastAddedDirectory;
    QString lastMoveDestination;

private slots:
    void showBrowseDirectoryDialog();
    void removeSelectedDirectory();
    void editSelectedDirectory();
    void searchDuplicatesInThread();
    void showFileInfo();
    void openContainingDir(QTreeWidgetItem * item);
    void addDirectoryToList(const QString & directory, bool recursive);
    void showStatus(const QString &text);
    void searchFinished();
    void searchStarted();

public slots:
    void updateProgressBar(qint64 count);
    void setupProgressBar(qint64 value);
    void showProgressBar();
    void hideProgressBar();

};

#endif // MAINWINDOW_H
