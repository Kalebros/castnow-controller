import QtQuick 2.0

Item {

    property string fileType
    property string fileName
    property bool fileHasSubtitles
    property int fileId

    MouseArea {
        anchors.fill: parent
        onClicked:
        {
            formularioBase.mostrarArchivo(fileId,fileName,fileType,fileHasSubtitles);
        }
    }

    Image {
        id: delegateIcon
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2
        anchors.topMargin: 2
        anchors.leftMargin: 10
        width: delegateIcon.height
        source: (fileType=="AVI") ? "/images/avi" : "images/movies"
        sourceSize.height: parent.height
        sourceSize.width: parent.width
        antialiasing: true
    }
    Rectangle {
        id: delegateRect
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: delegateIcon.right
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.leftMargin: 5
        anchors.bottomMargin: 2
        anchors.topMargin: 2
        Text {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: subIcon.left
            anchors.leftMargin: 3
            anchors.rightMargin: 3
            font.family: "Roboto"
            text: fileName
            fontSizeMode: Text.Fit
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        Image {
            id: subIcon
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 5
            anchors.rightMargin: 7
            height: parent.height / 2 +(parent.height / 3)
            width: height
            source: "images/subtitles"
            sourceSize.height: parent.height
            sourceSize.width: parent.width
            visible: fileHasSubtitles
        }
    }
}
