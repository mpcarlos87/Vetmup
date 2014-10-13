//!  This class is used to store a song information (to send it through signals or whatever you want)
/*!
  A more elaborated description.
*/
#ifndef VETMUPSONG_H
#define VETMUPSONG_H

#include <QString>

class VetmupSong
{

public:

    //! The constructor of the class.
    /*!
      \param title The title of the Song.
      \param duration The duration of the song in miliseconds
      \return The Vetmup Song
    */
    explicit VetmupSong(QString title,qint64 duration);
    VetmupSong(const VetmupSong &other);
    ~VetmupSong(){}

    QString GetTitle()const;
    qint64 GetDuration()const;
    QString GetTitleToDisplay() const;

private:

    qint64 m_duration;
    QString m_title;

    const int max_length = 15;
};

#endif // VETMUPSONG_H
