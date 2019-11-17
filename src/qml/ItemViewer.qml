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
        property var selectedColor: actionBox.currentColor

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
            hoverEnabled: false

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
            ctx.strokeStyle = selectedColor
            ctx.beginPath()

            if (lastX > 0 && lastY > 0)
                ctx.moveTo(lastX, lastY)

            lastX = area.mouseX
            lastY = area.mouseY

            ctx.lineTo(lastX, lastY)
            ctx.stroke()

            if (lastX > 0 && lastY > 0) {
                polygons.append({"x": lastX, "y": lastY})
                console.log(polygons.get(polygons.count - 1)["x"] + " " + polygons.get(polygons.count - 1)["y"])
            }
        }

        property var clearCanvas: function() {

            console.log("Here")

            var ctx = getContext("2d")
            ctx.reset()

            if (polygons.count > 0)
                polygons.remove(polygons.count - 1)

            lastX = -1
            lastY = -1

            for (var i = 0; i < polygons.count; i++) {
                canvas.requestPaint()
                var ctx = getContext("2d")
                ctx.lineWidth = 4
                ctx.strokeStyle = "#000000"
                ctx.beginPath()

                if (lastX > 0 && lastY > 0)
                    ctx.moveTo(lastX, lastY)

                ctx.lineTo(polygons.get(i)["x"], polygons.get(i)["y"])
                ctx.stroke()
                
                lastX = polygons.get(i)["x"]
                lastY = polygons.get(i)["y"]
            }
        }
    }
}
