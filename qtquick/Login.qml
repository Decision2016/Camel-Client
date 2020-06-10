import QtQuick 2.14
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.14
import QtQuick.Window 2.14

Item {
    property string username
    property string password
    signal clickLogin

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
                Layout.fillHeight: true
                Layout.fillWidth: true
                font.bold: true
                font.pixelSize: 25
            }



            TextEdit {
                id: usernameEdit
                width: 80
                height: 20
                textFormat: Text.PlainText
                selectByMouse: true
                topPadding: 15
                bottomPadding: 15
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
                topPadding: 15
                bottomPadding: 15
                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                font.bold: true
                font.pixelSize: 25
            }

            TextEdit {
                id: passwordEdit
                width: 80
                height: 20
                selectByMouse: true
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
                id: loginButton
                topPadding: 6
                checkable: false
                Layout.topMargin: 20
                Layout.minimumHeight: 30
                Layout.minimumWidth: 100
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                background: Rectangle {
                    color: loginButton.down ? "dodgerblue" : "deepskyblue"
                    radius :2
                }

                onClicked: {
                    username = usernameEdit.text
                    password = passwordEdit.text
                    clickLogin()
                }

                contentItem: Text {
                    text: "登录"
                    font.bold: true
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"
                }
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
