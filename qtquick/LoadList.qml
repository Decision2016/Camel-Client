import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12
Item {
    property ListModel dataList
    property int clickedIndex

    Layout.fillHeight: true
    Layout.fillWidth: true

    signal clickStart
    signal clickPause
    signal clickStop
    signal clickDelete

    ColumnLayout {
        id: columnLayout5
        anchors.fill: parent

        RowLayout {
            id: rowLayout4
            width: 100
            height: 100
            Layout.maximumHeight: 50
            Layout.fillWidth: true
            Layout.minimumHeight: 50
        }

        ScrollView {
            id: scrollView
            topPadding: 20
            rightPadding: 20
            leftPadding: 20
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                id: listView
                clip: true
                orientation: ListView.Vertical
                model: dataList
                spacing: 10
                delegate: Item {
                    id: element5
                    height: 80
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    Rectangle {
                        radius: 3
                        border.color: "#e2dddd"
                        border.width: 1
                        anchors.fill: parent
                        Row {
                            id: rowView
                            anchors.rightMargin: 25
                            anchors.leftMargin: 25
                            anchors.bottomMargin: 10
                            anchors.topMargin: 10
                            anchors.fill: parent
                            spacing: 20

                            Image {
                                mipmap: true
                                width: 60
                                height: 60
                                id: imgView
                                source: "qrc:/ext/src/ext/" + ext + ".png"
                            }

                            Column {
                                id: column
                                anchors.right: buttonsRow.right
                                anchors.rightMargin: 120
                                anchors.left: imgView.left
                                anchors.leftMargin: 70

                                Row {
                                    id: row
                                    Rectangle {
                                        id: rectangle
                                        width: 60
                                        height: 25
                                        color: "#00bfff"
                                        radius: 2
                                        border.color: "#ffffff"
                                        border.width: 2
                                        anchors.verticalCenter: parent.verticalCenter

                                        Text {
                                            color: "#ffffff"
                                            text: uploading ? "上传" : "下载"
                                            font.bold: true
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            anchors.verticalCenter: parent.verticalCenter
                                        }
                                    }

                                    Text {
                                        text: name
                                        bottomPadding: 10
                                        topPadding: 10
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }



                                ProgressBar {
                                    id: progressbar
                                    value: percent
                                    to: 100
                                    anchors.right: parent.right
                                    anchors.rightMargin: 0
                                    anchors.left: parent.left
                                    anchors.leftMargin: 0

                                    contentItem: Item {
                                            implicitWidth: parent.width
                                            implicitHeight: parent.height

                                            Rectangle {
                                                width: parent.width * progressbar.visualPosition
                                                height: parent.height
                                                radius: 5
                                                color: "#17a81a"
                                            }
                                     }
                                }
                            }
                            Row {
                                id: buttonsRow
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: 0
                                spacing: 5
                                RoundButton {
                                    width: 25
                                    height: 25
                                    display: AbstractButton.IconOnly
                                    anchors.verticalCenter: parent.verticalCenter

                                    icon.source: status == "paused" ? "qrc:/icons/src/play.png" : "qrc:/icons/src/pause.png"
                                    icon.color: hovered ? "darkorange" : "black"

                                    onClicked: {
                                        clickedIndex = index
                                        if (status === "running") {
                                            clickPause()
                                        }
                                        else {
                                            clickStart()
                                        }
                                    }
                                }

                                RoundButton {
                                    width: 25
                                    height: 25
                                    display: AbstractButton.IconOnly
                                    anchors.verticalCenter: parent.verticalCenter
                                    icon.color: hovered ? "red" : "black"
                                    icon.source: "qrc:/icons/src/stop.png"

                                    palette: {
                                        button : "ghostwhite"
                                    }

                                }

                                RoundButton {
                                    width: 25
                                    height: 25
                                    display: AbstractButton.IconOnly
                                    anchors.verticalCenter: parent.verticalCenter
                                    icon.source:  "qrc:/icons/src/close.png"
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
