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
#include <QPair>
#include <QtGlobal>
#include <QXmlStreamWriter>

QVector<MarkedObject*> Serializer::read(const QString& filename)
{
    QVector<MarkedObject*> objects;

    if (QFile::exists(filename)) {
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

    return QString();
}

QString Serializer::toXML(const QVector<MarkedObject*>& objects)
{
    if (objects.isEmpty())
        return QString();

    QString xmldoc;
    QXmlStreamWriter xmlWriter(&xmldoc);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("annotation");

    MarkedObject::Type objType = objects.first()->type();
    QVector<QPair<QString, QString>> values;
    for (const MarkedObject* object : objects) {
        xmlWriter.writeStartElement("object");

        xmlWriter.writeStartElement("class");
        xmlWriter.writeCharacters(object->objClass()->name());
        xmlWriter.writeEndElement();

        if (objType == MarkedObject::Type::Sentence) {
            const Sentence *sentence = static_cast<const Sentence*>(object);
            values << qMakePair(QString::number(sentence->begin()), QString::number(sentence->end()));
        }
        else if (objType == MarkedObject::Type::Polygon) {
            const Polygon *polygon = static_cast<const Polygon*>(object);
            for (int i = 0; i < polygon->size(); i++)
                values << qMakePair(QString::number(polygon->at(i).x()), QString::number(polygon->at(i).y()));

            xmlWriter.writeStartElement("Polygon");
        }

        for (const auto pair : values) {
            xmlWriter.writeStartElement(object->unitName());

            xmlWriter.writeStartElement("x");
            xmlWriter.writeCharacters(pair.first);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("y");
            xmlWriter.writeCharacters(pair.second);
            xmlWriter.writeEndElement();

            xmlWriter.writeEndElement();
        }

        if (objType == MarkedObject::Type::Polygon)
            xmlWriter.writeEndElement();
        xmlWriter.writeEndElement();
        values.clear();
    }

    xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    return xmldoc;
}

QString Serializer::toJSON(const QVector<MarkedObject*>& objects)
{
    if (objects.isEmpty())
        return QString();

    QJsonArray classesArray;
    QVector<QPair<QString, QString>> values;
    MarkedObject::Type objType = objects.first()->type();
    bool isPolygon = objType == MarkedObject::Type::Polygon;

    for (const MarkedObject* object : objects) {
        QJsonArray objectsArray;
        QJsonObject unitObj;
        QJsonObject markedObj;
        QJsonObject recordObject;
        recordObject.insert("Class", object->objClass()->name());

        if (objType == MarkedObject::Type::Sentence) {
            const Sentence *sentence = static_cast<const Sentence*>(object);
            values << qMakePair(QString::number(sentence->begin()), QString::number(sentence->end()));
        }
        else if (isPolygon) {
            const Polygon *polygon = static_cast<const Polygon*>(object);
            for (int i = 0; i < polygon->size(); i++)
                values << qMakePair(QString::number(polygon->at(i).x()), QString::number(polygon->at(i).y()));
        }
        for (const auto pair : values) {
            unitObj.insert("x", pair.first);
            unitObj.insert("y", pair.second);

            if (isPolygon) {
                markedObj.insert(object->unitName(), unitObj);
                objectsArray.push_back(markedObj);
            }
            else
                recordObject.insert(object->unitName(), unitObj);
        }
        if (isPolygon)
            recordObject.insert("Polygon", objectsArray);

        classesArray.push_back(recordObject);
        values.clear();
    }

    QJsonDocument doc(classesArray);

    return doc.toJson();
}

QVector<MarkedObject*> Serializer::readJSON(const QString& filename)
{
    QVector<MarkedObject*> savedObjects;
    QMap<QString, MarkedClass*> markedClasses;
    QVector<QPair<double, double>> values;

    QByteArray data = getData(filename);
    QJsonDocument doc = QJsonDocument::fromJson(data);

    QJsonArray objectArray = doc.array();

    MarkedObject::Type objectType;
    if (objectArray.first()["st"] != QJsonValue::Undefined)
        objectType = MarkedObject::Type::Sentence;
    else if (objectArray.first()["Polygon"] != QJsonValue::Undefined)
        objectType = MarkedObject::Type::Polygon;
    else
        return QVector<MarkedObject*>();

    for (const QJsonValue& jsonObj : qAsConst(objectArray)) {
        QString className = jsonObj["Class"].toString();
        MarkedClass *markedClass = markedClasses.value(className, nullptr);
        if (!markedClass) {
            markedClass = new MarkedClass(className);
            markedClasses.insert(className, markedClass);
        }

        QVector<QJsonObject> objectArray;
        if (objectType == MarkedObject::Type::Sentence)
            objectArray << jsonObj["st"].toObject();
        else if (objectType == MarkedObject::Type::Polygon) {
            for (const QJsonValue& unitObj : jsonObj["Polygon"].toArray())
                objectArray << unitObj["pt"].toObject();
        }

        for (const QJsonObject& unitObj : objectArray) {
            double x = unitObj.value("x").toString().toDouble();
            double y = unitObj.value("y").toString().toDouble();
            values << qMakePair(x, y);
        }

        MarkedObject* object;
        if (objectType == MarkedObject::Type::Sentence)
            object = new Sentence(markedClass, values.first().first, values.first().second);
        else if (objectType == MarkedObject::Type::Polygon)
            object = new Polygon(markedClass, values);

        savedObjects.append(object);
        values.clear();
    }

    return savedObjects;
}

QVector<MarkedObject*> Serializer::readXML(const QString& filename)
{
    QVector<MarkedObject*> savedObjects;
    QMap<QString, MarkedClass*> markedClasses;
    QVector<QPair<double, double>> values;

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

            MarkedObject::Type objectType;
            QString unitName;
            if (xmlReader.name() == "Polygon") {
                objectType = MarkedObject::Type::Polygon;
                unitName = "pt";
            }
            else if (xmlReader.name() == "st") {
                objectType = MarkedObject::Type::Sentence;
                unitName = "st";
            }
            else {
                savedObjects.clear();
                break;
            }
            while (xmlReader.name() != "object") {
                if (xmlReader.name() == unitName) {
                    xmlReader.readNextStartElement();
                    double x = xmlReader.readElementText().toDouble();
                    xmlReader.readNextStartElement();
                    double y = xmlReader.readElementText().toDouble();
                    xmlReader.skipCurrentElement();

                    values << qMakePair(x, y);
                }
                xmlReader.readNextStartElement();
            }

            MarkedObject* object;
            if (objectType == MarkedObject::Type::Sentence)
                object = new Sentence(markedClass, values.first().first, values.first().second);
            else if (objectType == MarkedObject::Type::Polygon)
                object = new Polygon(markedClass, values);

            savedObjects << object;
            values.clear();
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
