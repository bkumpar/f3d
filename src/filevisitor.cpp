/*
 * filevisitor.cpp
 *
 * implementation of FileVisitor class
 *
 * Boris Kumpar 15.05.2013
 *
 */
#include "filevisitor.h"
#include <QDebug>

FileVisitor::FileVisitor(QString filePattern)
    : m_sortFlags(QDir::NoSort),  m_filterFlags(QDir::NoSymLinks)
{
    m_filePattern = QStringList(filePattern) ;
}

FileVisitor::FileVisitor()
{

}

FileVisitor::~FileVisitor()
{

}

void FileVisitor::setFilePattern(QString filePattern)
{
    m_filePattern = QStringList(filePattern);
}

void FileVisitor::setFilterFlags(QDir::Filters filterFlags)
{
    m_filterFlags = filterFlags;
}


void FileVisitor::setSizeLimit(int minSize, int maxSize)
{
    if( minSize <= maxSize )
    {
        m_minSize = minSize;
        m_maxSize = maxSize;
    }
}

void FileVisitor::processPath(QString path, bool recursive)
{
    QFileInfo fileInfo(path);
    emit start(0);
    processEntry(fileInfo,recursive);
}

void FileVisitor::processEntry(QFileInfo &fileInfo, bool recursive)
{
    if(fileInfo.isDir())   {

        if(fileInfo.fileName()=="." || fileInfo.fileName()==".." )
            return;

        this->dirFound(fileInfo);
        QDir dir(fileInfo.filePath());
        processDir(dir, recursive);
    } else {
        this->fileFound(fileInfo);
    }
}

 void FileVisitor::processDir(QDir dir, bool recursive)
{
    m_filterFlags=QDir::Files | QDir::NoSymLinks | QDir::NoSymLinks;
    QFileInfoList files = dir.entryInfoList(m_filePattern, m_filterFlags, m_sortFlags);

    foreach(QFileInfo fileinfo, files)
        processEntry(fileinfo, recursive);

    if(recursive) {
        QFileInfoList dirs = dir.entryInfoList(QDir::Dirs);
        foreach(QFileInfo dir, dirs)
            processEntry(dir, recursive);
    }
}

void FileVisitor::fileFound(const QFileInfo & fileInfo)
{
    if( fileInfo.size()<=m_maxSize && fileInfo.size()>=m_minSize)
    {
        emit onFileFound(fileInfo);
    }
}

void FileVisitor::dirFound(const QFileInfo & fileInfo)
{
    emit onDirFound(fileInfo);
}
