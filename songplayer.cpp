#include "songplayer.h"
#include <QFile>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioDeviceInfo>


SongPlayer::SongPlayer(QObject *parent):
    QObject(parent)
{
      qDebug()<<"SongPlayer::SongPlayer(QObject *parent)";
      QAudioFormat format;
      // Set up the format, eg.
      format.setSampleRate(8000);
      format.setChannelCount(1);
      format.setSampleSize(8);
      format.setCodec("audio/pcm");
      format.setByteOrder(QAudioFormat::LittleEndian);
      format.setSampleType(QAudioFormat::UnSignedInt);

      QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
      if (!info.isFormatSupported(format)) {
          qWarning() << "Raw audio format not supported by backend, cannot play audio.";
          return;
      }

      m_audio = new QAudioOutput(format, this);
       connect(m_audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));
}

SongPlayer::SongPlayer(const SongPlayer &other){
}

void SongPlayer::buttonClickedQML(QString path){
    qDebug()<<"Play "<<path;


    sourceFile.setFileName(path);
    if(sourceFile.open(QIODevice::ReadOnly))
        m_audio->start(&sourceFile);
}

void SongPlayer::handleStateChanged(QAudio::State newState)
{
    switch (newState) {
        case QAudio::IdleState:
            // Finished playing (no more data)
            m_audio->stop();
            sourceFile.close();
            delete m_audio;
            break;

        case QAudio::StoppedState:
            // Stopped for other reasons
            if (m_audio->error() != QAudio::NoError) {
                // Error handling
            }
            break;

        default:
            // ... other cases as appropriate
            break;
    }
}
