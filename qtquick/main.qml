import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12
import QtQuick.VirtualKeyboard 2.14
import QtQuick.Dialogs 1.2
import cn.decision01.modules 1.0

import "qrc:/modules"

Window {
    property bool hasLogin: false
    property int stackIndex: 0
    property int runningCount

    id: window
    visible: true
    color : "ghostwhite"
    width: 1200
    height: 741
    minimumWidth: 1200
    minimumHeight: 741
    flags: Qt.Window | Qt.CustomizeWindowHint

    ListModel {
        id: fileList
    }

    ListModel {
        id: transportList
    }

    CamelClient{
        id: camelClient
        onLoginSuccess: {
            hasLogin = true
            stackIndex = 1
            columnLayout2.nowObj = 1
            refresh()
        }

        onCreateDirSuccess: {
            refresh()
            fileListPage.setIndex()
        }

        onDeleteDirSuccess: {
            refresh()
        }

        onBackupSuccess: {
            fileListPage.decrease()
            refresh()
        }

        onEnterDirSuccess: {
            fileListPage.increase()
            refresh()
        }

        onRenameSuccess: {
            fileListPage.setIndex()
            refresh()
        }
    }

    Timer {
        id: queueTimer
        repeat: true
        running: false
        interval: 500
        triggeredOnStart: true

        onTriggered: refreshQueue()
    }

    FileDialog {
        id: fileDialog
        title: "选择文件"
        folder: shortcuts.desktop
        selectMultiple: true

        onAccepted: {
            for (var pos = 0; pos < fileUrls.length; pos++) {
                camelClient.uploadFile(decodeUrl(fileUrls[pos]))
            }
        }
    }

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
                        visible: false
                        display: AbstractButton.IconOnly
                        enabled: hasLogin
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
                        enabled: hasLogin
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
                            stackIndex = 1
                            queueTimer.running = false
                        }
                    }

                    RoundButton {
                        id: roundButton1
                        enabled: hasLogin
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
                            stackIndex = 2
                            refreshQueue()
                            queueTimer.running = true
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
                    enabled: hasLogin
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

                        onClicked: {
                            window.showMinimized()
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
                currentIndex: stackIndex

                Login {
                    id: loginPage
                    onClickLogin: {
                        camelClient.signUser(username, password)
                    }
                }

                FileList {
                    id: fileListPage
                    dataList: fileList

                    onEnterDir: camelClient.openDirectory(editName)
                    onUpload: fileDialog.open()
                    onRefreshDir: refresh()
                    onDownloadFile: camelClient.downloadFile(editName)
                    onDeleteDir: camelClient.deleteDirectory(editName)
                    onDeleteFile: camelClient.deleteFile(editName)
                    onBackup: {
                        if (dirLevel == 0) return ;
                        camelClient.backupDirectory();
                    }
                    onMkdir: camelClient.createDirectory(editName)
                    onRename: camelClient.rename(editName, destinationName)
                }

                LoadList {
                    id: transportListPage
                    dataList: transportList

                    onClickPause: camelClient.pauseTask(clickedIndex)
                    onClickStart: camelClient.startTask(clickedIndex - runningCount)
                    onClickStop: camelClient.stopTask()
                    onClickDeleteRunning: camelClient.deleteTask(clickedIndex, true)
                    onClickDeletePaused: camelClient.deleteTask(clickedIndex - runningCount, false)
                }
            }
        }
    }
    /*
     *此处开始用于编写全局函数
     */

    function getList() {
        var infoString = String(camelClient.getDirInfo())
        var list = infoString.split(';')
        for (var i = 0; i < list.length - 1; i++) {
            var obj = list[i].split('/')
            fileList.append({
                                "infoType": obj[0] === "0" ? "folder" : "file",
                                "ext": checkExtName(obj[0], obj[1]),
                                "name": obj[2]
                            })
        }
    }

    function checkExtName(type, extName) {
        var extArray = ["avi", "dir", "dll", "doc", "exe", "gif", "html", "jpg", "mkv", "mp3", "mp4", "mpg", "pdf", "png", "ppt",  "psd", "rmvb", "swf", "txt", "wav", "xls", "zip"]
        if (extArray.indexOf(extName) !== -1) return extName
        else if(type === "0" && extName === "folder") return "folder";
        return "other"
    }

    function refresh() {
        fileList.clear()
        getList()
    }

    function refreshQueue() {
        var infoString = String(camelClient.getQueueInfo())
        var list = infoString.split(";")
        transportList.clear()
        runningCount = 0
        for (var i = 0; i < list.length - 1; i++) {
            console.log(list[i])
            var obj = list[i].split('/')
            var pos = obj[0].lastIndexOf('.')
            var extName = "other"
            if (obj[3] === "running") runningCount ++;
            if (pos !== -1) extName = obj[0].substring(pos + 1, obj[0].length)
            transportList.append({
                                     "name": obj[0],
                                     "percent": obj[1],
                                     "uploading": obj[2] === "1" ? true : false,
                                     "status": obj[3],
                                     "ext": checkExtName("1", extName),
                                 })
        }
    }

    function decodeUrl(urlObj) {
        var path = urlObj.toString()
        path = path.replace(/^(file:\/{3})/,"")
        return path
    }
}


