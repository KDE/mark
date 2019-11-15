import QtQuick 2.0
import QtQml.Models 2.13

Rectangle
{
    anchors.fill: parent
    anchors.centerIn: parent

    border.color: "#BDBDBD"
    border.width: 1

    property alias canvas: canvas

    ListModel {
        id: polygons
    }

    Canvas {
        id: canvas
        z: 10
        renderStrategy: Canvas.Immediate

        anchors.fill: parent

        property int lastX: -1
        property int lastY: -1

        contextType: "2d"

        property alias image: image

        Image {
            id: image
            anchors.centerIn: parent
            source: ""
            z: -1
        }

        MouseArea {
            id: area
            anchors.fill: parent

            onClicked: {
                canvas.lastX = mouseX
                canvas.lastY = mouseY
            }

            onPositionChanged: {
                canvas.requestPaint()
            }
        }

        onPaint: {
            var ctx = getContext("2d")
            ctx.lineWidth = 4
            ctx.strokeStyle = "#000000"
            ctx.beginPath()

            if (lastX > 0 && lastY > 0)
                ctx.moveTo(lastX, lastY)

            lastX = area.mouseX
            lastY = area.mouseY

            ctx.lineTo(lastX, lastY)
            ctx.stroke()

            polygons.append({"x": lastX, "y": lastY})

            console.log(polygons.get(polygons.count - 1)["x"] + " " + polygons.get(polygons.count - 1)["y"])

        }
    }
}
