import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.0

MenuBar {
    id: menuBar

    background: Rectangle {
        color: mainwindow.color
    }

    FileDialog {
        id: filedialog
        title: qsTr("Please select a file or directory...")
        nameFilters: ["Image files (*.jpg *.png)"]
        selectFolder: true
        folder: shortcuts.home

        onAccepted: { itemlistdrawer.open() }
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
        title: qsTr("File")

        Action {
            text: qsTr("New")
        }

        Action {
            text: qsTr("Open...")
            shortcut: "Ctrl+O"
            onTriggered: filedialog.open()
        }

        Action {
            text: qsTr("Open recent...")
        }
    }

    Menu {
        title: qsTr("Help")

        Action {
            text: qsTr("marK Handbook")
        }

        Action {
            text: qsTr("What's this?")
        }

        MenuSeparator { }

        Action {
            text: qsTr("Report bug")
        }

        MenuSeparator { }

        Action {
            text: qsTr("Donate")
        }

        MenuSeparator { }

        Action {
            text: qsTr("About marK")
        }

        Action {
            text: qsTr("About KDE")
        }
    }
}
