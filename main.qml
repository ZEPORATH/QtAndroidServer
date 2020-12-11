import QtQuick 2.12
import QtQuick.Controls 2.0
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import test.server 1.0

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Server
    {
        id: server;
        onMessageReady: {
            statusText.text += data+"\n";
        }
    }


    Rectangle {
        anchors.horizontalCenter: statusScreen.horizontalCenter;
        id: serverStateToggler
        width: parent.width*0.2;
        height: parent.height*0.08;
        anchors.leftMargin: 20;
        anchors.topMargin: 12;
        color: "orange";
        Text {
            id: label
            text: server.getServerState() ? qsTr("Stop") : qsTr("Start");
            font.pointSize: 12;
            horizontalAlignment: Text.AlignHCenter;
            anchors.centerIn: parent;
        }
        MouseArea {
            anchors.fill: parent;
            onClicked: {
                console.log("Clicked");
                server.toggleServerState();
                label.text = server.getServerState() ? qsTr("Stop") : qsTr("Start");
            }
        }
    }


    Rectangle {
        id: statusScreen;
        width: parent.width*0.8;
        height: parent.height*0.8;
        border.color: "gray";
        radius: 8;
        anchors.top:serverStateToggler.bottom;
        anchors.topMargin: 12;
        anchors.leftMargin: 20;
        anchors.centerIn: parent;
        Flickable {
            id: flickArea;
            anchors.fill: parent;
            contentWidth: statusText.width;
            contentHeight: statusText.height;
            flickableDirection: Flickable.VerticalFlick;
            contentY: contentHeight-height;
            clip: true
            ScrollBar.vertical: ScrollBar {
                parent: flickArea.parent;
                anchors.top: flickArea.top;
                anchors.right: flickArea.right;
                anchors.bottom: flickArea.bottom;
                hoverEnabled: true;
                active: hovered || pressed;

            }
            TextEdit{
                id: statusText
                wrapMode: TextEdit.Wrap
                width:statusScreen.width;
                readOnly:true

                text:  "Welcome to simple webSocket Server App\n";
            }
        }
    }
}

