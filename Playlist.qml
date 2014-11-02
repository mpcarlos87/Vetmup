import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import vetmup.style 1.0
import VetmupPlayerQML 1.0

Rectangle{
        id: listSongsRectangle
        anchors.fill : parent
        color:  VetmupStyle.colorNormal;

        Component {
            id: songComponent
            Rectangle {
                id:songComponentRectangle
                width: listSongsRectangle.width;
                height: listSongsRectangle.height/10;
                color: "transparent"
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
                    color: songComponentRectangle.ListView.isCurrentItem ? VetmupStyle.colorTextSelected : VetmupStyle.colorText;
                }
                MouseArea{
                    anchors.fill: parent;
                    onClicked: {
                        myVetmupPlayer.PlaySong(index);
                    }
                    onPressAndHold:
                    {
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
            clip: true
            currentIndex: listSongsCurrentIndex
        }
}
