#include "VetmupSong.h"

VetmupSong::VetmupSong(QString title, qint64 duration)
    :m_title(title),m_duration(duration)
{
}

VetmupSong::VetmupSong(const VetmupSong &other)
    :m_duration(other.GetDuration()),m_title(other.GetTitle())
{
}

QString VetmupSong::GetTitle()const
{
    return m_title;
}

qint64 VetmupSong::GetDuration()const
{
    return m_duration;
}
