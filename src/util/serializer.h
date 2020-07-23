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

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "ui/markedobject.h"

#include <QVector>
#include <QString>

/** Class responsible of writing and reading annotation data from files. */
class Serializer
{
public:
    /** Output format of annotation data. */
    enum class OutputType {
        None,
        XML,
        JSON
    };

public:
    /** Create a Serializer object.
     * @param markedClasses - Vector of MarkedClass to add and search for pointers.
     */
    explicit Serializer(QVector<MarkedClass*>* markedClasses);

    /** Create a Serializer object.
     * @param items - annotated objects.
     */
    explicit Serializer(const QVector<MarkedObject*>& items);

    /** Write annotation to a file, the path is changed accordingly to the output_type.
     * @param filepath - path of the file, its extension is changed accordingly to output_type.
     * @param output_type - type of output to save.
     */
    bool write(const QString& filepath, OutputType output_type);

    /** Read given file and return the annotated objects inside it.
     * @param filename - path of the file to load.
     */
    QVector<MarkedObject*> read(const QString& filename);

public:

    static const char* filterString(OutputType output_type);

private:
    /** Read given file.
     * @return raw data readen.
     * @param filename - path of the file.
     */
    QByteArray getData(const QString& filename);

    /** Iterate through annnotated data and create a JSON document.
     * @return JSON document created.
     */
    QString toJSON();

    /** Iterate through annnotated data and create a XML document.
     * @return XML document created.
     */
    QString toXML();

    /** Read given JSON document and create MarkedObject's objects accordingly.
     * @return created objects.
     * @param filename - path of the JSON document.
     */
    QVector<MarkedObject*> readJSON(const QString& filename);

    /** Read given XML document and create MarkedObject's objects accordingly.
     * @return created objects.
     * @param filename - path of the XML document.
     */
    QVector<MarkedObject*> readXML(const QString& filename);

    /** Turns annotated objects into the file format of given output_type.
     * @param output_type - file format to serialize the annotated objects.
     */
    QString serialize(OutputType output_type);

    /** @return the pointer of a markedClass, or a new one if none matches the className.
     * @param className - name of the markedClass.
     */
    MarkedClass* getMarkedClass(const QString& className);

private:
    QVector<MarkedObject*> m_items;
    QVector<MarkedClass*>* m_markedClasses;
};
#endif // SERIALIZER_H
