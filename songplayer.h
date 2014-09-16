#ifndef SONGPLAYER_H
#define SONGPLAYER_H

#include <QObject>
#include <QDebug>
#include <QQmlListProperty>
#include <QtMultimedia/QAudioOutput>

class SongPlayer : public QObject
{
    Q_OBJECT
    public:
        explicit SongPlayer(QObject *parent = 0);
        SongPlayer(const SongPlayer &other);
        Q_INVOKABLE void buttonClickedQML(QString path);

private:
        QAudioOutput * m_audio;
};

Q_DECLARE_METATYPE(SongPlayer)
#endif // SONGPLAYER_H
