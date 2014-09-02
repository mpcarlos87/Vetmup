import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Window {
    visible: true
    width: 360
    height: 360

    Item{
    id: mainWindow
    anchors.fill: parent;
    }

    Text {
        text: qsTr("This is Vetmup!")
        anchors.centerIn: parent
    }

    Button{
        id: contactButton
        width: 48;
        height: 48;
        anchors.left: mainWindow.left;
        anchors.leftMargin: 5;
        tooltip : qsTr("Contact Me!")
        style: ButtonStyle {
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 25
                    border.width: control.activeFocus ? 2 : 1
                    border.color: "#f00"
                    radius: 4
                    gradient: Gradient {
                        GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                        GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                    }
                    Image {
                        id: contactImage
                        source: {if(control.hovered && !control.pressed) "qrc:///Icons/Contact";
                            else if(control.pressed) "qrc:///Icons/Play";
                        else "qrc:///Icons/Pause";}
                        anchors.fill: parent
                    }
                }
            }
    }

}
