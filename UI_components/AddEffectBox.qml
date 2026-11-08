import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

Item{
    id: root
    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.rightMargin: 10
    Layout.topMargin: 10
    Layout.bottomMargin: 10
    Layout.maximumWidth: 200
    Layout.row: 1
    Layout.column: 3

    Rectangle{
        anchors.fill: parent

        color: "#0B132B"

        ColumnLayout{
            id: listing
            spacing: 10
            anchors.fill: parent

            Item{
                id: addEffectControls
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                height: 50


                ComboBox{
                    id: dropDown
                    width: parent.width / 3 * 2
                    height: parent.height
                    anchors{
                        left: parent.left
                    }

                    background: Rectangle{
                        color: "#304359"
                        topLeftRadius: 4
                        bottomLeftRadius: 4
                    }

                    model: ["Option 1", "Option 2", "Option 3"]

                }

                Button{
                    id: addEffect

                    height: parent.height
                    anchors{
                        left: dropDown.right
                        right: parent.right
                    }

                    background: Rectangle{
                        color: addEffect.pressed ? "#1e2a38" : (addEffect.hovered ? "#2b3c4f" : "#304359")

                        topRightRadius: 4
                        bottomRightRadius: 4
                    }

                    Image{
                        anchors {
                            verticalCenter: parent.verticalCenter
                            horizontalCenter: parent.horizontalCenter
                        }
                        source: "../assets/icons/plus_icon.svg"
                        width: 20
                        height: 20
                    }
                }
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10

                clip: true

                Column {
                    width: parent.width
                    spacing: 10

                    Repeater {
                        model: 20
                        EffectInstance{
                            idx: 0
                            effectName: "Gray scale"
                        }
                    }
                }
            }

            Item{
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                height: 30

                Button{
                    id: applyButton
                    anchors.fill: parent


                    background: Rectangle{
                        color: applyButton.pressed ? "#1e403f" : (applyButton.hovered ? "#316665" : "#5BC0BE")
                        radius: 4
                    }



                    Text{
                        anchors{
                            horizontalCenter: parent.horizontalCenter
                            verticalCenter: parent.verticalCenter
                        }

                        text: "Apply effect"
                    }
                }
            }
        }

    }
}