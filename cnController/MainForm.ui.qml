import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    id: mainItem
    anchors.fill: parent

    property alias searchButton: searchButton
    property alias disconnectButton: disconnectButton
    property alias listaArchivos: listaArchivos
    property alias fileForm: fileForm

    Button {
        id: searchButton
        x: 269
        y: 236
        text: qsTr("Search servers")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    Button {
        id: disconnectButton
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        height: (parent.height>parent.width) ? parent.height / 15 : parent.width / 15
        anchors.leftMargin: 15
        anchors.rightMargin: 15
        text: qsTr("Disconnect")
        opacity: 0
    }


    ListView {
        id: listaArchivos
        anchors.top: disconnectButton.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true
        visible: false
        model: ListModel {
        }
        delegate: FileDelegate {
            width: listaArchivos.width
            height: (listaArchivos.height > listaArchivos.width) ? listaArchivos.height / 15 : listaArchivos.width / 15

            fileType: type
            fileName: name
            fileHasSubtitles: hasSubtitles
            fileId: id
        }
    }

    FileForm {
        id: fileForm
        anchors.fill: parent
        iconHeight: parent.height / 10
        iconWidth: parent.height / 10
        visible: false
    }

    states: [
        State {
            name: "conectado"

            PropertyChanges {
                target: fileForm
                visible: false
            }

            PropertyChanges {
                target: listaArchivos
                width: 640
                height: 366
                visible: true
            }

            PropertyChanges {
                target: searchButton
                visible: false
            }

            PropertyChanges {
                target: disconnectButton
                x: 270
                y: 436
                text: qsTr("Disconnect")
                opacity: 1
            }
        },
        State {
            name: "archivo"

            PropertyChanges {
                target: listaArchivos
                width: 640
                height: 366
                visible: false
            }

            PropertyChanges {
                target: searchButton
                visible: false
            }

            PropertyChanges {
                target: disconnectButton
                x: 270
                y: 436
                text: qsTr("Disconnect")
                opacity: 0
            }

            PropertyChanges {
                target: fileForm
                visible: true
            }
        }
    ]
}
