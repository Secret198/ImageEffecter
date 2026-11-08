import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

Item{
    id:root
    height: 30
    width: parent.width

    property int idx
    property string effectName

    Rectangle{
        anchors.fill: parent
        color: "#3A506B"
        radius: 4

        Text{
            anchors{
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            leftPadding: 5
            color: "#cecece"

            text: effectName
        }

        Button{
            id: deleteButton
            height: parent.height
            width: 50

            anchors{
                right: parent.right
            }

            background: Rectangle{
                color: deleteButton.pressed ? "#223040" : (deleteButton.hovered ? "#2b3b4f" : "#3A506B")
                topRightRadius: 4
                bottomRightRadius: 4
            }

            Image{
                anchors {
                    verticalCenter: parent.verticalCenter
                    horizontalCenter: parent.horizontalCenter
                }
                width: 20
                height: 20

                source: "../assets/icons/cross_icon.svg"
            }
        }
    }
}