#include "songplayer.h"
#include <QFile>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QMediaPlaylist>


SongPlayer::SongPlayer(QObject *parent):
    QObject(parent)
{
      qDebug()<<"SongPlayer::SongPlayer(QObject *parent)";

      player = new QMediaPlayer(this);

}

SongPlayer::SongPlayer(const SongPlayer &other){
}

void SongPlayer::buttonClickedQML(QList<QUrl> urls){

    QMediaPlaylist *playlist = new QMediaPlaylist(this);

    foreach(QUrl songPath,urls){
        qDebug()<<"Play "<<songPath;
        playlist->addMedia(songPath);
    }
    playlist->setCurrentIndex(1);
    player->setPlaylist(playlist);
    player->play();
}
