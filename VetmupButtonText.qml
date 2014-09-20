/*Carlos Martinez Perez - mpcarlos87@gmail.com - 20/09/2014*/
/*This class is for the buttons with only text*/

import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Button {
    id: simpleButtonText
    width: 48
    height: 48
    style: ButtonStyle {
                   background: Rectangle {
                       implicitWidth: 100
                       implicitHeight: 25
                       border.width: control.activeFocus ? 2 : 1
                       border.color: "black"
                       radius: 4
                       gradient: Gradient {
                           GradientStop { position: 0 ; color: control.pressed ? "#ddd" : "#eee" }
                           GradientStop { position: 1 ; color: control.pressed ? "#bbb" : "#fff" }
                       }
                   }
               }
}
