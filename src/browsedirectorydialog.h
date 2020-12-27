/*
 * browsedirectorydialog.h
 *
 * declaration of BrowseDirectoryDialog
 * reimplements standard QFileDialog to choose only directories
 *
 * Boris Kumpar 08.12.2012
 *
 */
#ifndef BROWSEDIRECTORYDIALOG_H
#define BROWSEDIRECTORYDIALOG_H

#include <QFileDialog>
#include <QCheckBox>

class BrowseDirectoryDialog : public QFileDialog
{
    Q_OBJECT

public:
    BrowseDirectoryDialog(QWidget * parent );
    bool recursive();
    void setRecursive(bool value);
    QString directory();

private:
        void customize();
        QCheckBox *recursiveCheckBox;
};

#endif // BROWSEDIRECTORYDIALOG_H
