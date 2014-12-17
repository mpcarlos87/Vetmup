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
#include <QImageWriter>

/******************CONSTRUCTORS - DESTRUCTOR****************/
VetmupPlayer::VetmupPlayer(QObject *parent):
    QObject(parent),m_player(new QMediaPlayer(this)),m_mediaPlaylist(new QList<QMediaContent>()),
    m_playerPosition (0),m_thumbnailPath(QString("")),m_thumbnail(QImage()),m_random(false)
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    connect(m_player,SIGNAL(positionChanged(qint64)),this,SLOT(positionChangedSlot(qint64)));
    connect(m_player,SIGNAL(durationChanged(qint64)),this,SLOT(durationChangedSlot(qint64)));
    connect(m_player,SIGNAL(metaDataChanged()),SLOT(metaDataChangedSlot()));
    connect(m_player,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(mediaStatusChangedSlot(QMediaPlayer::MediaStatus)));
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
    m_mediaPlaylist->append(listOfContent);

    foreach(QMediaContent mediaContent,listOfContent){
        QFileInfo fileInformation = QFileInfo(mediaContent.canonicalUrl().toString());
        VetmupSong song =  VetmupSong(fileInformation.completeBaseName(),0);
        emit songAddedSignal(song.GetTitle());
    }

    if(isNewList){
        m_index = 0;
        QMediaContent firstContent = m_mediaPlaylist->at(m_index);
        m_player->setMedia(firstContent);
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
        m_index = 0;
        emit deletePlaylistSignal();
    }
}

void VetmupPlayer::NextSong()
{
    if(HasSongs())
    {
        QMediaPlayer::State actualState = m_player->state();
        m_player->stop();
        m_player->setMedia(m_mediaPlaylist->at(GetNextIndex()));
        RestorePreviousState(actualState);
    }
}

void VetmupPlayer::PreviousSong()
{
    if(HasSongs())
    {
        QMediaPlayer::State actualState = m_player->state();
        m_player->stop();
        m_player->setMedia(m_mediaPlaylist->at(GetPreviousIndex()));
        RestorePreviousState(actualState);

    }
}


void VetmupPlayer::PlaySong(int index)
{
    if(HasSongs())
    {
        m_player->stop();
        m_index = index;
        m_player->setMedia(m_mediaPlaylist->at(m_index));
        m_player->play();
    }
}

void VetmupPlayer::DeleteSong(int index)
{
    //Only deletes the songs different at the current played/paused
    if(m_index != index)
    {
        m_mediaPlaylist->removeAt(index);
        //Emit delete song to update the UI in QML
        emit deleteSongSignal(index);

        if(index<m_index)
            m_index--;

    }
}

bool VetmupPlayer::HasSongs(){
    if(m_mediaPlaylist->count()>0)
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
        QMediaPlaylist *mediaplaylist = new QMediaPlaylist(this);
        for(int i= 0; i < m_mediaPlaylist->count(); i++) {
            mediaplaylist->addMedia(m_mediaPlaylist->at(i));
        }

        if(mediaplaylist->save(file,"m3u"))
            qDebug()<<"File: "<<file<< " saved succesfully";
        else
            qDebug()<<"Error Saving File: "<<file;

    }
}

void VetmupPlayer::ShufflePlaylist(bool enable)
{
    if(enable)
        m_random = true;
    else
        m_random = false;
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

         if(audioFile.isFile() && (audioFile.suffix()=="mp3" || audioFile.suffix() =="wma" || audioFile.suffix() == "m4a" || audioFile.suffix() == "m3u")){
             QString path = audioFile.absoluteFilePath();
             //If we are in android, add file:// to the path
             //It is necessary to the QMediaContent :/
             //#ifdef __ANDROID_API__
                 path = path.prepend("file:///");
                 qDebug()<<"Path Open Folder: "<<path;
             //#endif
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
         QFileInfo fileInfo(path.toString());
         //Open Playlist m3u
         if(fileInfo.suffix() == "m3u")
         {
             qDebug()<<"List path: "<<path;
             QList<QMediaContent> playList = OpenPlayList(path.toString());
             listOfMedia.append(playList);
         }
         else
         {
             qDebug()<<"Path:"<<path;
             listOfMedia.append(QMediaContent(path));
         }

     }
     return listOfMedia;
 }

 QList<QMediaContent> VetmupPlayer::OpenPlayList(QUrl path)
 {
     //qDebug()<<"List path: "<<path;
     QString pathToLoad = path.toString();
     if(!pathToLoad.contains("file:///"))
         pathToLoad= pathToLoad.prepend("file:///");

     QList<QMediaContent> list = QList<QMediaContent>();
     QMediaPlaylist playList(this);
     playList.load(pathToLoad,"m3u");
     for(int i = 0 ; i < playList.mediaCount(); i++)
        list.append(playList.media(i));
    return list;
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

 void VetmupPlayer::RestorePreviousState(QMediaPlayer::State previousState)
 {
     switch(previousState){
         case QMediaPlayer::State::PlayingState:
             m_player->play();
             break;
         case QMediaPlayer::State::PausedState:
             m_player->pause();
             break;
     }
 }

 int VetmupPlayer::RandInt(int low, int high)
{
     // Random number between low and high
     return qrand() % ((high + 1) - low) + low;
}


int VetmupPlayer::GetNextIndex(){
 if(m_random)
 {
     m_index = RandInt(0,m_mediaPlaylist->count()-1);
 }
 else
 {
     if(m_index == m_mediaPlaylist->count()-1)
         m_index = 0;
     else
         m_index++;

 }
 return m_index;
}

int VetmupPlayer::GetPreviousIndex(){
 if(m_random)
 {
     m_index = RandInt(0,m_mediaPlaylist->count()-1);

 }
 else
 {
     if(m_index == 0)
         m_index = m_mediaPlaylist->count()-1;
     else
         m_index--;

 }
 return m_index;
}
 /**********************************************************/


 /*********************Private Slots *********************/

void VetmupPlayer::positionChangedSlot(qint64 position){
    QString timeString = GetTimeString(position);
    //Notify the UI to set the slider position
    emit sliderPositionChangedSignal(position,timeString);
}

void VetmupPlayer::durationChangedSlot(qint64 duration)
{
    if(duration >0){
        QFileInfo fileInformation = QFileInfo(m_mediaPlaylist->at(m_index).canonicalUrl().toString());
        VetmupSong song =  VetmupSong(fileInformation.completeBaseName(),duration);
        QString durationString = GetTimeString(duration);
        emit songChangedSignal(song.GetTitle(),song.GetDuration(),durationString,m_index);
    }
}



void VetmupPlayer::metaDataChangedSlot()
{
    if(m_player->isMetaDataAvailable()){

        QVariant imageVariant = m_player->metaData(QMediaMetaData::ThumbnailImage);

        if(imageVariant.type() == QVariant::Type::Image){
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


void VetmupPlayer::mediaStatusChangedSlot(QMediaPlayer::MediaStatus status)
{
    switch(status){
        case QMediaPlayer::MediaStatus::EndOfMedia:
            this->NextSong();
            break;
    }
}

/**********************************************************/



