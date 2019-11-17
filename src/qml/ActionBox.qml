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

    property var currentColor: classesModel.get(classcombobox.model.current).color

    ColorDialog {
        id: colorDialog
        title: qsTr("Please choose a color")

        onAccepted: {
            classesModel.set(classcombobox.model.current, {"color": colorDialog.color.toString()})
        }
    }

    ColumnLayout {
        width: childrenRect.width
        spacing: 5

        GroupBox {
            id: shapes

            property string selectedShape: "polygon"

            title: qsTr("Shapes")
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter

            RowLayout {
                spacing: 3
                anchors.fill: parent

                ToolButton {
                    property string name: "polygon"
                    Layout.alignment: Qt.AlignHCenter
                    icon.source: "qrc:res/polygon_icon.png"
                    highlighted: shapes.selectedShape === name
                    onClicked: shapes.selectedShape = name
                }

                ToolButton {
                    property string name: "circle"
                    Layout.alignment: Qt.AlignHCenter
                    icon.source: "qrc:res/circle_icon.png"
                    highlighted: shapes.selectedShape === name
                    onClicked: shapes.selectedShape = name
                }

                ToolButton {
                    property string name: "rect"
                    Layout.alignment: Qt.AlignHCenter
                    icon.source: "qrc:res/rect_icon.png"
                    highlighted: shapes.selectedShape === name
                    onClicked: shapes.selectedShape = name
                }

                ToolButton {
                    property string name: "triangle"
                    Layout.alignment: Qt.AlignHCenter
                    icon.source: "qrc:res/triangle_icon.png"
                    highlighted: shapes.selectedShape === name
                    onClicked: shapes.selectedShape = name
                }
            }
        }

        GroupBox {
            title: qsTr("Classes")
            Layout.fillWidth: true

            ColumnLayout {
                anchors.fill: parent

                ComboBox {
                    id: classcombobox

                    Layout.alignment: Qt.AlignHCenter

                    editable: true

                    onAccepted: {
                        if (currentIndex !== -1)
                            model.set(currentIndex, {"text": editText})
                    }

                    model: ListModel {
                        id: classesModel

                        ListElement {
                            text: qsTr("New class")
                            color: "red"
                        }
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
                                color: model.color
                                width: classcombobox.width * 0.2
                                height: parent.height
                            }
                            Text {
                                text: model.get(index).text
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
                        text: qsTr("Edit Color")

                        onClicked: colorDialog.open()
                    }

                    Button {
                        Layout.preferredWidth: parent.width / 2
                        text: qsTr("New Class")

                        onClicked: {
                            let current = classcombobox.model.count
                            classcombobox.model.append({text: qsTr("New class") + " (" + current + ")",
                                                        color: '#'+Math.floor(Math.random()*16777215).toString(16) })
                        }
                    }
                }
            }
        }

        GroupBox {
            title: qsTr("Edit")
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent
                Button {
                    Layout.preferredWidth: parent.width / 2
                    text: qsTr("Erase")
                }

                Button {
                    Layout.preferredWidth: parent.width / 2
                    text: qsTr("Undo")
                    onClicked: itemviewer.canvas.clearCanvas()
                }
            }
        }
    }
}
