#ifndef DUPLICATESFINDER_CPP
#define DUPLICATESFINDER_CPP

#include "duplicatesfinder.h"
#include <QCryptographicHash>
#include <QFile>

DuplicatesFinder::DuplicatesFinder()
{

}
DuplicatesFinder::~DuplicatesFinder()
{

}

void DuplicatesFinder::setStrength(int strength)
{
    _strength = strength;
}


void DuplicatesFinder::clear()
{
    duplicatesMap.clear();
    _numberOfFiles = 0;
    _wastedSpace = 0;
    _numberOfDuplicates = 0;
}


QString DuplicatesFinder::hash(const QString &fileName, qint64 chunkSize)
{
    QFile file(fileName);
    if( chunkSize == 0)
    {
        return "";
    }
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray fileData;
        fileData.resize(chunkSize);
        if (file.read(fileData.data(),fileData.size()) < 0)
        {
            fileData.resize(0);
        }

        QByteArray hash = QCryptographicHash::hash(fileData,QCryptographicHash::Md5);
        file.close();
        return hash.toHex();
    }
    return "";

}


void DuplicatesFinder::add(qint64 fileSize, QString fileName)
{
    SizeMap::iterator sizeMapIterator = duplicatesMap.find(fileSize);
    if(sizeMapIterator == duplicatesMap.end())
    {
        // add entry and return iterator
        std::pair<SizeMap::iterator, bool> ptr;
        HashMap emptyHashMap;
        ptr = duplicatesMap.insert(std::pair<long, HashMap>(fileSize, emptyHashMap));
        sizeMapIterator = ptr.first;
    }

    HashMap &  hashMap = sizeMapIterator->second;
    qint64 chunkSize;
    switch (_strength)
    {
     case 0:
        chunkSize = 0 ;
        break;
    case 1:
        chunkSize = 4096 ;
        break;
    case 2:
        chunkSize = fileSize ;
        break;
     default:
        return;
    }

    QString hash = this->hash(fileName, chunkSize) ;
    HashMap::iterator hashMapIterator = hashMap.find(hash);
    if(hashMapIterator == hashMap.end())
    {
        // add entry and return iterator
        std::pair<HashMap::iterator, bool> ptr;
        NameSet emptyNameSet;
        ptr = hashMap.insert(std::pair<QString, NameSet>(hash, emptyNameSet));
        hashMapIterator = ptr.first;
    }
    else
    {
        _wastedSpace += fileSize;
        _numberOfDuplicates++;
    }
    NameSet & nameSet = hashMapIterator->second;

    nameSet.insert(fileName);
    _numberOfFiles++;
}

int DuplicatesFinder::numberOfDuplicates()
{
    return _numberOfDuplicates;
}

qint64 DuplicatesFinder::wastedSpace()
{
    return _wastedSpace;
}

int DuplicatesFinder::numberOfFiles()
{
    return _numberOfFiles;
}

SizeMap & DuplicatesFinder::duplicates()
{
    return  duplicatesMap;
}

#endif // DUPLICATESFINDER_CPP
