import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12
import QtQuick.VirtualKeyboard 2.14
import cn.decision01.modules 1.0

Window {
    property bool hasLogin: false
    property int stackIndex: 0
    property int editIndex: -1
    property int dirLevel: 0
    property bool newDir: false

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

        ListElement {
           infoType: "folder"
           ext: "folder"
           name: "test.png"
        }

        ListElement {
           infoType: "folder"
           ext: "folder"
           name: "test.png"
        }
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
            editIndex = -1
            newDir = false
        }

        onDeleteDirSuccess: {
            refresh()
        }

        onBackupSuccess: {
            dirLevel --
            refresh()
        }

        onEnterDirSuccess: {
            dirLevel ++
            refresh()
        }

        onRenameSuccess: {
            editIndex = -1
            refresh()
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
                        text: "+"
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
                                textFormat: Text.PlainText
                                selectByMouse: true
                                verticalAlignment: Text.AlignBottom
                                Layout.maximumHeight: 40
                                Layout.minimumHeight: 40
                                Layout.fillWidth: true
                                font.pixelSize: 22
                                activeFocusOnTab: true
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
                                selectByMouse: true
                                verticalAlignment: Text.AlignBottom
                                Layout.fillWidth: true
                                Layout.maximumHeight: 40
                                Layout.minimumHeight: 40
                                font.pixelSize: 22
                                activeFocusOnTab: true
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

                                onClicked: {
                                    camelClient.signUser(textEdit.text, textEdit1.text)
                                }

                                KeyNavigation.tab: textEdit
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
                    id: element4

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
                                id: backup
                                Layout.minimumHeight: 30
                                Layout.minimumWidth: 100
                                background: Rectangle {
                                        color: button3.down ? "dodgerblue" :  "deepskyblue"
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
                                    if (dirLevel == 0) return ;
                                    camelClient.backupDirectory();
                                }
                            }

                            Button {
                                id: button1
                                Layout.minimumHeight: 30
                                Layout.minimumWidth: 100
                                background: Rectangle {
                                        color: button1.down ? "dodgerblue" :  "deepskyblue"
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
                            }

                            Button {
                                id: button2
                                Layout.minimumHeight: 30
                                Layout.minimumWidth: 100
                                background: Rectangle {
                                        color: button2.down ? "dodgerblue" :  "deepskyblue"
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
                                    fileList.append({
                                                        "infoType": "folder",
                                                        "ext": "folder",
                                                        "name": ""
                                                    })
                                }
                            }

                            Button {
                                id: button3
                                Layout.minimumHeight: 30
                                Layout.minimumWidth: 100
                                background: Rectangle {
                                        color: button3.down ? "dodgerblue" :  "deepskyblue"
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
                                    refresh()
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
                                model: fileList
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
                                                    if (infoType === "folder") {
                                                        dirMenu.popup()
                                                    }
                                                    else {
                                                        fileMenu.popup()
                                                    }

                                                    break
                                                }
                                            }
                                            onDoubleClicked: {
                                                console.log("test")
                                                if (infoType == "folder") {
                                                    camelClient.openDirectory(name)
                                                }
                                            }

                                            Menu {
                                                id: fileMenu
                                                MenuItem {
                                                    text: "下载"
                                                }
                                                MenuItem {
                                                    text: "删除"
                                                }
                                                MenuItem {
                                                    text: "重命名"

                                                    onClicked: {
                                                        editIndex = index
                                                    }
                                                }
                                            }

                                            Menu {
                                                id: dirMenu
                                                MenuItem {
                                                    text: "删除"
                                                    onClicked: {
                                                        camelClient.deleteDirectory(name)
                                                    }
                                                }
                                                MenuItem {
                                                    text: "重命名"

                                                    onClicked: editIndex = index
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
                                                    if (newDir) camelClient.createDirectory(text)
                                                    else {
                                                        camelClient.rename(name, text)
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

                    BusyIndicator {
                        id: busyIndicator
                        x: 0
                        y: 625
                        visible: false

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }

                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
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

                            Button {
                                id: button4
                                text: qsTr("Button")
                            }

                            Button {
                                id: button5
                                text: qsTr("Button")
                            }
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
                                delegate: Item {
                                    id: element5
                                    height: 70
                                    anchors.left: parent.left
                                    anchors.leftMargin: 0
                                    anchors.right: parent.right
                                    anchors.rightMargin: 0

                                    Row {
                                        id: rowView
                                        spacing: 20
                                        anchors.left: parent.left
                                        anchors.leftMargin: 0
                                        anchors.right: parent.right
                                        anchors.rightMargin: 0

                                        Image {
                                            mipmap: true
                                            width: 60
                                            height: 60
                                            id: imgView
                                            source: "qrc:/ext/src/ext/png.png"
                                        }

                                        Column {
                                            id: column
                                            anchors.right: buttonsRow.right
                                            anchors.rightMargin: 120
                                            anchors.left: imgView.left
                                            anchors.leftMargin: 70

                                            Text {
                                                text: rowView.width
                                                bottomPadding: 10
                                                topPadding: 10
                                                verticalAlignment: Text.AlignVCenter
                                            }

                                            ProgressBar {
                                                id: progressbar
                                                value: 30
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
                                            anchors.right: parent.right
                                            anchors.rightMargin: 0
                                            spacing: 5
                                            RoundButton {
                                                width: 30
                                                height: 30
                                                anchors.verticalCenter: parent.verticalCenter

                                            }

                                            RoundButton {
                                                width: 30
                                                height: 30
                                                anchors.verticalCenter: parent.verticalCenter

                                            }

                                            RoundButton {
                                                width: 30
                                                height: 30
                                                anchors.verticalCenter: parent.verticalCenter

                                            }
                                        }
                                    }
                                }
                                model: ListModel {

                                }
                            }
                        }
                    }
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
}


