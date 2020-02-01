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
Serializer::Serializer(const QString& filepath) :
    m_filepath(filepath)
{
}

Serializer::Serializer(const QVector<Polygon> items) :
    m_items(items)
{
}

QVector<Polygon> Serializer::read(marK::OutputType output_type)
{
    QVector<Polygon> objects;

    if(output_type == marK::OutputType::XML)
        objects = this->readXML();

    else if (output_type == marK::OutputType::JSON)
        objects = this->readJSON();

    return objects;
}

QString Serializer::serialize(marK::OutputType output_type)
{
    if (output_type == marK::OutputType::XML)
        return this->toXML();

    else if (output_type == marK::OutputType::JSON)
        return this->toJSON();

    return nullptr;
}

QString Serializer::toXML()
{
    if (m_items.isEmpty())
        return nullptr;

    QString xmldoc;
    QXmlStreamWriter xmlWriter(&xmldoc);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("annotation");

    for (const Polygon& item : m_items) {
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

QString Serializer::toJSON()
{
    if(m_items.isEmpty())
        return nullptr;

    QJsonArray classesArray;

    for (const Polygon& item : m_items) {
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

QVector<Polygon> Serializer::readJSON()
{
    QByteArray data = getData();

    QJsonParseError errorptr;
    QJsonDocument doc = QJsonDocument::fromJson(data, &errorptr);
    if (doc.isNull()) {
        qDebug() << errorptr.errorString();
    }

    QJsonArray polygonArray = doc.array();
    QVector<Polygon> savedPolygons;

    for (const QJsonValue& classObj : polygonArray)
    {
        Polygon polygon;
        MarkedClass polygonClass(classObj.toObject().value("Class").toString());

        polygon.setPolygonClass(&polygonClass);
        QJsonArray polygonArray = classObj.toObject().value("Polygon").toArray();

        for (const QJsonValue& polygonObj : polygonArray)
        {
            auto ptObj = polygonObj.toObject().value("pt").toObject();

            double x = ptObj.value("x").toString().toDouble();
            double y = ptObj.value("y").toString().toDouble();

            polygon.append(QPointF(x,y));
        }

        savedPolygons.append(polygon);
    }

    return savedPolygons;
}

QVector<Polygon> Serializer::readXML()
{
    QVector<Polygon> savedPolygons;
    return savedPolygons;
}

QByteArray Serializer::getData()
{
    m_filepath.replace(QRegularExpression(".jpg|.png|.xpm"), ".json");

    QFile file(m_filepath);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QByteArray data = file.readAll();
    file.close();

    return data;
}
