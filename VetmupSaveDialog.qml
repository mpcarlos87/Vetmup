import QtQuick 2.0
import QtQuick.Dialogs 1.2

FileDialog {
    title: qsTr("Please type the name of the file")
    selectMultiple: false;
    selectExisting: false;
    nameFilters: [ qsTr("Audio Playlist (*.m3u)") ];
}
