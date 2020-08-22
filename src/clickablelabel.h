/*
 * clickablelabel.h
 *
 * declaration of ClickableLabel class
 *
 * Boris Kumpar 06.22.2013
 *
 */


#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget *parent = 0);
    ~ClickableLabel();
    void setLinked(bool linked);
    bool linked();
    void refresh();

signals:
    void clicked();

protected:
    void mousePressEvent ( QMouseEvent * event );

private:
    bool m_linked;
    void linkStyle();

};

#endif // CLICKABLELABEL_H
