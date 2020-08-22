#include "collector.h"

Collector::Collector() : QObject()
{

}

void Collector::clear()
{
    _candidatesMap.clear();
    _numberOfFiles = 0;
}

Candidates & Collector::candidates()
{
    return _candidatesMap;
}

int Collector::numberOfFiles()
{
    return _numberOfFiles;

}

void Collector::add(QFileInfo fileInfo)
{
    qint64 fileSize = fileInfo.size();
    Candidates::iterator candidatesIterator = _candidatesMap.find(fileSize);
    if(candidatesIterator == _candidatesMap.end())
    {
        std::pair<Candidates::iterator, bool> ptr;
        SameSizeFilesSet emptySameSizeSet;
        ptr = _candidatesMap.insert(std::pair<long, SameSizeFilesSet>(fileSize, emptySameSizeSet));
        candidatesIterator = ptr.first;
    }
    SameSizeFilesSet & nameSet = candidatesIterator->second;
    QString fileName = fileInfo.absoluteFilePath();

    nameSet.insert(fileName);
    _numberOfFiles++;
}
