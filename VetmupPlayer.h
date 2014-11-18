//!  This class is the responsable to manage the playlist and the player of Vetmup
/*!
  This class has a media player and a playlist who manages the list of songs to play on Vetmup.
*/

#ifndef VETMUPPLAYER_H
#define VETMUPPLAYER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QQmlListProperty>
#include <QtMultimedia/QAudioOutput>
#include <QMediaPlayer>
#include <QList>
#include <QMediaContent>
#include <QImage>
#include "VetmupSong.h"

class VetmupPlayer : public QObject
{
    Q_OBJECT
    public:
        //! The constructor of the class.
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
        Q_INVOKABLE void OpenFiles(QList<QUrl> urls);

        //! This method opens a directory and all the subdirectories to reproduce (called from QML)
        /*!
          \param url of the directory (path)
        */
        Q_INVOKABLE void OpenFolder(QUrl url);

        //! This method is called when the screen is clicked. It plays or pause the songs.
        /*!
        */
        Q_INVOKABLE void PlayPause();

        //! This method deletes all the contents into the playlist and stops the player.
        /*!
        */
        Q_INVOKABLE void DeletePlaylist();

        //! This method starts to play the next song of the list
        /*!
        */
        Q_INVOKABLE void NextSong();

        //! This method starts to play the previous song of the list
        /*!
        */
        Q_INVOKABLE void PreviousSong();
        //! This method starts to play the song into the list in the index (index)
        /*!
          \param index Index on the playlist of the new song.
        */
        Q_INVOKABLE void PlaySong(int index);
        //! This method deletes the song into the list in the index (index)
        /*!
          \param index Index on the playlist of the new song.
        */
        Q_INVOKABLE void DeleteSong(int index);
        //! This method returns true if Vetmup has songs into the playlist
        /*!
        */
        Q_INVOKABLE bool HasSongs();

        //! This method sets the time of the actual song
        /*!
        */
        Q_INVOKABLE void SetSongTime(int miliseconds);

        //! This method sets the volume of the VetmupPlayer
        /*!
        */
        Q_INVOKABLE void SetVolume(double volume);

        //! This method saves the current playlist into the file determined.
        /*!
          \param file Path/Name of the new file.
        */
        Q_INVOKABLE void SavePlaylist(QUrl file);

        //! Enable or Disable the shuffle of the current playlist
        /*!
          \param enable If is enabled or not
        */
        Q_INVOKABLE void ShufflePlaylist(bool enable);
private:
       QMediaPlayer* m_player;
       QMediaPlaylist* m_mediaPlaylist;
       qint64 m_playerPosition;
       QString m_thumbnailPath;
       QImage m_thumbnail;

       //! This method does a recursive searching for songs into a folder
       /*!
         \param folderPath The path of the folder to search
         \return The list with the urls of all the songs inside the folder (and child folders)
       */
       QList<QUrl> OpenFolderRecursively(QString folderPath);

       //! This method creates the media content from the list of paths.
       /*!
         \param listOfSongs The list of paths to the songs
         \return The list with media content to play
       */
       QList<QMediaContent> GetContent(QList<QUrl> listOfSongs);

       //! Process the time formatted to the vetmup UI
       /*!
         \param mseconds The miliseconds of the song / current moment
         \return The String with the time in 00:00:00 format.
       */
       QString GetTimeString(int mseconds);


       QList<QMediaContent> OpenPlayList(QUrl path);

private slots:
       void mediaInsertedSlot(int,int);
       void	mediaAboutToBeRemovedSlot(int, int);
       void	mediaRemovedSlot(int, int);
       void positionChangedSlot(qint64 position);
       void durationChangedSlot(qint64 duration);
       void metaDataChangedSlot();

signals:

       void songChangedSignal(QString title, qint64 duration, QString durationText, int index);
       void sliderPositionChangedSignal(qint64 position,QString positionText);
       void songAddedSignal(QString title);
       void deletePlaylistSignal();
       void deleteSongSignal(int index);
       void songMetaDataChangedSignal(QString sourceUrl);
};

Q_DECLARE_METATYPE(VetmupPlayer)
#endif // SONGPLAYER_H
