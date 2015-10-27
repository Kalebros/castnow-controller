import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480
    visible: true
    visibility: "FullScreen"

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Find server")
                onTriggered:messageDialog.show("hola!")
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    MainForm {
        id: formularioBase
        anchors.fill: parent
        searchButton.onClicked: controller.searchServer();
        disconnectButton.onClicked: limpiarYcambiarEstado();

        function mostrarServidor(name,puerto) {
            listaArchivos.model=fileModel;
            if(state==="conectado") {
                state="";
            } else state="conectado";
        }

        function limpiarYcambiarEstado() {
            if(state!="archivo")
                state="";
        }

        function playFile(id,sub) {
            controller.playFile(id,sub);
        }

        function mostrarArchivo(id,name,type,subtitles) {
            fileForm.type=type;
            fileForm.name=name;
            fileForm.id=id;
            subtitlesModel.clear();
            if(subtitles) {
                var list=fileModel.getSubtitles(id);
                for(var i=0;i<list.length;i++) {
                    var element={};
                    element["sName"]=list[i];
                    subtitlesModel.append(element);
                }
            }

            state="archivo";
        }

        ListModel {
            id: subtitlesModel
        }

    }

    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();

        }
    }


    Connections {
        target: controller
        onServerFound: formularioBase.mostrarServidor(sName,port);
    }
}
