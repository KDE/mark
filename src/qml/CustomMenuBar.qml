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
        title: "Please select a file or directory..."
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
