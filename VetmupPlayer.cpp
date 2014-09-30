#include "VetmupPlayer.h"
#include <QFile>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QMediaPlaylist>
#include <QDir>


VetmupPlayer::VetmupPlayer(QObject *parent):
    QObject(parent),m_player(new QMediaPlayer(this)),m_mediaPlaylist(new QMediaPlaylist(this))
{
    connect(m_mediaPlaylist,SIGNAL(mediaInserted(int, int)),this,SLOT(mediaInsertedSlot(int,int)));
    connect(m_mediaPlaylist,SIGNAL(mediaAboutToBeRemoved(int, int)),this,SLOT(mediaAboutToBeRemovedSlot(int,int)));
    connect(m_mediaPlaylist,SIGNAL(currentIndexChanged(int)),this,SLOT(currentIndexChangedSlot(int)));
}

VetmupPlayer::~VetmupPlayer(){
    delete m_player;
    delete m_mediaPlaylist;
}

VetmupPlayer::VetmupPlayer(const VetmupPlayer &other)
    :QObject(other.parent())
{
    this->m_player = new QMediaPlayer(other.m_player);

}

void VetmupPlayer::OpenFiles(QList<QUrl> urls)
{
    bool isNewList =false;
    if(!this->HasSongs())
        isNewList = true;

    QList<QMediaContent> listOfContent = GetContent(urls);
    m_mediaPlaylist->addMedia(listOfContent);

    if(isNewList){
        m_player->setPlaylist(m_mediaPlaylist);
        m_player->play();
    }
}

void VetmupPlayer::OpenFolder(QUrl url)
{
    bool isNewList =false;
    if(!this->HasSongs())
        isNewList = true;

    QStringList nameFilter("*");
    QString localFile = url.toLocalFile();

    QList<QUrl> audioFiles = OpenFolderRecursively(localFile);
    OpenFiles(audioFiles);
}

 QList<QUrl> VetmupPlayer::OpenFolderRecursively(QString folderPath)
 {
     QList<QUrl> listToReturn=  QList<QUrl>();
     QStringList nameFilter("*");
     QDir directory(folderPath);
     QFileInfoList audioFiles = directory.entryInfoList(nameFilter, QDir::AllEntries | QDir::NoDotAndDotDot |QDir::NoSymLinks);

     foreach(QFileInfo audioFile,audioFiles){
         if(audioFile.isFile() && audioFile.suffix()=="mp3"){
            QUrl url = QUrl(audioFile.absoluteFilePath());
            listToReturn.append(url);
         }
         else if(audioFile.isDir()){
            listToReturn.append(OpenFolderRecursively(audioFile.absoluteFilePath()));
         }
     }

     return listToReturn;

 }

 void VetmupPlayer::PlayPause()
 {
     QMediaPlayer::State state = m_player->state();
     switch(state){
        case QMediaPlayer::PlayingState:
             m_player->pause();
             break;
     case QMediaPlayer::PausedState:
          m_player->play();
          break;
     }
 }

 bool VetmupPlayer::HasSongs(){
     if(m_mediaPlaylist->mediaCount()>0)
         return true;
     else
         return false;
 }

void VetmupPlayer::mediaInsertedSlot(int,int)
{
    qDebug()<<"List of songs";
    for(int i = 0; i < m_mediaPlaylist->mediaCount();i++)
    {
        qDebug()<<m_mediaPlaylist->media(i).canonicalUrl();

    }
}

void VetmupPlayer::mediaAboutToBeRemovedSlot(int start, int end)
{
    for(int i = start; i <= end;i++)
    {
        qDebug()<<"Deleted: "<<m_mediaPlaylist->media(i).canonicalUrl();

    }
}

void VetmupPlayer::currentIndexChangedSlot(int index)
{
    qDebug()<<"Playing: "<<m_mediaPlaylist->media(index).canonicalUrl();
}

QList<QMediaContent> VetmupPlayer::GetContent(QList<QUrl>listOfSongs){
    QList<QMediaContent> listOfMedia = QList<QMediaContent>();
    foreach(QUrl path,listOfSongs)
    {
        listOfMedia.append(QMediaContent(path));
    }
    return listOfMedia;
}

void VetmupPlayer::DeletePlaylist()
{
    if(HasSongs()){
        m_player->stop();
        m_mediaPlaylist->clear();
    }
}
