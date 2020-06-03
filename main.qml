import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12

Window {
    id: window
    visible: true
    color : "ghostwhite"
    width: 1200
    height: 741
    minimumWidth: 1200
    minimumHeight: 741
    flags: Qt.Window | Qt.CustomizeWindowHint

    RowLayout {
        id: rowLayout
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent

        ColumnLayout {
            id: columnLayout
            width: 100
            height: 100
            spacing: 0
            Layout.minimumWidth: 100
            Layout.maximumWidth: 100
            Layout.fillHeight: true

            Rectangle {
                id: rectangle
                width: 200
                height: 200
                color: "#ffffff"
                Layout.fillHeight: true
                Layout.fillWidth: true

                ColumnLayout {

                    id: columnLayout2
                    width: 71
                    height: 208
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    property int nowObj:-1

                    RoundButton {
                        id: roundButton6
                        text: "+"
                        visible: false
                        display: AbstractButton.IconOnly
                        Layout.minimumHeight: 70
                        Layout.minimumWidth: 70
                        Layout.maximumHeight: 70
                        Layout.maximumWidth: 70
                        icon.source: "qrc:/icons/src/index-static.png"
                        icon.color: (hovered || columnLayout2.nowObj == 0) ? "deepskyblue": "black"

                        palette {
                            button: columnLayout2.nowObj == 0 ? "ghostwhite" : "white"
                        }

                        onClicked: {
                            columnLayout2.nowObj = 0
                        }
                    }

                    RoundButton {
                        id: roundButton
                        x: 0
                        display: AbstractButton.IconOnly
                        Layout.maximumHeight: 70
                        Layout.maximumWidth: 70
                        Layout.minimumHeight: 70
                        Layout.minimumWidth: 70
                        icon.source: "qrc:/icons/src/list-static.png"
                        icon.color: (hovered || columnLayout2.nowObj == 1) ? "deepskyblue": "black"

                        palette {
                            button: columnLayout2.nowObj == 1 ? "ghostwhite" : "white"
                        }

                        onClicked: {
                            columnLayout2.nowObj = 1
                        }
                    }

                    RoundButton {
                        id: roundButton1
                        x: 0
                        display: AbstractButton.IconOnly
                        Layout.maximumHeight: 70
                        Layout.maximumWidth: 70
                        Layout.minimumHeight: 70
                        Layout.minimumWidth: 70
                        icon.source: "qrc:/icons/src/queue-static.png"
                        icon.color: (hovered || columnLayout2.nowObj == 2) ? "deepskyblue": "black"

                        palette {
                            button: columnLayout2.nowObj == 2 ? "ghostwhite" : "white"
                        }

                        onClicked: {
                            columnLayout2.nowObj = 2
                        }
                    }

                }
            }

            Rectangle {
                id: divideLine
                width: 200
                height: 200
                color: "#f1f1f1"
                Layout.minimumHeight: 1
                Layout.maximumHeight: 1
                Layout.fillWidth: true
            }

            Rectangle {
                id: rectangle1
                width: 200
                height: 200
                color: "#ffffff"
                border.width: 0
                Layout.maximumHeight: 100
                Layout.maximumWidth: 100
                Layout.minimumHeight: 100
                Layout.minimumWidth: 100

                RoundButton {
                    id: roundButton2
                    width: 70
                    height: 70
                    text: "+"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    display: AbstractButton.IconOnly
                    icon.source: "qrc:/icons/src/exit-static.png"

                    palette {
                        button: "white"
                    }
                }
            }

        }

        ColumnLayout {
            id: columnLayout1
            width: 100
            height: 100

            RowLayout {
                id: rowLayout2
                width: 100
                height: 100
                spacing: 0
                Layout.maximumHeight: 50
                Layout.fillWidth: true
                Layout.maximumWidth: 65535

                MouseArea {
                    id: mouseArea
                    width: 100
                    height: 100
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.maximumHeight: 50

                    property point lastMousePos: Qt.point(0, 0)
                    onPressed: { lastMousePos = Qt.point(mouseX, mouseY); }
                    onMouseXChanged: window.x += (mouseX - lastMousePos.x)
                    onMouseYChanged: window.y += (mouseY - lastMousePos.y)
                }

                RowLayout {
                    id: rowLayout1
                    width: 159
                    height: 50
                    Layout.rightMargin: 20

                    RoundButton {
                        id: roundButton3
                        Layout.maximumHeight: 30
                        Layout.maximumWidth: 30
                        icon.source: "qrc:/icons/src/minus-static.png"
                        icon.color: hovered ? "deepskyblue" : "black"

                        palette {
                            button: "ghostwhite"
                        }
                    }

                    RoundButton {
                        property bool maxStatus: false
                        id: roundButton4
                        Layout.maximumHeight: 30
                        Layout.maximumWidth: 30
                        icon.source: maxStatus ? "qrc:/icons/src/shrink.png" : "qrc:/icons/src/arrowsalt-static.png"
                        icon.color: hovered ? "deepskyblue" : "black"

                        palette {
                            button: "ghostwhite"
                        }

                        onClicked: {
                            if (maxStatus == false) {
                                window.showMaximized()
                                maxStatus = true
                            }
                            else {
                                window.showNormal()
                                maxStatus = false
                            }
                        }

                    }

                    RoundButton {
                        id: roundButton5
                        Layout.maximumHeight: 30
                        Layout.maximumWidth: 30
                        icon.source: "qrc:/icons/src/cross-static.png"
                        icon.color: hovered ? "red" : "black"
                        palette {
                            button: "ghostwhite"
                        }

                        onClicked: {
                            Qt.quit()
                        }
                    }

                }

            }

            StackLayout {
                id: stackLayout
                width: 100
                height: 100
                currentIndex: 1

                Item {
                    id: element2

                    Rectangle {
                        id: rectangle2
                        width: 654
                        height: 340
                        color: "#ffffff"
                        radius: 13
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter

                        ColumnLayout {
                            id: columnLayout3
                            x: -115
                            y: -7
                            width: 430
                            height: 235
                            spacing: 0
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter


                            Text {
                                id: element1
                                text: qsTr("Username")
                                verticalAlignment: Text.AlignVCenter
                                bottomPadding: 15
                                topPadding: 15
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                font.bold: true
                                font.pixelSize: 25
                            }



                            TextEdit {
                                id: textEdit
                                width: 80
                                height: 20
                                text: qsTr("")
                                verticalAlignment: Text.AlignBottom
                                Layout.maximumHeight: 40
                                Layout.minimumHeight: 40
                                Layout.fillWidth: true
                                font.pixelSize: 22

                            }

                            Rectangle {
                                id: rectangle6
                                width: 200
                                height: 200
                                color: "deepskyblue"
                                Layout.maximumHeight: 1
                                Layout.fillWidth: true
                                Layout.minimumHeight: 1
                            }

                            Text {
                                id: element
                                text: "Password"
                                verticalAlignment: Text.AlignVCenter
                                bottomPadding: 15
                                topPadding: 15
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                font.bold: true
                                font.pixelSize: 25
                            }

                            TextEdit {
                                id: textEdit1
                                width: 80
                                height: 20
                                text: qsTr("")
                                verticalAlignment: Text.AlignBottom
                                Layout.fillWidth: true
                                Layout.maximumHeight: 40
                                Layout.minimumHeight: 40
                                font.pixelSize: 22
                            }

                            Rectangle {
                                id: rectangle5
                                width: 200
                                height: 200
                                color: "deepskyblue"
                                Layout.maximumHeight: 1
                                Layout.fillWidth: true
                                Layout.minimumHeight: 1
                            }

                            Button {
                                id: button
                                text: qsTr("Login")
                                topPadding: 6
                                checkable: false
                                Layout.topMargin: 20
                                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            }



                        }


                    }

                    Text {
                        id: element3
                        width: 151
                        height: 60
                        text: qsTr("Login")
                        anchors.verticalCenterOffset: -220
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenterOffset: -260
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.bold: true
                        font.pixelSize: 50
                    }






                }

                Item {

                    RowLayout {
                        id: fileList
                        anchors.fill: parent
                    }

                }

                Item {
                    RowLayout {
                        id: uploadList
                        anchors.fill: parent
                    }
                }


            }


        }
    }
}




