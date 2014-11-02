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
            tabPlayer.timeSliderValue = 0;
            tabPlayer.timeSliderMinimumValue = 0;
            tabPlayer.timeSliderMaximumValue = duration;
            tabPlayer.mainTextString= title;
            tabPlaylist.listSongsCurrentIndex = index;
            tabPlayer.textTimeSliderTotal = durationText;
        }
        onSliderPositionChangedSignal:{
            if(!tabPlayer.children[0].children[1].children[2].pressed){
                tabPlayer.timeSliderValue = position;
                tabPlayer.textTimeSliderCurrent = positionText;
            }
        }
        onSongAddedSignal:{
            tabPlaylist.listModelSongs.append({"title": title})
        }
        onDeletePlaylistSignal:{
            tabPlayer.mainTextString = VetmupStyle.textDefaultPlayer
            tabPlaylist.listModelSongs.remove(0,tabPlaylist.listModelSongs.count);
            tabPlayer.textTimeSliderCurrent = VetmupStyle.textDefaultTimeSlider;
            tabPlayer.textTimeSliderTotal = VetmupStyle.textDefaultTimeSlider;
        }
        onDeleteSongSignal:{
            if(!myVetmupPlayer.HasSongs()){
                tabPlayer.mainTextString = VetmupStyle.textDefaultPlayer
            }
            listModelSongs.remove(index);
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


    //User Interface
    TabView {
        id: tabView
        anchors.fill: parent

        style: TabViewStyle {
                frameOverlap: 1
                tab: Rectangle {
                    color: styleData.selected ? VetmupStyle.colorHighlight :VetmupStyle.colorNormal
                    implicitWidth: tabView.width/2
                    implicitHeight: tabView.height/15
                    radius: 2
                    Text {
                        id: text
                        anchors.centerIn: parent
                        text: styleData.title
                        font.family: mainFont.name
                        font.pixelSize: tabView.height/20
                        color: styleData.selected ? VetmupStyle.colorTextSelected :VetmupStyle.colorText
                    }
                }
            }

        Tab {
            id:tabPlayer
            title: qsTr("Player")
            source: "Player.qml"
            property int timeSliderValue:0
            property int timeSliderMinimumValue:0
            property int timeSliderMaximumValue:100
            property string mainTextString: VetmupStyle.textDefaultPlayer
            property string textTimeSliderTotal:VetmupStyle.textDefaultTimeSlider
            property string textTimeSliderCurrent:VetmupStyle.textDefaultTimeSlider
        }

        Tab {
            id: tabPlaylist
            title: "Playlist"
            source: "Playlist.qml"
            property int listSongsCurrentIndex
            property ListModel listModelSongs: ListModel {}
        }
    }
}
