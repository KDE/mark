import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.0

ApplicationWindow
{
    id: mainwindow

    title: "marK"

    visible: true
    visibility: Window.Maximized

    minimumWidth: 800
    minimumHeight: 600

    FileDialog {
        id: filedialog
        title: "Please select a file or directory..."
        nameFilters: ["Image files (*.jpg *.png)"]
        selectFolder: true

        onAccepted: { itemlistdrawer.open() }
    }

    menuBar: MenuBar {
        id: menuBar

        background: Rectangle {
            color: mainwindow.color
        }

        delegate: MenuBarItem {
            id: menuBarItem
            contentItem: Text {
                text: menuBarItem.text
                font: menuBarItem.font
                opacity: enabled ? 1.0 : 0.3
                color: menuBarItem.highlighted ? "#BDBDBD" : "#FFFFFF"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 40
                opacity: enabled ? 1.0 : 0.3
                color: menuBarItem.highlighted ? "white" : "#BDBDBD"
            }
        }

        Menu {
            title: "File"

            Action {
                text: "New"
            }

            Action {
                text: "Open..."
                shortcut: "Ctrl+O"
                onTriggered: filedialog.open()
            }

            Action {
                text: "Open recent..."
            }
        }

        Menu {
            title: "Help"

            Action {
                text: "marK Handbook"
            }

            Action {
                text: "What's this?"
            }

            MenuSeparator { }

            Action {
                text: "Report bug"
            }

            MenuSeparator { }

            Action {
                text: "Donate"
            }

            MenuSeparator { }

            Action {
                text: "About marK"
            }

            Action {
                text: "About KDE"
            }
        }
    }

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
