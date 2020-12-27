#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QObject>
#include <stdexcept>

#ifdef Q_OS_WIN32

#include <Windows.h>   // general Windows header file
#include <ShellAPI.h>  // for shell functions, like SHFileOperation
#include <string>

#endif

class FileManager : public QObject
{
    Q_OBJECT

public:
    FileManager(QWidget *parent = 0);

    void moveToTrash(const QString &fileName);
    void remove(const QString &fileName);
    bool binaryEqual(const QString &fileName1, const QString &fileName2);

signals:
    void progressSetupSignal(qint64 fileSize);
    void progressUpdateSignal(qint64 value);

private:
    void initTrash();
    QString m_trashPath;
    QString m_trashPathInfo;
    QString m_trashPathFiles;
};

#endif // FILEMANAGER_H
