import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQml.Models 2.1
import QtQuick.Dialogs 1.3

GroupBox
{
    id: groupbox
    title: "Options"

    anchors.fill: parent
    anchors.margins: 5

    property var currentColor: "#000000"

    ColorDialog {
        id: colorDialog
        title: "Please choose a color"

        onAccepted: {
            groupbox.currentColor = colorDialog.color
        }
    }

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
                    id: classcombobox

                    Layout.alignment: Qt.AlignHCenter

                    editable: true

                    onAccepted: {
                        model.set(currentIndex, {"text": editText})
                        classcombobox.reload()
                    }

                    model: ListModel {
                        property var current: 0
                    }

                    /*
                    contentItem: Rectangle {
                        height: parent.height
                        width: classcombobox.width
                        Rectangle {
                            id: contentcolorcombobox
                            color: "red"
                            width: classcombobox.width * 0.2
                            height: classcombobox.height / 2
                        }
                        Text {
                            text: classcombobox.displayText
                            x: contentcolorcombobox.width + 10
                            width: classcombobox.width * 0.5
                            height: classcombobox.height / 2
                        }
                    }*/

                    delegate: ItemDelegate {
                        width: classcombobox.width
                        contentItem: Rectangle {
                            Rectangle {
                                id: contentcolor
                                color: "red"
                                width: classcombobox.width * 0.2
                                height: parent.height
                            }
                            Text {
                                text: modelData
                                x: contentcolor.width + 10
                                width: classcombobox.width * 0.5
                                height: parent.height
                            }
                        }
                        highlighted: classcombobox.highlightedIndex === index
                    }
                }

                RowLayout {
                    Layout.preferredWidth: parent.width
                    Button {
                        Layout.preferredWidth: parent.width / 2
                        text: "Edit Color"

                        onClicked: colorDialog.open()
                    }

                    Button {
                        Layout.preferredWidth: parent.width / 2
                        text: "New Class"

                        onClicked: {
                            classcombobox.model.current += 1
                            classcombobox.model.append({ text: classcombobox.model.current.toString() })
                        }
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
