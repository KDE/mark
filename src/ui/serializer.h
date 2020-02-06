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

#include "image/polygon.h"
#include "mark.h"

#include <QVector>
#include <QString>
#include <QPointF>

class Serializer
{
public:
    explicit Serializer(const QString& filepath);
    explicit Serializer(const QVector<Polygon> items);

    bool write(const QString &filepath, marK::OutputType output_type);
    QString writeTempFile(const QString &originalFileName);

    QVector<Polygon> read(marK::OutputType output_type);
    QVector<Polygon> readTempFile();

private:
    QByteArray getData();
    QString toJSON();
    QString toXML();

    QVector<Polygon> readJSON();
    QVector<Polygon> readXML();

    QString serialize(marK::OutputType output_type);
    QString getTempFileName(const QString &filepath);
    QString handleFileNameExtension(const QString &str);

private:
    // put it to work with MarkedObject
    QVector<Polygon> m_items;
    QString m_filepath;
    marK::OutputType m_output_type;
};
#endif // SERIALIZER_H
