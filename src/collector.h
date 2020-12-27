#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <QObject>
#include <QFileInfo>
#include <map>
#include <set>
#include <QString>


typedef std::set<QString> SameSizeFilesSet;
typedef std::map<qint64, SameSizeFilesSet> Candidates;

class Collector : public QObject
{
    Q_OBJECT

public:
    explicit Collector();
    Candidates & candidates();
    int numberOfFiles();

public slots:
    void add(const QFileInfo &fileInfo);
    void add(qint64 fileSize, QString fileName);
    void clear();

private:
    int _numberOfFiles;
    Candidates _candidatesMap;
};

#endif // COLLECTOR_H
