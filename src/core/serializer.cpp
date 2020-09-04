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

    if (dynamic_cast<const Sentence*>(objects.first())) {
        for (const MarkedObject* item : objects) {
            const Sentence *sentence = static_cast<const Sentence*>(item);
            xmlWriter.writeStartElement("object");

            xmlWriter.writeStartElement("class");
            xmlWriter.writeCharacters(sentence->objClass()->name());
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement(sentence->unitName());

            xmlWriter.writeStartElement("x");
            xmlWriter.writeCharacters(QString::number(sentence->begin()));
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("y");
            xmlWriter.writeCharacters(QString::number(sentence->end()));
            xmlWriter.writeEndElement();

            xmlWriter.writeEndElement();
            xmlWriter.writeEndElement();
        }
    }
    else if (dynamic_cast<const Polygon*>(objects.first())) {
        for (const MarkedObject* item : objects) {
            const Polygon *polygon = static_cast<const Polygon*>(item);
            xmlWriter.writeStartElement("object");

            xmlWriter.writeStartElement("class");
            xmlWriter.writeCharacters(polygon->objClass()->name());
            xmlWriter.writeEndElement();
            xmlWriter.writeStartElement(polygon->type());
            for (int i = 0; i < polygon->size(); ++i) {
                xmlWriter.writeStartElement(polygon->unitName());

                xmlWriter.writeStartElement("x");
                xmlWriter.writeCharacters(QString::number(polygon->at(i).x()));
                xmlWriter.writeEndElement();

                xmlWriter.writeStartElement("y");
                xmlWriter.writeCharacters(QString::number(polygon->at(i).y()));
                xmlWriter.writeEndElement();

                xmlWriter.writeEndElement();
            }
            xmlWriter.writeEndElement();
            xmlWriter.writeEndElement();
        }
    }

    xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    return xmldoc;
}

QString Serializer::toJSON(const QVector<MarkedObject*>& objects)
{
    if (objects.isEmpty())
        return nullptr;

    QJsonArray classesArray;

    if (dynamic_cast<const Sentence*>(objects.first())) {
        for (const MarkedObject* item : objects) {
            const Sentence *sentence = static_cast<const Sentence*>(item);
            QJsonObject recordObject;
            recordObject.insert("Class", sentence->objClass()->name());

            QJsonObject unitObj;
            unitObj.insert("x", QString::number(sentence->begin()));
            unitObj.insert("y", QString::number(sentence->end()));

            recordObject.insert(sentence->unitName(), unitObj);
            classesArray.push_back(recordObject);
        }
    }
    else if (dynamic_cast<const Polygon*>(objects.first())) {
        for (const MarkedObject* item : objects) {
            const Polygon *polygon = static_cast<const Polygon*>(item);
            QJsonObject recordObject;

            recordObject.insert("Class", polygon->objClass()->name());

            QJsonArray objectsArray;
            QJsonObject markedObj;

            for (int i = 0; i < polygon->size(); ++i) {
                QJsonObject unitObj;

                unitObj.insert("x", QString::number(polygon->at(i).x()));
                unitObj.insert("y", QString::number(polygon->at(i).y()));

                markedObj.insert(polygon->unitName(), unitObj);
                objectsArray.push_back(markedObj);
            }
            recordObject.insert(polygon->type(), objectsArray);

            classesArray.push_back(recordObject);
        }
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
        if (classObj["st"] != QJsonValue::Undefined) {
            QJsonObject unitObj = classObj["st"].toObject();
            double x = unitObj.value("x").toString().toDouble();
            double y = unitObj.value("y").toString().toDouble();

            object = new Sentence(markedClass, x, y);
        }
        else if (classObj[Polygon::type()] != QJsonValue::Undefined) {
            Polygon *pol = new Polygon(markedClass);
            QJsonArray typeArray = classObj[Polygon::type()].toArray();

            for (const QJsonValue& typeObj : qAsConst(typeArray)) {
                QJsonObject unitObj = typeObj[pol->unitName()].toObject();
                double x = unitObj.value("x").toString().toDouble();
                double y = unitObj.value("y").toString().toDouble();

                pol->append(QPointF(x, y));
            }
            object = new Polygon(pol);
        }
        else {
            savedObjects.clear();
            break;
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
            if (xmlReader.name() == "st") {
                xmlReader.readNextStartElement();
                double x = xmlReader.readElementText().toDouble();

                xmlReader.readNextStartElement();
                double y = xmlReader.readElementText().toDouble();

                object = new Sentence(markedClass, x, y);
                xmlReader.readNextStartElement();
                xmlReader.readNextStartElement();
            }
            else if (xmlReader.name() == Polygon::type()) {
                Polygon *pol = new Polygon(markedClass);

                xmlReader.readNextStartElement();
                while (xmlReader.name() == pol->unitName()) {
                    xmlReader.readNextStartElement();
                    double x = xmlReader.readElementText().toDouble();

                    xmlReader.readNextStartElement();
                    double y = xmlReader.readElementText().toDouble();

                    pol->append(QPointF(x, y));
                    xmlReader.readNextStartElement();
                    xmlReader.readNextStartElement();
                }
                object = new Polygon(pol);
            }
            else {
                savedObjects.clear();
                break;
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
