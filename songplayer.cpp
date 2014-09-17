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

void SongPlayer::buttonClickedQML(QString path){
    qDebug()<<"Play "<<path;


    sourceFile.setFileName(path);
    if(sourceFile.open(QIODevice::ReadOnly)){
        QMediaPlaylist *playlist = new QMediaPlaylist(this);
        playlist->addMedia(QUrl(path));
        playlist->setCurrentIndex(1);
        player->setPlaylist(playlist);
        player->play();
    }
}
