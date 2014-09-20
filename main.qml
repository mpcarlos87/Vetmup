import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Dialogs 1.2
import VetmupPlayerQML 1.0
import vetmup.style 1.0

Window {
    visible: true
    width: 360
    height: 360

    VetmupPlayer{
        id: myVetmupPlayer
    }

    Item{
    id: mainWindow
    anchors.fill: parent;
    }

    Text {
        text: qsTr("This is Vetmup!")
        color: VetmupStyle.colourBlue
        anchors.centerIn: parent
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        selectMultiple: true;
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
            myVetmupPlayer.buttonClickedQML(fileDialog.fileUrls)
        }
        onRejected: {
            console.log("Canceled")
        }
    }

    VetmupButton{
        ///Definition and styles
        id: buttonOpen
        anchors.left: mainWindow.left;
        style: ButtonStyle {
                background: Rectangle {
                    border.width: control.activeFocus ? 2 : 1
                    radius: 4
                    Image {
                        id: buttonOpenImage
                        source: {if(!control.pressed) VetmupStyle.iconAdd;
                        else VetmupStyle.iconAdd;}
                        anchors.fill: parent
                    }
                }
            }
        ///Functions
        onClicked:{fileDialog.open(); }
    }

    //Button Settings
    VetmupButton{
        ///Definition and styles
        id: buttonExit
        anchors.right: parent.right;
        style: ButtonStyle {
                background: Rectangle {
                    border.width: control.activeFocus ? 2 : 1
                    radius: 4
                    Image {
                        id: buttonSettingsImage
                        source: {if(!control.pressed) VetmupStyle.iconClose;
                        else VetmupStyle.iconClose;}
                        anchors.fill: parent
                    }
                }
            }
        ///Functions
        onClicked:{Qt.quit()}
    }

    //Button Settings
    VetmupButton{
        ///Definition and styles
        id: buttonSettings
        anchors.right: buttonExit.left;
        style: ButtonStyle {
                background: Rectangle {
                    border.width: control.activeFocus ? 2 : 1
                    radius: 4
                    Image {
                        id: buttonSettingsImage
                        source: {if(!control.pressed) VetmupStyle.iconSettings;
                        else VetmupStyle.iconSettings;}
                        anchors.fill: parent
                    }
                }
            }
        ///Functions
        onClicked:{}
    }

}
