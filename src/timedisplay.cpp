/*
 * timedisplay.h
 *
 * implementation of TimeDisplay class
 *
 * Boris Kumpar 21.01.2013
 *
 */
#include "timedisplay.h"



TimeDisplay::TimeDisplay()
    :QElapsedTimer()
{
}


void TimeDisplay::read()
{
    calc();
}


QString TimeDisplay::text(bool showMiliseconds)
{
    calc();
    QString time;
    if(m_hours > 0)
    {
        time= QString("%1h:").arg(m_hours);
    }

    if(m_mins  > 0 || m_hours > 0)
    {
        time+= QString("%1m:").arg(m_mins);
    }

    if(m_secs > 0 || m_mins  > 0 || m_hours > 0)
    {
        time+= QString("%1s").arg(m_secs);
    }

    if((m_msecs  > 0 && showMiliseconds ) ||m_secs > 0 || m_mins  > 0 || m_hours > 0)
    {
        time+= QString(":%1ms").arg(m_msecs);
    }

    return time;
}

int TimeDisplay::msecs()
{
    return m_msecs;
}

int TimeDisplay::secs()
{
    return m_secs;
}

int TimeDisplay::mins()
{
    return m_mins;
}

int TimeDisplay::hours()
{
    return m_hours;
}

void TimeDisplay::calc()
{
    const int SEC_FACTOR = 1000;
    const int MIN_FACTOR = 60 * SEC_FACTOR;
    const int HOUR_FACTOR = 60 * MIN_FACTOR;

    m_msecs = this->elapsed();
    m_hours = m_msecs / HOUR_FACTOR;
    if(m_hours > 0){ m_msecs = m_msecs % HOUR_FACTOR; }

    m_mins = m_msecs / MIN_FACTOR;
    if(m_mins > 0){ m_msecs = m_msecs % MIN_FACTOR; }

    m_secs = m_msecs / SEC_FACTOR;
    if(m_secs > 0){ m_msecs = m_msecs % SEC_FACTOR; }

}
