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
#include "image/polygon.h"

#include <QtGlobal>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QXmlStreamWriter>
#include <QFile>
#include <QDir>
#include <QRegularExpression>
#include <QPointF>

Serializer::Serializer()
{
}

Serializer::Serializer(const QVector<MarkedObject*>& items) :
    m_items(items)
{
}

QVector<MarkedObject*> Serializer::read(const QString& filename)
{
    QVector<MarkedObject*> objects;

    bool fileExists = QFile::exists(filename);
    if (fileExists) {
        if (filename.endsWith(".xml"))
            objects = this->readXML(filename);

        else if (filename.endsWith(".json"))
            objects = this->readJSON(filename);

    }

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

    for (const MarkedObject* item : m_items) {
        xmlWriter.writeStartElement("object");

        xmlWriter.writeStartElement("class");
        xmlWriter.writeCharacters(item->className());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement(item->type());

        for (int i = 0; i < item->size(); ++i) {
            xmlWriter.writeStartElement(item->unitName());

            xmlWriter.writeStartElement(item->memberX());

            xmlWriter.writeCharacters(QString::number(item->XValueOf(i)));

            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement(item->memberY());

            xmlWriter.writeCharacters(QString::number(item->YValueOf(i)));

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
    if (m_items.isEmpty())
        return nullptr;

    QJsonArray classesArray;

    for (const MarkedObject* item : m_items) {
        QJsonObject recordObject;

        recordObject.insert("Class", item->className());

        QJsonArray objectsArray;
        QJsonObject markedObj;

        for (int i = 0; i < item->size(); ++i) {
            QJsonObject unitObj;

            unitObj.insert(item->memberX(), QString::number(item->XValueOf(i)));

            unitObj.insert(item->memberY(), QString::number(item->YValueOf(i)));

            markedObj.insert(item->unitName(), unitObj);
            objectsArray.push_back(markedObj);
        }

        recordObject.insert(item->type(), objectsArray);

        classesArray.push_back(recordObject);
    }

    QJsonDocument doc(classesArray);

    return doc.toJson();
}

QVector<MarkedObject*> Serializer::readJSON(const QString& filename)
{
    QByteArray data = getData(filename);
    QJsonDocument doc = QJsonDocument::fromJson(data);

    QJsonArray objectArray = doc.array();
    QVector<MarkedObject*> savedObjects;

    for (const QJsonValue& classObj : qAsConst(objectArray)) {
        MarkedObject* object;
        //FIXME: do a verification, if there is an object with the same class name, do not create a new one
        auto objClass = new MarkedClass(classObj["Class"].toString());

        object = new Polygon(objClass);
        QJsonArray typeArray = classObj[object->type()].toArray();

        for (const QJsonValue& typeObj : qAsConst(typeArray)) {
            QJsonObject unitObj = typeObj[object->unitName()].toObject();

            double x = unitObj.value(object->memberX()).toString().toDouble();
            double y = unitObj.value(object->memberY()).toString().toDouble();

            // this part needs improvement, do something to remove QPointF, maybe a pair?
            object->append(QVariant(QPointF(x,y)));
        }

        savedObjects.append(object);
    }

    return savedObjects;
}

QVector<MarkedObject*> Serializer::readXML(const QString& filename)
{
    QVector<MarkedObject*> savedObjects;

    QByteArray data = getData(filename);

    QXmlStreamReader xmlReader(data);
    xmlReader.readNextStartElement(); // going to first element

    while (!xmlReader.atEnd()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartElement) {
            MarkedObject* object;
            MarkedClass* markedClass;
            xmlReader.readNextStartElement();

            if (xmlReader.name() == "class")
            {
                //FIXME: do a verification, if there is an object with the same class name, do not create a new one
                markedClass = new MarkedClass(xmlReader.readElementText());

                xmlReader.readNextStartElement();
                if (xmlReader.name() == "Polygon") {
                    object = new Polygon(markedClass);

                    xmlReader.readNextStartElement();

                    while (xmlReader.name() == object->unitName()) {
                        xmlReader.readNextStartElement();

                        double x = xmlReader.readElementText().toDouble();

                        xmlReader.readNextStartElement();

                        double y = xmlReader.readElementText().toDouble();

                        object->append(QVariant(QPointF(x, y)));

                        xmlReader.readNextStartElement();

                        xmlReader.readNextStartElement();
                    }

                }
            }
            savedObjects << object;
        }
    }

    return savedObjects;
}

QByteArray Serializer::getData(const QString& filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QByteArray data = file.readAll();
    file.close();

    return data;
}

bool Serializer::write(const QString &filepath, marK::OutputType output_type)
{
    if (!m_items.isEmpty()) {
        QString filename = QString(filepath);
        // this part needs improvement
        filename.replace(QRegularExpression(".jpg|.jpeg|.png|.xpm|.txt"), (output_type == marK::OutputType::XML ? ".xml" : ".json"));

        QString document = serialize(output_type);

        if (!document.isEmpty()) {
            QFile file(filename);
            bool fileOpened = file.open(QIODevice::WriteOnly|QIODevice::Text);
            if (fileOpened) {
                file.write(document.toUtf8());
                file.close();

                return true;
            }
        }
    }

    return false;
}
