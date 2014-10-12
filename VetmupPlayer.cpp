#include "VetmupPlayer.h"
#include <QFile>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QMediaPlaylist>
#include <QDir>

/******************CONSTRUCTORS - DESTRUCTOR****************/
VetmupPlayer::VetmupPlayer(QObject *parent):
    QObject(parent),m_player(new QMediaPlayer(this)),m_mediaPlaylist(new QMediaPlaylist(this)),m_playerPosition (0)
{
    connect(m_mediaPlaylist,SIGNAL(mediaInserted(int, int)),this,SLOT(mediaInsertedSlot(int,int)));
    connect(m_mediaPlaylist,SIGNAL(mediaAboutToBeRemoved(int, int)),this,SLOT(mediaAboutToBeRemovedSlot(int,int)));
    connect(m_mediaPlaylist,SIGNAL(currentIndexChanged(int)),this,SLOT(currentIndexChangedSlot(int)));
    connect(m_player,SIGNAL(positionChanged(qint64)),this,SLOT(positionChangedSlot(qint64)));
    connect(m_player,SIGNAL(durationChanged(qint64)),this,SLOT(durationChangedSlot(qint64)));
    m_player->setVolume(100);
}

VetmupPlayer::VetmupPlayer(const VetmupPlayer &other)
    :QObject(other.parent()),m_playerPosition (other.m_playerPosition)
{
    this->m_player = new QMediaPlayer(other.m_player);
}

VetmupPlayer::~VetmupPlayer(){
    delete m_player;
    delete m_mediaPlaylist;
}

/************************************************************/


/***************Public methods (called from QML)**************/
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
    QStringList nameFilter("*");
    QString localFile = url.toLocalFile();

    QList<QUrl> audioFiles = OpenFolderRecursively(localFile);
    OpenFiles(audioFiles);
}

void VetmupPlayer::PlayPause()
{
    QMediaPlayer::State state = m_player->state();
    switch(state){
       case QMediaPlayer::PlayingState:
            m_playerPosition = m_player->position();
            m_player->pause();
            break;
        case QMediaPlayer::PausedState:
            m_player->play();
            m_player->setPosition(m_playerPosition);
            break;
        case QMediaPlayer::StoppedState:
            m_player->play();
            m_player->setPosition(m_playerPosition);
            break;
    }
}

void VetmupPlayer::DeletePlaylist()
{
    if(HasSongs())
    {
        m_player->stop();
        m_mediaPlaylist->clear();
    }
}

void VetmupPlayer::NextSong()
{
    if(HasSongs())
    {
        m_player->stop();
        m_mediaPlaylist->next();
        m_player->play();
    }
}

void VetmupPlayer::PreviousSong()
{
    if(HasSongs())
    {
        m_player->stop();
        m_mediaPlaylist->previous();
        m_player->play();
    }

}

bool VetmupPlayer::HasSongs(){
    if(m_mediaPlaylist->mediaCount()>0)
        return true;
    else
        return false;
}

void VetmupPlayer::SetSongTime(int miliseconds)
{
    m_playerPosition = miliseconds;
    if(m_player->state() == QMediaPlayer::PlayingState)
    {
        m_player->setPosition(m_playerPosition);
    }
}

void VetmupPlayer::SetVolume(double volume)
{
    m_player->setVolume(volume);
}

/**********************************************************/

/*********************Private methods *********************/
 QList<QUrl> VetmupPlayer::OpenFolderRecursively(QString folderPath)
 {
     QList<QUrl> listToReturn=  QList<QUrl>();
     QStringList nameFilter("*");
     QDir directory(folderPath);
     QFileInfoList audioFiles = directory.entryInfoList(nameFilter, QDir::AllEntries | QDir::NoDotAndDotDot |QDir::NoSymLinks);

     foreach(QFileInfo audioFile,audioFiles){
         if(audioFile.isFile() && audioFile.suffix()=="mp3"){
             QString path = audioFile.absoluteFilePath();
             //If we are in android, add file:// to the path
             //It is necessary to the QMediaContent :/
             #ifdef __ANDROID_API__
                 path = path.prepend("file://");
             #endif
            QUrl url = QUrl(path);
            listToReturn.append(url);
         }
         else if(audioFile.isDir()){
            listToReturn.append(OpenFolderRecursively(audioFile.absoluteFilePath()));
         }
     }

     return listToReturn;

 }

 QList<QMediaContent> VetmupPlayer::GetContent(QList<QUrl>listOfSongs){
     QList<QMediaContent> listOfMedia = QList<QMediaContent>();
     foreach(QUrl path,listOfSongs)
     {
         listOfMedia.append(QMediaContent(path));
     }
     return listOfMedia;
 }

 /**********************************************************/


 /*********************Private Slots *********************/
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
        qDebug()<<"Deleted: "<<m_mediaPlaylist->media(i).canonicalUrl();
}

void VetmupPlayer::currentIndexChangedSlot(int index)
{

}

void VetmupPlayer::positionChangedSlot(qint64 position){
    qDebug()<<"void VetmupPlayer::positionChangedSlot(qint64 position)"<<position;
    emit sliderPositionChangedSignal(position);
}

void VetmupPlayer::durationChangedSlot(qint64 duration)
{
    if(duration >0){
        QFileInfo fileInformation = QFileInfo(m_mediaPlaylist->currentMedia().canonicalUrl().toString());
        VetmupSong song =  VetmupSong(fileInformation.fileName(),duration);
        qDebug()<<"Playing: "<< song.GetTitle() << "\t Duration: "<< QString::number(song.GetDuration());
        emit songChangedSignal(song.GetTitle(),song.GetDuration());
    }
}

/**********************************************************/




