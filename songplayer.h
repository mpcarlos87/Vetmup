#ifndef SONGPLAYER_H
#define SONGPLAYER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QQmlListProperty>
#include <QtMultimedia/QAudioOutput>
#include <QMediaPlayer>

class SongPlayer : public QObject
{
    Q_OBJECT
    public:
        explicit SongPlayer(QObject *parent = 0);
        SongPlayer(const SongPlayer &other);
        Q_INVOKABLE void buttonClickedQML(QString path);

private:
       QMediaPlayer* player;
        QFile sourceFile;
};

Q_DECLARE_METATYPE(SongPlayer)
#endif // SONGPLAYER_H
