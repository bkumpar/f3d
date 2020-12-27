#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QObject>
#include <QThread>
#include <QTreeWidget>
#include "duplicatesfinder.h"
#include "filevisitor.h"
#include "collector.h"
#include "directorylist.h"

class SearchThread : public QThread
{
    Q_OBJECT

public:
    SearchThread();
    ~SearchThread();
    void makeConnections();
    void run();
    void setTreeWidget(QTreeWidget * treeWidget);
    void setStrength(int strength);
    void setFilePattern(const QString &pattern);
    void setSizeLimit(int minSize, int maxSize);
    void setDirectoryList(DirectoryList * directoryList);

signals:
    void showStatus(const QString &message);
    void setupProgressBar(qint64 numberOfcandidates);
    void updateProgressBar(qint64 processed);

private slots:
    void addFileToCollector(const QFileInfo &fileInfo);

private:
    DuplicatesFinder * duplicatesFinder;
    FileVisitor * fileVisitor;
    Collector * collector;
    QTreeWidget * duplicatesTreeWidget;
    DirectoryList * directoryList;
    QString humanReadableFileSize(qint64 valueInBytes);

};

#endif // SEARCHTHREAD_H
