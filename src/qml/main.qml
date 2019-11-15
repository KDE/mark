import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.0

ApplicationWindow
{
    id: mainwindow

    title: "marK"

    visible: true
    visibility: Window.Maximized

    minimumWidth: 800
    minimumHeight: 600

    menuBar: CustomMenuBar { }

    Drawer {
        id: actionboxdrawer

        edge: Qt.LeftEdge
        dragMargin: 10

        y: menuBar.height
        width: actionBox.width + 10
        height: parent.height - menuBar.height

        ActionBox { id: actionBox }
    }

    Drawer {
        id: itemlistdrawer

        edge: Qt.RightEdge
        dragMargin: 10

        y: menuBar.height
        width: parent.width * 0.2
        height: parent.height - menuBar.height

        ItemList { }
    }

    ItemViewer {
        id: itemviewer
        anchors.fill: parent
        anchors.margins: 10
    }

}
