#include "VetmupPlayer.h"
#include <QFile>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QMediaPlaylist>
#include <QDir>


VetmupPlayer::VetmupPlayer(QObject *parent):
    QObject(parent),m_player(new QMediaPlayer(this)),m_mediaPlaylist(new QMediaPlaylist(this))
{

}

VetmupPlayer::~VetmupPlayer(){
    delete m_player;
    delete m_mediaPlaylist;
}

VetmupPlayer::VetmupPlayer(const VetmupPlayer &other){
    this->m_player = new QMediaPlayer(other.m_player);
}

void VetmupPlayer::openFiles(QList<QUrl> urls)
{
    foreach(QUrl songPath,urls){
        qDebug()<<"Play "<<songPath;
        m_mediaPlaylist->addMedia(songPath);
    }
    m_mediaPlaylist->setCurrentIndex(1);
    m_player->setPlaylist(m_mediaPlaylist);
    m_player->play();
}


void VetmupPlayer::openFolder(QUrl url)
{
    QStringList nameFilter("*");
    QString localFile = url.toLocalFile();

    QStringList audioFiles = OpenFolderRecursively(localFile);
    foreach(QString audioFile,audioFiles){
        qDebug()<< audioFile;
        m_mediaPlaylist->addMedia(QUrl(audioFile));
    }
    m_mediaPlaylist->setCurrentIndex(1);
    m_player->setPlaylist(m_mediaPlaylist);
    m_player->play();
}

 QStringList VetmupPlayer::OpenFolderRecursively(QString folderPath)
 {
     QStringList listToReturn= QStringList();;
     QStringList nameFilter("*");
     QDir directory(folderPath);
     QFileInfoList audioFiles = directory.entryInfoList(nameFilter, QDir::AllEntries | QDir::NoDotAndDotDot |QDir::NoSymLinks);

     foreach(QFileInfo audioFile,audioFiles){
         if(audioFile.isFile() && audioFile.suffix()=="mp3")
             listToReturn.append(audioFile.absoluteFilePath());
         else if(audioFile.isDir()){
            listToReturn.append(OpenFolderRecursively(audioFile.absoluteFilePath()));
         }
     }

     return listToReturn;

 }
