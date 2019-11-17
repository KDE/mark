import QtQuick 2.12
import QtQuick.Controls 2.5
import Qt.labs.folderlistmodel 2.12

GroupBox {
    anchors.fill: parent
    anchors.margins: 5
    title: qsTr("Files")

    ListView {
        id: itemlistview

        anchors.fill: parent

        // TODO: implement my own folderlistmodel with the possibility of adding images
        FolderListModel {
            id: folderModel

            folder: filedialog.fileUrl
            sortField: FolderListModel.Name

            nameFilters: ["*.png", "*.jpg", "*.jpeg"]
        }

        Component {
            id: fileDelegate

            MouseArea {
                id: mousearea
                height: 20
                width: itemlistview.width - scrollbar.width - 10

                Item {
                    anchors.fill: parent
                    Column {
                        leftPadding: 5
                        Text {
                            width: mousearea.width - 10
                            height: mousearea.height
                            text: fileName
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }
                    }
                }

                onClicked: {
                    itemlistview.currentIndex = index
                    itemviewer.canvas.image.source = "file:" + filePath
                }
            }
        }

        ScrollBar.vertical: ScrollBar { id: scrollbar; policy: ScrollBar.AlwaysOn }

        model: folderModel
        delegate: fileDelegate
        highlight: Rectangle { color: "grey" ; radius: 5 }
        highlightFollowsCurrentItem: true
        //displayMarginBeginning: 2
        //displayMarginEnd: 2
        spacing: 3
        currentIndex: -1
    }
}
