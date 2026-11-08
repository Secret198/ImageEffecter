import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import ImageEffecter

ApplicationWindow {
    id: window
    width: 1280
    height: 720
    minimumWidth: 200
    minimumHeight: 250
    visible: true
    title: qsTr("Hello World")
    color: "#0B132B"



    GridLayout {
        id: row
        rows: 2
        columns: 4
        columnSpacing: 10

        anchors.fill: parent

        MenuBar{
            Layout.columnSpan: 4
            Layout.row: 0
            Menu{
                title: qsTr("&File")
                Action {
                    text: qsTr("&Load")
                }
                Action {
                    text: qsTr("&Export")
                }
            }
        }

        ComputeItem {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: 10
            Layout.topMargin: 10
            Layout.bottomMargin: 10
            Layout.columnSpan: 3
            Layout.row: 1

            source: "E:/uni/qt/imageEff/ImageEffecter/assets/test2.jpg"
            alphaBlending: true

            // mirrorVertically: true   // flip if the image appears upside down
        }
        // ImageContainer{

        //     id: imgContainer


        // }


        AddEffectBox{

            id: addEffectBox

        }

    }
    //Color palette https://coolors.co/0b132b-1c2541-3a506b-5bc0be-6fffe9
}
