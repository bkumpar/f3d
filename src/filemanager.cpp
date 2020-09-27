#include "filemanager.h"
#include <iostream>

FileManager::FileManager(QWidget *parent)
{
#ifdef Q_OS_LINUX
    initTrash();
#endif
}


//
// TODO:
// bool QFile::moveToTrash()
//

#ifdef Q_OS_WIN32

void FileManager::moveToTrash(QString fileName )
{
    std::wstring sFileName = fileName.toStdWString();
    sFileName.append(1, L'\0');        // fileName string must be double nul-terminated
    SHFILEOPSTRUCT shfos = {};
    ZeroMemory (&shfos, sizeof(SHFILEOPSTRUCT));
    shfos.hwnd   = nullptr;       // handle to window that will own generated windows, if applicable
    shfos.wFunc  = FO_DELETE;
    shfos.pFrom  = sFileName.c_str();
    shfos.pTo    = nullptr;       // not used for deletion operations
    shfos.fFlags = FOF_ALLOWUNDO; // use the recycle bin

    const int retVal = SHFileOperation(&shfos);
    if (retVal != 0)
    {
        if(!shfos.fAnyOperationsAborted)
        {
            // The operation failed...
            throw std::runtime_error("Operation failed");
        }

    }
}

#elif defined  Q_OS_LINUX

void FileManager::initTrash()
{
    QStringList paths;
    const char* xdg_data_home = getenv( "XDG_DATA_HOME" );
    if( xdg_data_home ){
        qDebug() << "XDG_DATA_HOME not yet tested";
        QString xdgTrash( xdg_data_home );
        paths.append( xdgTrash + "/Trash" );
    }

    QString home = QDir::homePath();
    paths.append( home + "/.local/share/Trash" );
    paths.append( home + "/.trash" );
    foreach( QString path, paths ){
        if( m_trashPath.isEmpty() ){
            QDir dir( path );
            if( dir.exists() ){
                m_trashPath = path;
            }
        }
    }

    if( m_trashPath.isEmpty() )
        throw std::runtime_error( "Cant detect trash folder" );

    m_trashPathInfo = m_trashPath + "/info";
    m_trashPathFiles = m_trashPath + "/files";
    if( !QDir( m_trashPathInfo ).exists() || !QDir( m_trashPathFiles ).exists() )
        throw std::runtime_error( "Trash doesnt looks like FreeDesktop.org Trash specification" );

}



bool FileManager::moveToTrash(QString fileName)
{

    QFileInfo original( fileName );
    if( !original.exists() )
        throw std::runtime_error( "file doesnt exists, cant move to trash" );

    QString info;
    info += "[Trash Info]\nPath=";
    info += original.absoluteFilePath();
    info += "\nDeletionDate=";
    info += QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzzZ");
    info += "\n";
    QString trashName = original.fileName();
    QString infoPath = m_trashPathInfo + "/" + trashName + ".trashinfo";
    QString filePath = m_trashPathFiles + "/" + trashName;

    int nr = 1;
    while( QFileInfo( infoPath ).exists() || QFileInfo( filePath ).exists() )
    {
        nr++;
        trashName = original.baseName() + "." + QString::number( nr );
        if( !original.completeSuffix().isEmpty() )
        {
            trashName += QString( "." ) + original.completeSuffix();
        }

        infoPath = m_trashPathInfo + "/" + trashName + ".trashinfo";
        filePath = m_trashPathFiles + "/" + trashName;
    }

    QDir dir;
    if( !dir.rename(fileName, filePath ) )
    {
        throw std::runtime_error( "move to trash failed" );
    }

    QFile fileOut(infoPath);
    if (!fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw std::runtime_error( "move to trash failed" );
    }

    QTextStream streamfileOut(&fileOut);
    streamfileOut.setCodec("UTF-8");
    streamfileOut << info;
    streamfileOut.flush();

    fileOut.close();

}
#endif
void FileManager::remove(QString fileName)
{
    QFileInfo original( fileName );
    if( !original.exists() )
        throw std::runtime_error( "file doesnt exists, can't delete" );

    QFile::remove(fileName) ;

}

bool FileManager::binaryEqual(QString fileName1, QString fileName2)
{
    QFile file1(fileName1);
    QFile file2(fileName2);
    file1.open(QFile::ReadOnly);
    file2.open(QFile::ReadOnly);
    if(file1.size() != file2.size())
    {
        return false;
    }
    qint64 fileSize = file1.size();
    emit progressSetupSignal(fileSize);
    uchar *memory1 = file1.map(0, file1.size());
    uchar *memory2 = file2.map(0, file2.size());
    bool identical = false;
    int step = fileSize / 10 + 1;
    if (memory1 && memory2)
    {
        qint64 pos=0;
        while( (pos < fileSize) &&( *memory1 == *memory2))
        {
            if(!(pos % step))
            {
                emit progressUpdateSignal(pos);
            }
            pos++;
            memory1++;
            memory2++;
        }
        identical = ( pos == fileSize);
    }
    file1.unmap(memory1);
    file2.unmap(memory2);
    file1.close();
    file2.close();

    return identical;
}

