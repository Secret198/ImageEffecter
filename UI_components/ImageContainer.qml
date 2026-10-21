import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

Item{
    id: root
    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.leftMargin: 10
    Layout.topMargin: 10
    Layout.bottomMargin: 10
    Layout.columnSpan: 3
    Layout.row: 2

    Rectangle{
        anchors.fill: parent
        radius: 4

        color: "#1C2541"
        Text{
            text: "No image selected"
            color: "White"
            anchors{
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
            }
        }
    }
}