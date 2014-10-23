import QtQuick 2.3
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import VetmupPlayerQML 1.0
import vetmup.style 1.0

Window {
    visible: true
    width: 1280
    height: 720

    //Player
    VetmupPlayer{
        id: myVetmupPlayer
        onSongChangedSignal: {
            timeSlider.value = 0;
            timeSlider.minimumValue = 0;
            timeSlider.maximumValue = duration;
            mainText.text= title;
            listViewSongs.currentIndex = index;
        }
        onSliderPositionChangedSignal:{
            if(!timeSlider.pressed)
                timeSlider.value = position;
        }
        onSongAddedSignal:{
            listModelSongs.append({"title": title})
        }
        onDeletePlaylistSignal:{
            listModelSongs.remove(0,listModelSongs.count);
        }
        onDeleteSongSignal:{
            listModelSongs.remove(index);
        }
    }

    //Top Bar
    Rectangle{
        id: topBar
        color:"orange"
        height: parent.height/6;
        width: parent.width;

        //Open files button
        VetmupButton{
            ///Definition and styles
            id: buttonOpenFiles
            anchors.left: parent.left;
            style: ButtonStyle {
                    background: Rectangle {
                        radius: 4
                        color:{if(buttonOpenFiles.pressed)VetmupStyle.colorButtonsPressed;
                        else color:VetmupStyle.colorButtons;}
                        Image {
                            id: buttonOpenFilesImage
                            source: VetmupStyle.iconAdd;
                            anchors.fill: parent;
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
                    radius: 4
                    color:{if(buttonOpenFolder.pressed)VetmupStyle.colorButtonsPressed;
                    else color:VetmupStyle.colorButtons;}
                    Image {
                        id: buttonOpenFolderImage
                        source: VetmupStyle.iconFolder;
                        anchors.fill: parent;
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
                    radius: 4
                    color:{if(buttonDeletePlaylist.pressed)VetmupStyle.colorButtonsPressed;
                    else color:VetmupStyle.colorButtons;}
                    Image {
                        id: buttonDeletePlaylistImage
                        source: VetmupStyle.iconThrash;
                        anchors.fill: parent;
                    }
                }
            }
            ///Functions
            onClicked:{myVetmupPlayer.DeletePlaylist(); }
        }

        //Button Settings
        VetmupButton{
            ///Definition and styles
            id: buttonSettings
            anchors.right: parent.right;
            style: ButtonStyle {
                background: Rectangle {
                    radius: 4
                    color:{if(buttonSettings.pressed)VetmupStyle.colorButtonsPressed;
                    else color:VetmupStyle.colorButtons;}
                    Image {
                        id: buttonSettingsImage
                        source: VetmupStyle.iconSettings;
                        anchors.fill: parent;
                    }
                }
            }
            ///Functions
            onClicked:{}
        }
    }

    //Main Window
    RowLayout {
            id: mainWindow
            anchors.centerIn: parent
            width: parent.width
            height: parent.height*4/6
            spacing: 5

            //Region to previous song
            Rectangle{
                    id: previousSongWindow
                    Layout.fillWidth: true;
                    Layout.minimumWidth: parent.width/4;
                    Layout.minimumHeight: parent.height;
                    color: "orange";
                    MouseArea{
                        anchors.fill: parent
                        onClicked:{ myVetmupPlayer.PreviousSong();}
                        onPressed: {
                            parent.color= "#DF7401";
                        }
                        onReleased: {
                            parent.color= "orange";
                        }
                    }
            }

            //Region to play/pause
            Rectangle{
                    id: playPauseWindow
                    Layout.fillWidth: true;
                    Layout.minimumWidth: parent.width/2;
                    Layout.minimumHeight: parent.height;
                    color:"orange"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            if(myVetmupPlayer.HasSongs())
                                       myVetmupPlayer.PlayPause();
                                    else
                                       fileDialog.open();
                                    }
                        onPressed: {
                            parent.color= "#DF7401";
                        }
                        onReleased: {
                            parent.color= "orange";
                        }
                    }
            }

            //Region to next song
            Rectangle{
                   id: nextSongWindow
                   Layout.fillWidth: true
                   Layout.minimumWidth: parent.width/4;
                   Layout.minimumHeight: parent.height;
                   color:"orange"
                   MouseArea{
                       anchors.fill: parent
                       onClicked:{ myVetmupPlayer.NextSong();}

                       onPressed: {
                           parent.color= "#DF7401";
                       }
                       onReleased: {
                           parent.color= "orange";
                       }
                   }
            }
    }

    //Main Text
    Text {
        id: mainText
        width: playPauseWindow.width
        height: playPauseWindow.height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 18
        wrapMode: Text.Wrap
        text: qsTr("This is Vetmup!Touch to Open or Play/Pause")
        anchors.centerIn: parent
    }

    //File Dialog
    VetmupFileDialog{
        id:fileDialog
        onAccepted: {
            myVetmupPlayer.OpenFiles(fileDialog.fileUrls)
        }
    }

    //Folder Dialog
    VetmupFolderDialog{
        id:folderDialog
        onAccepted: {
            myVetmupPlayer.OpenFolder(folderDialog.fileUrl)
        }
    }


    //Volume slider
    Slider {
        id: volumeSlider
        width: parent.width/5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height/4
        maximumValue: 100.0;
        minimumValue: 0.0;
        value:99.9
        updateValueWhileDragging: true;
        orientation: Qt.Horizontal;
        onValueChanged:
        {
            myVetmupPlayer.SetVolume(volumeSlider.value);
        }
    }
    //Time slider
    Slider {
        id: timeSlider
        width: parent.width/3
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height/4
        value:0
        updateValueWhileDragging: false;

        onPressedChanged:{
                 if (!pressed){
                     myVetmupPlayer.SetSongTime(timeSlider.value);
                 }
        }
    }

    Rectangle{
        id: listSongsRectangle
        anchors.bottom : parent.bottom;
        height: parent.height/6;
        width: parent.width;
        color: "#FFA500"

        Component {
            id: songComponent
            Item {
                width: listSongsRectangle.width/6;
                height: listSongsRectangle.height
                Label {
                    width: parent.width
                    height: parent.height
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: title
                    wrapMode: Text.Wrap
                    font.pixelSize: 10
                    font.bold: true;
                }
                MouseArea{
                    anchors.fill: parent;
                    onClicked: {
                        myVetmupPlayer.PlaySong(index);
                    }
                    onPressAndHold:
                    {
                        console.log("ONPRESSED");
                        myVetmupPlayer.DeleteSong(index);
                    }
                }
            }
        }

        ListView {
            id: listViewSongs
            orientation: Qt.Horizontal
            anchors.fill: parent
            model: listModelSongs
            delegate: songComponent
            highlight: Rectangle { color: "#E59400"; radius: 5 }
            highlightMoveDuration: 100
        }

        ListModel {
            id: listModelSongs
        }
    }
}
