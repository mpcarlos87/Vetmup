import QtQuick 2.3
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import VetmupPlayerQML 1.0
import vetmup.style 1.0

Window {
    id:main
    visible: true
    width: 1280
    height: 720

    //Load font
    FontLoader { id: mainFont; source: "Fonts/Quickhand" }

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
            mainText.text = qsTr("This is Vetmup!Touch to Open or Play/Pause");
            listModelSongs.remove(0,listModelSongs.count);
        }
        onDeleteSongSignal:{
            if(!myVetmupPlayer.HasSongs()){
                mainText.text = qsTr("This is Vetmup!Touch to Open or Play/Pause");
            }
            listModelSongs.remove(index);
        }
    }

    //Top Bar
    Rectangle{
        id: topBar
        color: VetmupStyle.colorNormal;
        height: parent.height/8;
        width: parent.width;

        //Open files button
        VetmupButton{
            ///Definition and styles
            id: buttonOpenFiles
            anchors.left: parent.left;
            style: ButtonStyle {
                    background: Rectangle {
                        radius: 4
                        color:{if(buttonOpenFiles.pressed)VetmupStyle.colorHighlight;
                        else color:VetmupStyle.colorButtons;}
                        Image {
                            id: buttonOpenFilesImage
                            source: {if(!buttonOpenFiles.pressed)VetmupStyle.iconAdd;
                                else VetmupStyle.iconAddPressed;}
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
                    color:{if(buttonOpenFolder.pressed)VetmupStyle.colorHighlight;
                    else color:VetmupStyle.colorButtons;}
                    Image {
                        id: buttonOpenFolderImage
                        source: {if(!buttonOpenFolder.pressed)VetmupStyle.iconFolder;
                            else VetmupStyle.iconFolderPressed;}
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
                    color:{if(buttonDeletePlaylist.pressed)VetmupStyle.colorHighlight;
                    else color:VetmupStyle.colorButtons;}
                    Image {
                        id: buttonDeletePlaylistImage
                        source: {if(!buttonDeletePlaylist.pressed)VetmupStyle.iconThrash;
                            else VetmupStyle.iconThrashPressed;}
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
                    color:{if(buttonSettings.pressed)VetmupStyle.colorHighlight;
                    else color:VetmupStyle.colorButtons;}
                    Image {
                        id: buttonSettingsImage
                        source: {if(!buttonSettings.pressed)VetmupStyle.iconSettings;
                            else VetmupStyle.iconSettingsPressed;}
                        anchors.fill: parent;
                    }
                }
            }
            ///Functions
            onClicked:{}
        }
    }

    //Main
    Rectangle{
        id: mainWindow
        height: parent.height*7/8
        width: parent.width*2/3
        anchors.top: topBar.bottom
        anchors.left: parent.left
        anchors.right: listSongsRectangle.left
        color:VetmupStyle.colorNormal

        //Volume slider
        Slider {
            id: volumeSlider
            width: parent.width/2
            height: parent.height/10
            anchors.top: parent.top
            maximumValue: 100.0;
            minimumValue: 0.0;
            value:99.9
            updateValueWhileDragging: true;
            orientation: Qt.Horizontal;
            anchors.horizontalCenter: parent.horizontalCenter
            onValueChanged:
            {
                myVetmupPlayer.SetVolume(volumeSlider.value);
            }
        }

        RowLayout {
                id: mainRow
                height: parent.height*8/10;
                width: parent.width
                anchors.left: parent.left
                anchors.top: volumeSlider.bottom
                anchors.right:parent.right
                anchors.bottom: timeSlider.top
                spacing: 5

                //Region to previous song
                Rectangle{
                        id: previousSongWindow
                        Layout.fillWidth: true;
                        Layout.minimumWidth: parent.width/4;
                        Layout.minimumHeight: parent.height;
                        color: VetmupStyle.colorNormal;
                        Image{
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            width: parent.width/2;
                            height: parent.width/2;
                            source:  {if(!previousSongMouse.pressed)VetmupStyle.iconPrevious;
                                else VetmupStyle.iconPreviousPressed;}
                        }

                        MouseArea{
                            id: previousSongMouse
                            anchors.fill: parent
                            onClicked:{ myVetmupPlayer.PreviousSong();}
                            onPressed: {
                                parent.color= VetmupStyle.colorHighlight;
                            }
                            onReleased: {
                                parent.color= VetmupStyle.colorNormal;
                            }
                        }
                }

                //Region to play/pause
                Rectangle{
                        id: playPauseWindow
                        Layout.fillWidth: true;
                        Layout.minimumWidth: parent.width/2;
                        Layout.minimumHeight: parent.height;
                        color:VetmupStyle.colorNormal
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if(myVetmupPlayer.HasSongs())
                                           myVetmupPlayer.PlayPause();
                                        else
                                           fileDialog.open();
                                        }
                            onPressed: {
                                parent.color= VetmupStyle.colorHighlight;
                            }
                            onReleased: {
                                parent.color= VetmupStyle.colorNormal;
                            }
                        }

                        //Main Text
                        Text {
                            id: mainText
                            width: parent.width
                            height: parent.height
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: mainFont.name;
                            font.pixelSize: parent.height/15;
                            maximumLineCount: 6
                            wrapMode: Text.Wrap
                            text: qsTr("This is Vetmup!Touch to Open or Play/Pause")
                            anchors.centerIn: parent
                            color: VetmupStyle.colorText
                        }
                }

                //Region to next song
                Rectangle{
                       id: nextSongWindow
                       Layout.fillWidth: true
                       Layout.minimumWidth: parent.width/4;
                       Layout.minimumHeight: parent.height;
                       color:VetmupStyle.colorNormal

                       Image{
                           anchors.horizontalCenter: parent.horizontalCenter
                           anchors.verticalCenter: parent.verticalCenter
                           width: parent.width/2;
                           height: parent.width/2;
                           source:  {if(!nextSongMouse.pressed)VetmupStyle.iconNext;
                               else VetmupStyle.iconNextPressed;}
                       }

                       MouseArea{
                           id: nextSongMouse
                           anchors.fill: parent
                           onClicked:{ myVetmupPlayer.NextSong();}

                           onPressed: {
                               parent.color= VetmupStyle.colorHighlight;
                           }
                           onReleased: {
                               parent.color= VetmupStyle.colorNormal;
                           }
                       }
                }
        }

        Slider {
            id: timeSlider
            width: parent.width/1.5
            height: parent.height/10
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            value:0
            updateValueWhileDragging: false;

            onPressedChanged:{
                     if (!pressed){
                         myVetmupPlayer.SetSongTime(timeSlider.value);
                     }
            }
        }
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

    //List of songs
    Rectangle{
        id: listSongsRectangle
        anchors.top : topBar.bottom
        anchors.right: parent.right
        height: parent.height*7/8;
        width: parent.width/3;
        color:  VetmupStyle.colorNormal;

        Component {
            id: songComponent
            Item {
                width: listSongsRectangle.width;
                height: listSongsRectangle.height/10;
                Label {
                    width: parent.width
                    height: parent.height
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    text: title
                    wrapMode: Text.Wrap
                    maximumLineCount : 2
                    font.family: mainFont.name;
                    font.pixelSize: Math.max(parent.height/4,18)
                    font.bold: true;
                    color: VetmupStyle.colorText;
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
            orientation: Qt.Vertical
            anchors.fill: parent
            model: listModelSongs
            delegate: songComponent
            highlight: Rectangle { color: VetmupStyle.colorHighlight; radius: 5 }
            highlightMoveDuration: 100
        }

        ListModel {
            id: listModelSongs
        }
    }
}
