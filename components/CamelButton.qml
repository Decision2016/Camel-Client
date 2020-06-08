import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12
Item {
    property bool isEnabled: false
    property string source
    property string color
    property Component palette
    signal clicked

    RoundButton {
        display: AbstractButton.IconOnly
        enabled: isEnabled
        Layout.minimumHeight: 70
        Layout.minimumWidth: 70
        Layout.maximumHeight: 70
        Layout.maximumWidth: 70
        icon.source: source
        icon.color: color

        palette: palette

        onClicked: {
            clicked()
        }
    }
}
