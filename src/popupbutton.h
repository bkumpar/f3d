#ifndef POPUPBUTTON_H
#define POPUPBUTTON_H

#include <QPushButton>
#include <QMenu>

class PopUpButton : public QPushButton
{
    Q_OBJECT

public:
    explicit PopUpButton(QWidget *parent = 0);
    PopUpButton( const QIcon & icon, const QString & text, QWidget * parent = 0);
    ~PopUpButton();
    void addItem(const QString &item);
    void clearItems();

signals:
    void choosed(const QString &text);

private slots:
    void chooseMenuItem();

private:
    QMenu * menu;
    
};

#endif // POPUPBUTTON_H
