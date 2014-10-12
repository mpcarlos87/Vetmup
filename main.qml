import QtQuick 2.3
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import VetmupPlayerQML 1.0
import vetmup.style 1.0

Window {
    visible: true
    width: 1280
    height: 720

    VetmupPlayer{
        id: myVetmupPlayer
        onSongChangedSignal: {
            timeSlider.value= 0;
            timeSlider.minimumValue = 0;
            timeSlider.maximumValue = duration;
            mainText.text= title;
        }
        onSliderPositionChangedSignal:{
            if(!timeSlider.pressed)
                timeSlider.value = position;
        }
    }

    Item{
        id: mainWindow
        anchors.fill: parent;
        //Region to play/pause
        Item{
            id: playPauseWindow
            width: parent.width/2
            height: parent.height
            anchors.left: previousSongWindow.left
            anchors.right: nextSongWindow.right
            Rectangle{
                anchors.fill: parent
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {if(myVetmupPlayer.HasSongs())
                               myVetmupPlayer.PlayPause();
                            else
                               fileDialog.open();
                            }
            }
        }
        //Region to previous song
        Item {
            id: previousSongWindow
            width: parent.width/4
            height: parent.height
            anchors.left: parent.left
            Rectangle{
                anchors.fill: parent
                color:"gray"
            }

            MouseArea{
                anchors.fill: parent
                onClicked:{ myVetmupPlayer.PreviousSong();}
            }
        }
         //Region to next song
        Item {
            id: nextSongWindow
            width: parent.width/4
            height: parent.height
            anchors.right: parent.right
            Rectangle{
                anchors.fill: parent
                color:"gray"
            }

            MouseArea{
                anchors.fill: parent
                onClicked:{ myVetmupPlayer.NextSong();}
            }
        }
    }


    Text {
        id: mainText
        font.pointSize: 20
        text: qsTr("This is Vetmup!. Touch to Open or Play/Pause")
        anchors.centerIn: parent
    }

    VetmupFileDialog{
        id:fileDialog
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
            myVetmupPlayer.OpenFiles(fileDialog.fileUrls)
        }
        onRejected: {
            console.log("Canceled")
        }
    }

    VetmupFolderDialog{
        id:folderDialog
        onAccepted: {
            console.log("You chose the folder: " + folderDialog.fileUrl)
            myVetmupPlayer.OpenFolder(folderDialog.fileUrl)
        }
        onRejected: {
            console.log("Canceled")
        }

    }

    //Open files button
    VetmupButton{
        ///Definition and styles
        id: buttonOpenFiles
        anchors.left: mainWindow.left;
        style: ButtonStyle {
                background: Rectangle {
                    border.width: control.activeFocus ? 2 : 1
                    radius: 4
                    Image {
                        id: buttonOpenFilesImage
                        source: {if(!control.pressed) VetmupStyle.iconAdd;
                        else VetmupStyle.iconAdd;}
                        anchors.fill: parent
                    }
                }
            }
        ///Functions
        onClicked:{fileDialog.open(); }
    }

    //Open folder button
    VetmupButton{
        ///Definition and styles
        id: buttonOpenFolder
        anchors.left: buttonOpenFiles.right;
        style: ButtonStyle {
                background: Rectangle {
                    border.width: control.activeFocus ? 2 : 1
                    radius: 4
                    Image {
                        id: buttonOpenFolderImage
                        source: {if(!control.pressed) VetmupStyle.iconFolder;
                        else VetmupStyle.iconFolder;}
                        anchors.fill: parent
                    }
                }
            }
        ///Functions
        onClicked:{folderDialog.open(); }
    }

    //Delete Playlist button
    VetmupButton{
        ///Definition and styles
        id: buttonDeletePlaylist
        anchors.left: buttonOpenFolder.right;
        style: ButtonStyle {
                background: Rectangle {
                    border.width: control.activeFocus ? 2 : 1
                    radius: 4
                    Image {
                        id: buttonDeletePlaylistImage
                        source: {if(!control.pressed) VetmupStyle.iconThrash;
                        else VetmupStyle.iconThrash;}
                        anchors.fill: parent
                    }
                }
            }
        ///Functions
        onClicked:{myVetmupPlayer.DeletePlaylist(); }
    }

    //Button Exit
    VetmupButton{
        ///Definition and styles
        id: buttonExit
        anchors.right: parent.right;
        style: ButtonStyle {
                background: Rectangle {
                    border.width: control.activeFocus ? 2 : 1
                    radius: 4
                    Image {
                        id: buttonExitImage
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

    Slider {
        id: timeSlider
        width: parent.width
        anchors.bottom: parent.bottom
        value:0
        updateValueWhileDragging: false;

        onPressedChanged:{
            console.debug("Pressed changed");
                 if (!pressed){
                     myVetmupPlayer.SetSongTime(timeSlider.value);
                 }
        }
    }

    Slider {
        id: volumeSlider
        height: parent.height/2
        anchors.right: parent.right
        maximumValue: 100.0;
        minimumValue: 0.0;
        value:99.9
        updateValueWhileDragging: false;
        orientation: Qt.Vertical;

        onPressedChanged:{
                 if (!pressed){
                     myVetmupPlayer.SetVolume(volumeSlider.value);
                 }
        }
    }
}
