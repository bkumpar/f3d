#ifndef SPINBOXEX_H
#define SPINBOXEX_H

#include <QSpinBox>
#include <QChar>
#include <QValidator>

#define BYTE 1;
#define KILOBYTE 1024 * BYTE;
#define MEGABYTE 1024 * KILOBYTE;
#define GIGABYTE 1024 * MEGABYTE;


class SpinBoxEx : public QSpinBox
{
    Q_OBJECT


public:

    enum Unit {
        byte = 0,
        kilo = 1,
        mega = 2,
        giga = 3
    };

    explicit SpinBoxEx(QWidget *parent = 0);
    ~SpinBoxEx();
    void setFileSize(int value);
    int fileSize();

    void setUnit(int value);
    int unit();

    int factor();
signals:

public slots:
    void valueChanged(int i);

protected:
    QValidator::State validate ( QString & input, int & pos );

private:

    typedef struct {
        int scale;
        QChar suffix;
    } SizeUnit ;

    int m_factor;
    int m_unit;
    SizeUnit m_sizeUnit[4];
};

#endif // SPINBOXEX_H
