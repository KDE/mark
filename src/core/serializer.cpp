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

#include "core/markedclass.h"
#include "core/markedobject_p.h"
#include "core/serializer.h"
#include "image/polygon.h"
#include "text/sentence.h"

#include <memory>

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QRegularExpression>
#include <QtGlobal>
#include <QXmlStreamWriter>

QVector<MarkedObject*> Serializer::read(const QString& filename)
{
    QVector<MarkedObject*> objects;

    bool fileExists = QFile::exists(filename);
    if (fileExists) {
        if (filename.endsWith(".xml"))
            objects = readXML(filename);
        else if (filename.endsWith(".json"))
            objects = readJSON(filename);
    }

    return objects;
}

QString Serializer::serialize(const QVector<MarkedObject*>& objects, OutputType output_type)
{
    if (output_type == OutputType::XML)
        return toXML(objects);

    else if (output_type == OutputType::JSON)
        return toJSON(objects);

    return nullptr;
}

QString Serializer::toXML(const QVector<MarkedObject*>& objects)
{
    if (objects.isEmpty())
        return nullptr;


    QString xmldoc;
    QXmlStreamWriter xmlWriter(&xmldoc);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("annotation");

    for (const MarkedObject* item : objects) {
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

QString Serializer::toJSON(const QVector<MarkedObject*>& objects)
{
    if (objects.isEmpty())
        return nullptr;

    QJsonArray classesArray;

    for (const MarkedObject* item : objects) {
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
    QVector<MarkedObject*> savedObjects;
    QMap<QString, MarkedClass*> markedClasses;

    QByteArray data = getData(filename);
    QJsonDocument doc = QJsonDocument::fromJson(data);

    QJsonArray objectArray = doc.array();

    for (const QJsonValue& classObj : qAsConst(objectArray)) {
        QString className = classObj["Class"].toString();
        MarkedClass *markedClass = markedClasses.value(className, nullptr);
        if (!markedClass) {
            markedClass = new MarkedClass(className);
            markedClasses.insert(className, markedClass);
        }

        MarkedObject* object;
        if (classObj["Polygon"] != QJsonValue::Undefined)
            object = new Polygon(markedClass);
        else if (classObj["Sentence"] != QJsonValue::Undefined)
            object = new Sentence(markedClass);
        else
            return QVector<MarkedObject*>();

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
    QMap<QString, MarkedClass*> markedClasses;

    QByteArray data = getData(filename);

    QXmlStreamReader xmlReader(data);
    xmlReader.readNextStartElement();

    while (!xmlReader.atEnd()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartElement) {
            xmlReader.readNextStartElement();

            QString className = xmlReader.readElementText();
            MarkedClass *markedClass = markedClasses.value(className, nullptr);
            if (!markedClass) {
                markedClass = new MarkedClass(className);
                markedClasses.insert(className, markedClass);
            }
            xmlReader.readNextStartElement();

            MarkedObject* object;
            if (xmlReader.name() == "Polygon")
                object = new Polygon(markedClass);
            else if (xmlReader.name() == "Sentence")
                object = new Sentence(markedClass);
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

bool Serializer::write(const QString &filepath, const QVector<MarkedObject*>& objects, OutputType output_type)
{
    if (!objects.isEmpty()) {
        QString filename = QString(filepath);
        // this part needs improvement
        filename.replace(QRegularExpression(".jpg|.jpeg|.png|.xpm|.txt"), (output_type == OutputType::XML ? ".xml" : ".json"));

        QString document = serialize(objects, output_type);

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

const char* Serializer::filterString(OutputType output_type)
{
    if (output_type == OutputType::XML)
        return "XML files (*.xml *.XML)";
    else if (output_type == OutputType::JSON)
        return "JSON files (*.json *.JSON)";
    return "";
}
