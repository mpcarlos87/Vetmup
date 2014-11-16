#include "VetmupPlayer.h"
#include <QFile>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QMediaPlaylist>
#include <QMediaMetaData>
#include <QTime>
#include <QDir>
#include <QImage>
#include <QTemporaryFile>

/******************CONSTRUCTORS - DESTRUCTOR****************/
VetmupPlayer::VetmupPlayer(QObject *parent):
    QObject(parent),m_player(new QMediaPlayer(this)),m_mediaPlaylist(new QMediaPlaylist(this)),
    m_playerPosition (0),m_thumbnailPath(QString("")),m_thumbnail(QImage())
{
    connect(m_mediaPlaylist,SIGNAL(mediaInserted(int, int)),this,SLOT(mediaInsertedSlot(int,int)));
    connect(m_mediaPlaylist,SIGNAL(mediaAboutToBeRemoved(int, int)),this,SLOT(mediaAboutToBeRemovedSlot(int,int)));
    connect(m_player,SIGNAL(positionChanged(qint64)),this,SLOT(positionChangedSlot(qint64)));
    connect(m_player,SIGNAL(durationChanged(qint64)),this,SLOT(durationChangedSlot(qint64)));
    connect(m_player,SIGNAL(metaDataAvailableChanged(bool)),SLOT(metaDataAvailableChangedSlot(bool)));
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

    foreach(QMediaContent mediaContent,listOfContent){
        QFileInfo fileInformation = QFileInfo(mediaContent.canonicalUrl().toString());
        VetmupSong song =  VetmupSong(fileInformation.baseName(),0);
        emit songAddedSignal(song.GetTitle());
    }

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
        emit deletePlaylistSignal();
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

void VetmupPlayer::PlaySong(int index)
{
    if(HasSongs())
    {
        m_player->stop();
        m_mediaPlaylist->setCurrentIndex(index);
        m_player->play();
    }
}

void VetmupPlayer::DeleteSong(int index)
{
    if(m_mediaPlaylist->currentIndex() != index)
    {
        m_mediaPlaylist->removeMedia(index);
        emit deleteSongSignal(index);
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

void VetmupPlayer::SavePlaylist(QUrl file)
{
    if(HasSongs()){
        if(m_mediaPlaylist->save(file,"m3u"))
            qDebug()<<"File: "<<file<< " saved succesfully";
        else
            qDebug()<<"Error Saving File: "<<file;

    }
}

void VetmupPlayer::ShufflePlaylist(bool enable)
{
    if(enable)
        m_mediaPlaylist->setPlaybackMode(QMediaPlaylist::Random);
    else
        m_mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
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

 QString VetmupPlayer::GetTimeString(int mseconds){
     int totalSeconds = mseconds/1000;
     int hours = totalSeconds/3600;
     int minutes = totalSeconds%3600/60;
     int seconds = totalSeconds%3600%60;
     QTime time(hours,minutes,seconds);
     if(hours>0)
        return time.toString("hh:mm:ss");
     else
         return time.toString("mm:ss");
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

void VetmupPlayer::positionChangedSlot(qint64 position){
    qDebug()<<"void VetmupPlayer::positionChangedSlot(qint64 position)"<<position;
    QString timeString = GetTimeString(position);
    emit sliderPositionChangedSignal(position,timeString);
}

void VetmupPlayer::durationChangedSlot(qint64 duration)
{
    if(duration >0){
        QFileInfo fileInformation = QFileInfo(m_mediaPlaylist->currentMedia().canonicalUrl().toString());
        VetmupSong song =  VetmupSong(fileInformation.baseName(),duration);
        qDebug()<<"Playing: "<< song.GetTitle() << "\t Duration: "<< QString::number(song.GetDuration());
        QString durationString = GetTimeString(duration);
        emit songChangedSignal(song.GetTitle(),song.GetDuration(),durationString,m_mediaPlaylist->currentIndex());
    }
}

#include <QImageWriter>
#include <QPixmap>

void VetmupPlayer::metaDataAvailableChangedSlot(bool available){

    if(available){

        //Process the meta data

        qDebug() << m_player->availableMetaData();
        qDebug() << m_player->metaData(QMediaMetaData::CoverArtImage).type();
        qDebug() << m_player->metaData(QMediaMetaData::CoverArtUrlLarge);
        qDebug() << m_player->metaData(QMediaMetaData::CoverArtUrlSmall);
        qDebug() << m_player->metaData(QMediaMetaData::Lyrics);
        qDebug() << m_player->metaData(QMediaMetaData::AlbumTitle);
        qDebug() << m_player->metaData(QMediaMetaData::AlbumArtist);
        qDebug() << m_player->metaData(QMediaMetaData::Title);
        QVariant imageVariant = m_player->metaData(QMediaMetaData::ThumbnailImage);

        if(imageVariant.type() == QMetaType::QImage){
                QImage image = imageVariant.value<QImage>();
                if(!image.isNull() && m_thumbnail != image)
                {
                    m_thumbnail = QImage(image);

                    if(QFile::exists(m_thumbnailPath))
                        QFile::remove(m_thumbnailPath);
                    QTemporaryFile thumbnailFile;
                    if(thumbnailFile.open())
                    {
                        m_thumbnailPath = thumbnailFile.fileName().append(".png");
                        QImageWriter writer(m_thumbnailPath);
                        if(writer.write(m_thumbnail))
                        {
                            emit songMetaDataChangedSignal("file:///" +m_thumbnailPath);
                        }
                    }
                    else{
                        m_thumbnail = QImage();
                        emit songMetaDataChangedSignal("");
                    }
                }
        }
        else{
            m_thumbnail = QImage();
            emit songMetaDataChangedSignal("");
        }
    }
}

/**********************************************************/



