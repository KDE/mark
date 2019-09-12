import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQml.Models 2.1

GroupBox
{
    id: groupbox
    title: "Options"

    anchors.fill: parent
    anchors.margins: 5

    ColumnLayout {
        width: childrenRect.width
        spacing: 5

        GroupBox {
            id: shapes
            title: "Shapes"
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter

            RowLayout {
                spacing: 3
                anchors.fill: parent

                ToolButton {
                    Layout.alignment: Qt.AlignHCenter
                    icon.source: "qrc:res/polygon_icon.png"
                }

                ToolButton {
                    Layout.alignment: Qt.AlignHCenter
                    icon.source: "qrc:res/circle_icon.png"
                }

                ToolButton {
                    Layout.alignment: Qt.AlignHCenter
                    icon.source: "qrc:res/rect_icon.png"
                }

                ToolButton {
                    Layout.alignment: Qt.AlignHCenter
                    icon.source: "qrc:res/triangle_icon.png"
                }
            }
        }

        GroupBox {
            title: "Classes"
            Layout.fillWidth: true

            ColumnLayout {
                anchors.fill: parent

                ComboBox {
                    Layout.alignment: Qt.AlignHCenter

                    model: ["A", "B", "C"]
                }

                RowLayout {
                    Layout.preferredWidth: parent.width
                    Button {
                        Layout.preferredWidth: parent.width / 2
                        text: "Edit Color"
                    }

                    Button {
                        Layout.preferredWidth: parent.width / 2
                        text: "New Class"
                    }
                }
            }
        }

        GroupBox {
            title: "Edit"
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent
                Button {
                    Layout.preferredWidth: parent.width / 2
                    text: "Erase"
                }

                Button {
                    Layout.preferredWidth: parent.width / 2
                    text: "Undo"
                }
            }
        }
    }
}
