/*
 * clickablelabel.h
 *
 * implementation of ClickableLabel class
 *
 * Boris Kumpar 06.22.2013
 *
 */


#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget *parent) :
    QLabel(parent)
{
}

ClickableLabel::~ClickableLabel()
{

}

void ClickableLabel::setLinked(bool linked)
{
    m_linked = linked;
    linkStyle();
}


bool ClickableLabel::linked()
{
    return m_linked;
}

void ClickableLabel::refresh()
{
    linkStyle();
}

void ClickableLabel::mousePressEvent ( QMouseEvent * event )
{
    if(m_linked)
        emit clicked();
}

void ClickableLabel::linkStyle()
{
    QFont font = this->font();
    font.setUnderline(m_linked);
    if(m_linked)
        this->setStyleSheet("QLabel { color : blue; }");
    else
        this->setStyleSheet("QLabel { color : black; }");

    this->setFont(font);

}
