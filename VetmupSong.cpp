#include "VetmupSong.h"

VetmupSong::VetmupSong(QString title, qint64 duration)
    :m_title(title),m_duration(duration)
{
}

VetmupSong::VetmupSong(const VetmupSong &other)
    :m_duration(other.GetDuration(DurationType::Miliseconds)),m_title(other.GetTitle())
{
}

QString VetmupSong::GetTitle()const
{
    return m_title;
}

qint64 VetmupSong::GetDuration(DurationType durationType)const
{
    switch(durationType){
        case DurationType::Miliseconds:
            return m_duration;
        case DurationType::Seconds:
            return m_duration/1000;
    }
}
