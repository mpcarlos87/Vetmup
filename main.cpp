#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>
#include <QQmlApplicationEngine>

#include "VetmupPlayer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<VetmupPlayer>("VetmupPlayerQML", 1, 0, "VetmupPlayer");
    qRegisterMetaType<VetmupPlayer>("VetmupPlayer");

    qmlRegisterSingletonType( QUrl("qrc:///Style.qml"), "vetmup.style", 1, 0, "VetmupStyle" );

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    return app.exec();
}
