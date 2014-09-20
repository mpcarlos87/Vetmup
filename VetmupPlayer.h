//!  This class is the responsable to manage the playlist and the player of Vetmup
/*!
  A more elaborate class description.
*/

#ifndef VETMUPPLAYER_H
#define VETMUPPLAYER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QQmlListProperty>
#include <QtMultimedia/QAudioOutput>
#include <QMediaPlayer>

class VetmupPlayer : public QObject
{
    Q_OBJECT
    public:
        //! A normal member taking two arguments and returning an integer value.
        /*!
          \param parent The QObject parent of this class
          \return The Vetmup Player
        */
        explicit VetmupPlayer(QObject *parent = 0);
        //! Destructor of the class
        ~VetmupPlayer();
        //! The copy constructor of the class
        /*!
          \param other The original VetmupPlayer to copy
          \return The new VetmupPlayer
        */
        VetmupPlayer(const VetmupPlayer &other);

        //! This method opens a list of files to reproduce (called from QML)
        /*!
          \param urls The list of urls to open (paths)
        */
        Q_INVOKABLE void openFiles(QList<QUrl> urls);

        //! This method opens a directory and all the subdirectories to reproduce (called from QML)
        /*!
          \param url of the directory (path)
        */
        Q_INVOKABLE void openFolder(QUrl url);

private:
       QMediaPlayer* m_player;
       QMediaPlaylist* m_mediaPlaylist;

       QStringList OpenFolderRecursively(QString folderPath);
};

Q_DECLARE_METATYPE(VetmupPlayer)
#endif // SONGPLAYER_H
