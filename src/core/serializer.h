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

#include "core/markedobject.h"

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

    /** Write annotation to a file, the path is changed accordingly to the output_type.
     * @param filepath - path of the file, its extension is changed accordingly to output_type.
     * @param output_type - type of output to save.
     */
    static bool write(const QString& filepath, const QVector<MarkedObject*>& objects, OutputType output_type);

    /** Read given file and return the annotated objects inside it.
     * @param filepath - path of the file to load.
     */
    static QVector<MarkedObject*> read(const QString& filepath);

private:
    /** Read given file.
     * @return raw data readen.
     * @param filepath - path of the file.
     */
    static QByteArray getData(const QString& filepath);

    /** Iterate through annnotated data and create a JSON document.
     * @return JSON document created.
     */
    static QString toJSON(const QVector<MarkedObject*>& objects);

    /** Iterate through annnotated data and create a XML document.
     * @return XML document created.
     */
    static QString toXML(const QVector<MarkedObject*>& objects);

    /** Read given JSON document and create MarkedObject's objects accordingly.
     * @return created objects.
     * @param filepath - path of the JSON document.
     */
    static QVector<MarkedObject*> readJSON(const QString& filepath);

    /** Read given XML document and create MarkedObject's objects accordingly.
     * @return created objects.
     * @param filepath - path of the XML document.
     */
    static QVector<MarkedObject*> readXML(const QString& filepath);

    /** Turns annotated objects into the file format of given OutputType.
     * @param outputType - file format to serialize the annotated objects.
     */
    static QString serialize(const QVector<MarkedObject*>& objects, OutputType outputType);
};
#endif // SERIALIZER_H
