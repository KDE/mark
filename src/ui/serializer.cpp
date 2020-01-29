/*************************************************************************
 *  Copyright (C) 2020 by Jean Lima Andrade <jeno.andrade@gmail.com>     *
 *                                                                       *
 *  This program is free software; you can redistribute it and/or        *
 *  modify it under the terms of the GNU General Public License as       *
 *  published by the Free Software Foundation; either version 3 of       *
 *  the License, or (at your option) any later version.                  *
 *                                                                       *
 *  This program is distributed in the hope that it will be useful,      *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 *************************************************************************/

#include "serializer.h"
#include "markedclass.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QXmlStreamWriter>
#include <QFile>
#include <QRegularExpression>

QString Serializer::toXML(const QVector<Polygon>& annotatedPolygons)
{
    if (annotatedPolygons.isEmpty())
        return nullptr;

    QString xmldoc;
    QXmlStreamWriter xmlWriter(&xmldoc);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

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

QVector<Polygon> Serializer::readJSON(const QByteArray& data, const QPointF& offset)
{
    QJsonParseError errorptr;
    QJsonDocument doc = QJsonDocument::fromJson(data, &errorptr);
    if (doc.isNull()) {
        qDebug() << errorptr.errorString();
    }

    QJsonArray polygonArray = doc.array();
    QVector<Polygon> savedPolygons;

    for (const auto& classObj : polygonArray)
    {
        Polygon polygon;
        MarkedClass polygonClass(classObj.toObject().value("Class").toString());

        polygon.setPolygonClass(&polygonClass);
        QJsonArray polygonArray = classObj.toObject().value("Polygon").toArray();

        for (const auto& polygonObj : polygonArray)
        {
            auto ptObj = polygonObj.toObject().value("pt").toObject();

            double x = ptObj.value("x").toString().toDouble();
            double y = ptObj.value("y").toString().toDouble();

            polygon.append(QPointF(x,y) + offset);
        }

        savedPolygons.append(polygon);
    }

    return savedPolygons;
}
/*
QVector<Polygon> Serializer::readXML(const QByteArray& data, const QPointF& offset)
{
    QVector<Polygon> savedPolygons;
    return savedPolygons;
}
*/
QByteArray Serializer::getData(const QString& filepath)
{
    QString filename(filepath);
    filename.replace(QRegularExpression(".jpg|.png|.xpm"), ".json");

    QFile file(filename);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QByteArray data = file.readAll();
    file.close();

    return data;
}
