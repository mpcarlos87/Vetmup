#include "VetmupPlayer.h"
#include <QFile>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QMediaPlaylist>


VetmupPlayer::VetmupPlayer(QObject *parent):
    QObject(parent)
{
      qDebug()<<"VetmupPlayer::VetmupPlayer(QObject *parent)";

      player = new QMediaPlayer(this);

}

VetmupPlayer::VetmupPlayer(const VetmupPlayer &other){
    this->player = new QMediaPlayer(other.player);
}

void VetmupPlayer::buttonClickedQML(QList<QUrl> urls){

    QMediaPlaylist *playlist = new QMediaPlaylist(this);

    foreach(QUrl songPath,urls){
        qDebug()<<"Play "<<songPath;
        playlist->addMedia(songPath);
    }
    playlist->setCurrentIndex(1);
    player->setPlaylist(playlist);
    player->play();
}
