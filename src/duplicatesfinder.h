#ifndef DUPLICATESFINDER_H
#define DUPLICATESFINDER_H

#include <QString>
#include <map>
#include <set>
#include <QObject>

typedef std::set<QString> NameSet;
typedef std::map<QString, NameSet> HashMap;
typedef std::map<qint64, HashMap> SizeMap;

class DuplicatesFinder : public QObject
{
    Q_OBJECT
public:
    DuplicatesFinder();
    ~DuplicatesFinder();
    SizeMap & duplicates();
    int numberOfFiles();
    int numberOfDuplicates();
    qint64 wastedSpace();
    void setStrength(int strength);

public slots:
    void add(qint64 fileSize, QString fileName);
    void clear();

private:
    SizeMap duplicatesMap;
    int _numberOfFiles;
    int _numberOfDuplicates;
    qint64 _wastedSpace;
    int _strength;
    QString hash(QString fileName,qint64 chunkSize);
};

#endif // DUPLICATESFINDER_H
