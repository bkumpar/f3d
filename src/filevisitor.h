/*
 * filevisitor.h
 *
 * declaration of FileVisitor, class implementing Visitor Pattern for file-traversing
 *
 * Boris Kumpar 15.05.2013
 *
 */
#ifndef FILEVISITOR_H
#define FILEVISITOR_H

#include <QObject>
#include <QDir>
#include <QStringList>


class FileVisitor : public QObject
{
    Q_OBJECT
public:
    explicit FileVisitor(const QString &filePattern);
    explicit FileVisitor();
    ~FileVisitor();

    void setFilterFlags(QDir::Filters filterFlags);
    void setRecursive(bool recursive);
    void setSizeLimit(int minSize, int maxSize);
    void setFilePattern(const QString &filePattern);

signals:
    void onFileFound(const QFileInfo & fileInfo);
    void onDirFound(const QFileInfo & fileInfo);
    void start(qint64 startValue);


public slots:
    void processPath(const QString &path, bool recursive);

private:
    QStringList m_filePattern;
    QDir::SortFlags m_sortFlags;
    QDir::Filters m_filterFlags;
    qint64 m_minSize;
    qint64 m_maxSize;

    void processEntry(QFileInfo & fileInfo, bool recursive);
    void processDir(QDir dir, bool recursive);
//    void countDirs(const QFileInfo &fileInfo, bool recursive);
    void fileFound(const QFileInfo & fileInfo);
    void dirFound(const QFileInfo & fileInfo);

};

#endif // FILEVISITOR_H
