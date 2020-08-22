/*
 * browsedirectorydialog.h
 *
 * implementation of BrowseDirectoryDialog class
 *
 * Boris Kumpar 08.12.2012
 *
 */

//#include <QFileDialog>
#include <QGridLayout>

#include "browsedirectorydialog.h"

//_OutputFolder = QFileDialog::getExistingDirectory(0, ("Select Output Folder"), QDir::currentPath());

BrowseDirectoryDialog::BrowseDirectoryDialog(QWidget * parent) :
    QFileDialog(parent)
{
    recursiveCheckBox = new QCheckBox(tr("Recursive search"), this );
    recursiveCheckBox->setChecked(true);
    this->setFileMode(QFileDialog::Directory);
    this->setViewMode(QFileDialog::List);
    this->setOption( QFileDialog::DontUseNativeDialog, true);
    this->setOption( QFileDialog::ShowDirsOnly, true);
    this->setDirectory( QDir::homePath());
    this->setWindowTitle(tr("Choose directory to search" ));
    customize();
}


 void BrowseDirectoryDialog::customize()
 {
    QGridLayout *layout = qobject_cast<QGridLayout *>(QFileDialog::layout());
    int lastRow = layout->rowCount()-1;
    QWidget * w0 = layout->itemAtPosition(lastRow, 0)->widget();
    w0->hide();
    QWidget * w1 = layout->itemAtPosition(lastRow, 1)->widget();
    w1->hide();
    layout->addWidget(recursiveCheckBox, lastRow, 1);
 }

 bool BrowseDirectoryDialog::recursive()
 {
     return  recursiveCheckBox->isChecked();
 }

 void BrowseDirectoryDialog::setRecursive(bool value)
 {
     recursiveCheckBox->setChecked(value);
 }

 QString BrowseDirectoryDialog::directory()
 {
    QStringList fileNames = this->selectedFiles();
    return fileNames[0];
 }
