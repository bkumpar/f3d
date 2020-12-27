#include "spinboxex.h"
#include <QLineEdit>
#include <QValidator>
#include <QDebug>

SpinBoxEx::SpinBoxEx(QWidget *parent) :
    QSpinBox(parent)
{
    m_sizeUnit[byte].scale=BYTE;
    m_sizeUnit[byte].suffix = 'B';

    m_sizeUnit[kilo].scale=KILOBYTE;
    m_sizeUnit[kilo].suffix = 'K';

    m_sizeUnit[mega].scale=MEGABYTE ;
    m_sizeUnit[mega].suffix = 'M';

    m_sizeUnit[giga].scale=GIGABYTE;
    m_sizeUnit[giga].suffix = 'G';

    m_unit = kilo;
    m_factor = m_sizeUnit[m_unit].scale;
    this->setRange(0, 1024);
    this->setSuffix(m_sizeUnit[m_unit].suffix);

    connect(this,SIGNAL(valueChanged(int)), this,SLOT(valueChanged(int)));
}

SpinBoxEx::~SpinBoxEx()
{
}

void SpinBoxEx::setFileSize(int value)
{
    for (int  i=byte; i<giga; i++ )
    {
        if (m_sizeUnit[i].scale <= value && value <=  m_sizeUnit[i+1].scale)
        {
            m_unit = i+1;
            m_factor = m_sizeUnit[m_unit].scale;
            this->setValue(value / m_sizeUnit[m_unit].scale);
            break;
        }
    }
}

int SpinBoxEx::fileSize()
{
    return this->value()*m_factor;
}

int SpinBoxEx::unit()
{
    return m_unit;
}

int SpinBoxEx::factor()
{
    return m_factor;
}

void SpinBoxEx::valueChanged(int i)
{
    if(i==0 )
    {
        if( unit()>byte)
        {
            setValue(1023);
            setUnit(unit()-1);
        }
        else
        {
            setValue(1);
        }
    }
    else if(i==1024 )
    {
        if(unit()<giga)
        {
            setValue(1);
            setUnit(unit()+1);
        }
        else
        {
            this->setValue(1023);
        }
    }
}

void SpinBoxEx::setUnit(int value)
{
    if(m_unit!= value)
    {
        m_unit= value;
        this->setSuffix(m_sizeUnit[m_unit].suffix);
        m_factor = m_sizeUnit[m_unit].scale;
    }
}

QValidator::State SpinBoxEx::validate ( QString & input, int & pos ) const
{
    return QSpinBox::validate (  input, pos );
}
