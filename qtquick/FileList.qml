import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12
Item {

    property ListModel dataList
    property string editName
    property string destinationName
    property int editIndex: -1
    property int dirLevel: 0
    property bool newDir: false

    signal backup
    signal upload
    signal mkdir
    signal refreshDir
    signal downloadFile
    signal deleteDir
    signal deleteFile
    signal rename
    signal enterDir

    ColumnLayout {
        id: columnLayout4
        anchors.fill: parent

        RowLayout {
            id: rowLayout3
            width: 100
            height: 100
            Layout.fillHeight: true
            Layout.maximumHeight: 50
            Layout.minimumHeight: 50
            Layout.fillWidth: true

            Button {
                id: backupButton
                enabled: dirLevel != 0
                Layout.minimumHeight: 30
                Layout.minimumWidth: 100
                background: Rectangle {
                        color: backupButton.down ? "dodgerblue" :  "deepskyblue"
                        radius: 3
                        border.color: "#007bff"
                        border.width: 1
                    }

                contentItem: Text {
                    text: "返回上一级"
                    font.bold: true
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"
                }

                onClicked: {
                    backup()
                }
            }

            Button {
                id: uploadButton
                Layout.minimumHeight: 30
                Layout.minimumWidth: 100
                background: Rectangle {
                        color: uploadButton.down ? "dodgerblue" :  "deepskyblue"
                        radius: 3
                        border.color: "#007bff"
                        border.width: 1
                    }

                contentItem: Text {
                    text: "上传文件"
                    font.bold: true
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"
                }

                onClicked: {
                    upload()
                }
            }

            Button {
                id: mkdirButton
                Layout.minimumHeight: 30
                Layout.minimumWidth: 100
                background: Rectangle {
                        color: mkdirButton.down ? "dodgerblue" :  "deepskyblue"
                        radius: 3
                        border.color: "#007bff"
                        border.width: 1
                    }

                contentItem: Text {
                    text: "新建文件夹"
                    font.bold: true
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"
                }

                onClicked: {
                    if(editIndex != -1) return
                    editIndex = fileList.count
                    newDir = true
                    dataList.append({
                                        "infoType": "folder",
                                        "ext": "folder",
                                        "name": ""
                                    })
                }
            }

            Button {
                id: refreshButton
                Layout.minimumHeight: 30
                Layout.minimumWidth: 100
                background: Rectangle {
                        color: refreshButton.down ? "dodgerblue" :  "deepskyblue"
                        radius: 3
                        border.color: "#007bff"
                        border.width: 1
                    }

                contentItem: Text {
                    text: "刷新"
                    font.bold: true
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"
                }
                onClicked: {
                    refreshDir()
                }
            }
        }

        ScrollView {
            id: scroolView
            Layout.fillHeight: true
            Layout.fillWidth: true

            GridView {
                id: gridView
                topMargin: 20
                objectName: "gridView"
                rightMargin: 20
                leftMargin: 20
                anchors.fill: parent
                cellWidth: 150
                cellHeight: 150
                highlightFollowsCurrentItem: true
                focus: true
                model: dataList
                currentIndex: -1
                delegate: Component {
                    id: baseListDelegate
                    Item {
                        id: itemView
                        width: 120
                        height: 125
                        MouseArea{
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                            onClicked: {
                                switch (mouse.button) {
                                case Qt.LeftButton :
                                    gridView.currentIndex = index
                                    break
                                case Qt.RightButton :
                                    menu.popup()
                                    break
                                }
                            }
                            onDoubleClicked: {
                                if (infoType == "folder") {
                                    editName = name
                                    enterDir()
                                }
                            }

                            Menu {
                                id: menu
                                MenuItem {
                                    text: "下载"
                                    enabled: infoType == "file"
                                    onClicked: {
                                        editName = name
                                        downloadFile()
                                    }
                                }
                                MenuItem {
                                    text: "删除"
                                    onClicked: {
                                        if (infoType == "file") {
                                            editName = name
                                            deleteFile()
                                        }

                                        else {
                                            editName = name
                                            deleteDir()
                                        }
                                    }
                                }
                                MenuItem {
                                    text: "重命名"

                                    onClicked: {
                                        editIndex = index
                                    }
                                }
                            }
                        }

                        Column {
                            spacing: 5
                            Rectangle {
                                width: 100
                                height: 100
                                color: "transparent"
                                anchors.horizontalCenter: parent.horizontalCenter
                                Image {
                                    mipmap: true
                                    source: "qrc:/ext/src/ext/" + ext + ".png"
                                    anchors.fill: parent
                                }


                            }

                            Text {
                                visible: editIndex != index
                                x: 5
                                text: name
                                horizontalAlignment: Text.AlignHCenter
                                elide: Text.ElideRight
                                font.bold: true
                                width: 120
                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            TextInput {
                                id: dirNameInput
                                visible: editIndex == index
                                width: 120
                                horizontalAlignment: Text.AlignHCenter
                                Keys.onReleased: {
                                    if (event.key !== Qt.Key_Return || text.length <= 0) return ;
                                    if (newDir) {
                                        editName = text
                                        mkdir()
                                    }
                                    else {
                                        editName = name
                                        destinationName = text
                                        rename()
                                    }
                                }
                            }
                        }
                    }
                }

                highlight: Component {
                    Rectangle {
                        radius: 5
                        opacity: 0.3
                        color: "dodgerblue"
                    }
                }

                flickableChildren: MouseArea {
                    anchors.fill: parent
                    onClicked: gridView.currentIndex = -1
                }
            }
        }
    }
}
