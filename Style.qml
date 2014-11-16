/*Carlos Martinez Perez - mpcarlos87@gmail.com - 20/09/2014*/
/*This class is used to manage the styles of vetmup*/
pragma Singleton
import QtQuick 2.0

Item {
    //Icons
    readonly property string iconAdd: "qrc:///Icons/AddWhite"
    readonly property string iconAddPressed: "qrc:///Icons/AddOrange"
    readonly property string iconFolder: "qrc:///Icons/FolderWhite"
    readonly property string iconFolderPressed: "qrc:///Icons/FolderOrange"
    readonly property string iconThrash: "qrc:///Icons/ThrashWhite"
    readonly property string iconThrashPressed: "qrc:///Icons/ThrashOrange"
    readonly property string iconSettings: "qrc:///Icons/SettingsWhite"
    readonly property string iconSettingsPressed: "qrc:///Icons/SettingsOrange"
    readonly property string iconNext: "qrc:///Icons/NextWhite"
    readonly property string iconNextPressed: "qrc:///Icons/NextOrange"
    readonly property string iconPrevious: "qrc:///Icons/PreviousWhite"
    readonly property string iconPreviousPressed: "qrc:///Icons/PreviousOrange"
    readonly property string iconContact: "qrc:///Icons/Contact"
    readonly property string iconPlay: "qrc:///Icons/Play"
    readonly property string iconPause: "qrc:///Icons/Pause"
    readonly property string iconVolumeHigh: "qrc:///Icons/VolumeHighWhite"
    readonly property string iconVolumeLow: "qrc:///Icons/VolumeLowWhite"
    readonly property string iconShuffle: "qrc:///Icons/ShuffleWhite"
    readonly property string iconShufflePressed: "qrc:///Icons/ShuffleOrange"
    readonly property string iconSave: "qrc:///Icons/SaveWhite"
    readonly property string iconSavePressed: "qrc:///Icons/SaveOrange"
    readonly property string defaultSongImage: "/Images/DefaultSongImage";
    readonly property string colorButtons: "transparent"
    readonly property string colorHighlight: "#2E2E2E"
    readonly property string colorNormal: "#1C1C1C"
    readonly property string colorTextSelected: "#CC8400"
    readonly property string colorText: "#FFFFFF"
    readonly property string textDefaultPlayer:qsTr("This is Vetmup!Touch to Open or Play/Pause")
    readonly property string textDefaultTimeSlider:""
}
