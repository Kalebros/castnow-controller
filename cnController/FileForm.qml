import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Item {

    property string type
    property string name
    property int id
    property int iconHeight
    property int iconWidth
    property alias listOfSubtitles: listOfSubtitles

    Row {
        id: rowA
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: iconHeight
        spacing: 10

        Image {
            id: iconType
            height: iconHeight
            width: iconWidth
            antialiasing: true
            source: (type=="AVI") ? "/images/avi" : "images/movies"
            sourceSize.height: parent.height
            sourceSize.width: parent.width
        }

        Rectangle {
            height: iconHeight-5
            width: rowA.width - 10 - iconWidth
            color: "white"
            Text {
                id: textName
                font.family: "Roboto"
                anchors.fill: parent
                text: name + "("+id+")"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                fontSizeMode: Text.Fit
                elide: Text.ElideMiddle
            }
        }

    }

    Rectangle {
        color: "white"
        anchors.top: rowA.bottom
        anchors.bottom: backButton.top
        anchors.left: parent.left
        anchors.right: parent.right

        Button {
            id: playButton
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 20
            anchors.leftMargin: 15
            anchors.rightMargin: 15
            height: parent.height / 15
            text: "Play without subtitles ("+id+")"
            onClicked: formularioBase.playFile(id,"")
        }

        ListView {
            id: listOfSubtitles
            clip: true
            anchors.top: playButton.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.leftMargin: 25
            anchors.rightMargin: 25
            model: subtitlesModel
            delegate: Item {
                width: listOfSubtitles.width
                height: listOfSubtitles.height / 10
                Text{
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    width: parent.width- (parent.width / 5)
                    text: sName
                    fontSizeMode: Text.Fit
                    font.family: "Roboto"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                Button {
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    width: parent.width / 5
                    text: "Play ("+id+")"
                    onClicked: formularioBase.playFile(id,sName)
                }
            }
        }

    }


    Button {
        id: backButton
        text: "Back"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        height: parent.height / 15
        onClicked: { formularioBase.mostrarServidor(); }
    }
}

