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
                property int indexClicked: -1;
                property int x_movement;

                onXChanged: {
                        if (mouseArea.drag.active) {
                            x_movement=x;
                        }
                }

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
                    id:mouseArea
                    anchors.fill: parent;
                    drag.target: songComponentRectangle
                    drag.axis: Drag.XAxis
                    drag.minimumX: 0
                    drag.maximumX: songComponentRectangle.width

                    onClicked: {
                        myVetmupPlayer.PlaySong(index);
                    }
                    onPressed: {
                        x_movement = 0;
                        indexClicked = index;
                    }
                    onPositionChanged:
                    {
                        if(x_movement > songComponentRectangle.width/2 && indexClicked != -1)
                        {
                            console.log("Delete: "+indexClicked);
                            myVetmupPlayer.DeleteSong(indexClicked);
                            x_movement = 0;
                            indexClicked = -1;
                        }

                    }

                    onReleased:
                    {
                        if(x_movement!=0)
                            parent.x += x_movement *(-1);
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
