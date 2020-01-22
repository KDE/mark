#include "serializer.h"

#include <QDebug>
#include <QPointF>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QXmlStreamWriter>

QString Serializer::toXML(QVector<Polygon> annotatedPolygons)
{
    if (annotatedPolygons.isEmpty())
        return nullptr;

    QString xmldoc;
    QXmlStreamWriter xmlWriter(&xmldoc);

    xmlWriter.writeStartElement("annotation");

    for (Polygon item : annotatedPolygons) {
        xmlWriter.writeStartElement("object");

        xmlWriter.writeStartElement("class");
        xmlWriter.writeCharacters(item.polygonClass()->name());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("polygon");

        for (QPointF &point : item) {
            xmlWriter.writeStartElement("pt");

            xmlWriter.writeStartElement("x");

            xmlWriter.writeCharacters(QString::number(point.x()));

            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("y");

            xmlWriter.writeCharacters(QString::number(point.y()));

            xmlWriter.writeEndElement();

            xmlWriter.writeEndElement();
        }

        xmlWriter.writeEndElement();

        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();

    xmlWriter.writeEndDocument();

    return xmldoc;
}

QString Serializer::toJSON(QVector<Polygon> annotatedPolygons)
{
    if(annotatedPolygons.isEmpty())
        return nullptr;

    QJsonArray classesArray;

    for (Polygon item : annotatedPolygons) {
        QJsonObject recordObject;

        recordObject.insert("Class", item.polygonClass()->name());

        QJsonObject polygonObj;
        QJsonArray polygonsArray;

        for (QPointF &point : item) {
            QJsonObject ptObj;

            ptObj.insert("x", QString::number(point.x()));
            ptObj.insert("y", QString::number(point.y()));

            polygonObj.insert("pt", ptObj);
            polygonsArray.push_back(polygonObj);
        }

        recordObject.insert("Polygon", polygonsArray);

        classesArray.push_back(recordObject);
    }

    QJsonDocument doc(classesArray);

    return doc.toJson();
}
