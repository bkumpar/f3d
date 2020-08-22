/*
 * timedisplay.h
 *
 * declaration of TimeDisplay class
 *
 * Boris Kumpar 21.01.2013
 *
 */
#ifndef TIMEDISPLAY_H
#define TIMEDISPLAY_H
#include <QElapsedTimer>
#include <QString>


class TimeDisplay : public QElapsedTimer
{
public:
    TimeDisplay();

    void read();

    QString text(bool showMiliseconds=false);
    int msecs();
    int secs();
    int mins();
    int hours();

private:
    void calc();
    int m_msecs;
    int m_secs;
    int m_mins;
    int m_hours;

};

#endif // TIMEDISPLAY_H
