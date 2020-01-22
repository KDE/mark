#include "serializer.h"

#include <QDebug>
#include <QPointF>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QXmlStreamWriter>

QString Serializer::toXML(const QVector<Polygon>& annotatedPolygons)
{
    if (annotatedPolygons.isEmpty())
        return nullptr;

    QString xmldoc;
    QXmlStreamWriter xmlWriter(&xmldoc);

    xmlWriter.writeStartElement("annotation");

    for (const Polygon& item : annotatedPolygons) {
        xmlWriter.writeStartElement("object");

        xmlWriter.writeStartElement("class");
        xmlWriter.writeCharacters(item.polygonClass()->name());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("polygon");

        for (const QPointF& point : item) {
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

QString Serializer::toJSON(const QVector<Polygon>& annotatedPolygons)
{
    if(annotatedPolygons.isEmpty())
        return nullptr;

    QJsonArray classesArray;

    for (const Polygon& item : annotatedPolygons) {
        QJsonObject recordObject;

        recordObject.insert("Class", item.polygonClass()->name());

        QJsonObject polygonObj;
        QJsonArray polygonsArray;

        for (const QPointF &point : item) {
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
