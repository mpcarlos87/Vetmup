import QtQuick 2.0
import QtQuick.Dialogs 1.2

FileDialog {
    title: qsTr("Please choose a file")
    selectMultiple: true;
    selectExisting:true;
    nameFilters: [ qsTr("Audio files (*.mp3 *.acc *.ogg *.m3u)"), qsTr("All files (*)") ];
}
