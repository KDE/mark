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

#include "ui/serializer.h"
#include "ui/markedclass.h"
#include "ui/markedobject_p.h"
#include "image/polygon.h"

#include <QtGlobal>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QXmlStreamWriter>
#include <QFile>
#include <QDir>
#include <QRegularExpression>
#include <memory>

Serializer::Serializer(QVector<MarkedClass*>& markedClasses) :
    m_markedClasses(markedClasses)
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

QString Serializer::serialize(OutputType output_type)
{
    if (output_type == OutputType::XML)
        return this->toXML();

    else if (output_type == OutputType::JSON)
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
        QString className = classObj["Class"].toString();
        MarkedClass* objClass = getMarkedClass(className);

        auto object = new Polygon(objClass);
        QJsonArray typeArray = classObj[object->type()].toArray();

        for (const QJsonValue& typeObj : qAsConst(typeArray)) {
            QJsonObject unitObj = typeObj[object->unitName()].toObject();

            // without toString() the value is 0
            double memberX = unitObj.value(object->memberX()).toString().toDouble();
            double memberY = unitObj.value(object->memberY()).toString().toDouble();

            object->append(memberX, memberY);
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
    xmlReader.readNextStartElement();

    while (!xmlReader.atEnd()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartElement) {
            xmlReader.readNextStartElement();

            QString className = xmlReader.readElementText();
            MarkedClass* markedClass = getMarkedClass(className);

            xmlReader.readNextStartElement();

            MarkedObject* object;
            // add new types later on
            // not sure about this, needs improvement
            if (xmlReader.name() == "Polygon")
                object = new Polygon(markedClass);

            else
                return QVector<MarkedObject*>();

            xmlReader.readNextStartElement();

            while (xmlReader.name() == object->unitName()) {
                xmlReader.readNextStartElement();

                double memberX = xmlReader.readElementText().toDouble();

                xmlReader.readNextStartElement();

                double memberY = xmlReader.readElementText().toDouble();

                object->append(memberX, memberY);

                xmlReader.readNextStartElement();

                xmlReader.readNextStartElement();
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

MarkedClass* Serializer::getMarkedClass(const QString& className)
{
    for (MarkedClass* markedClass : m_markedClasses) {
        if (markedClass->name() == className)
            return markedClass;
    }

    MarkedClass* mClass = new MarkedClass(className);
    m_markedClasses << mClass;

    return mClass;
}

bool Serializer::write(const QString &filepath, OutputType output_type)
{
    if (!m_items.isEmpty()) {
        QString filename = QString(filepath);
        // this part needs improvement
        filename.replace(QRegularExpression(".jpg|.jpeg|.png|.xpm|.txt"), (output_type == OutputType::XML ? ".xml" : ".json"));

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
