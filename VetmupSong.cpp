#include "VetmupSong.h"
#include <QStringList>

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

QString VetmupSong::GetTitleToDisplay() const
{
    QStringList list = m_title.split(" ");
    QString titleModified = QString("");
    int length=0;

    foreach(QString titlePart,list)
    {
        titleModified.append(titlePart);
        length+= titlePart.length();

        if(length > max_length){
            titleModified.append("\n");
            length = 0;
        }
        else{
            titleModified+=" ";
        }
    }

    return titleModified;


}
