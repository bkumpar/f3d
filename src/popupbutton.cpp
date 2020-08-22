#include "popupbutton.h"

#include <QAction>

PopUpButton::PopUpButton(QWidget *parent) :
    QPushButton(parent)
{
    this->menu = new QMenu("Menu",this);
    this->setMenu(this->menu);
}

PopUpButton::PopUpButton(const QIcon &icon, const QString &text, QWidget *parent):
    QPushButton(icon, text, parent)
{
    this->menu = new QMenu("Menu",this);
    this->setMenu(this->menu);
}

PopUpButton::~PopUpButton()
{
    delete this->menu;
}

void PopUpButton::addItem(QString item)
{
    QAction * a = this->menu->addAction(item);
    a->setData(item);
    connect( a, SIGNAL(triggered()) , this, SLOT(chooseMenuItem()));
}

void PopUpButton::clearItems()
{
    foreach (QAction * action, this->menu->actions())
    {
       delete action;
    }
}

void PopUpButton::chooseMenuItem()
{
    QAction * a = (QAction *) sender(); //Returns a pointer to the object that sent the signal
    QString text = a->data().toString();
    emit choosed(text);
    return;
}
