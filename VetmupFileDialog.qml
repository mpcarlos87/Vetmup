import QtQuick 2.0
import QtQuick.Dialogs 1.2

FileDialog {
    title: "Please choose a file"
    selectMultiple: true;
    selectExisting:true;
    nameFilters: [ "Audio files (*.mp3 *.acc *.m3u)", "All files (*)" ]
}
