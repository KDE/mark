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

#include <QVector>
#include <QString>
#include <QPointF>

class Serializer
{
public:
    static QString toJSON(const QVector<Polygon>& annotatedPolygons);
    static QString toXML(const QVector<Polygon>& annotatedPolygons);
    static QVector<Polygon> readJSON(const QByteArray& data, const QPointF& offset);
    //static QVector<Polygon> readXML(const QByteArray& data, const QPointF& offset);
    static QByteArray getData(const QString& filepath);
};
#endif // SERIALIZER_H
