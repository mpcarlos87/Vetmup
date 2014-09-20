#ifndef VETMUPPLAYER_H
#define VETMUPPLAYER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QQmlListProperty>
#include <QtMultimedia/QAudioOutput>
#include <QMediaPlayer>

class VetmupPlayer : public QObject
{
    Q_OBJECT
    public:
        explicit VetmupPlayer(QObject *parent = 0);
        VetmupPlayer(const VetmupPlayer &other);
        Q_INVOKABLE void buttonClickedQML(QList<QUrl> urls);

private:
       QMediaPlayer* player;
};

Q_DECLARE_METATYPE(VetmupPlayer)
#endif // SONGPLAYER_H
