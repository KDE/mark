/*************************************************************************
 *  Copyright (C) 2020 by Caio Jordão Carvalho <caiojcarvalho@gmail.com> *
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

#include "core/markedobject_p.h"
#include "image/polygon.h"

Polygon::Polygon(MarkedClass* objClass) :
    MarkedObject(std::make_unique<MarkedObjectPrivate>(), objClass)
{
    d_p->m_objClass = objClass;
}

Polygon::Polygon(const Polygon* pol) :
    MarkedObject (std::make_unique<MarkedObjectPrivate>(), pol->objClass())
{
    for (const QPointF& point : *pol)
        append(point);
}

Polygon::Polygon(MarkedClass* objClass, const QPolygonF& polygon) :
    MarkedObject (std::make_unique<MarkedObjectPrivate>(), objClass)
{
    for (const auto& point : polygon)
        append(point);
}

void Polygon::clear()
{
    QPolygonF::clear();
}

QString Polygon::unitName() const
{
    return "pt";
}

MarkedObject::Type Polygon::type()
{
    return MarkedObject::Type::Polygon;
}

void Polygon::scale(const QPointF offset, const qreal scaleW, const qreal scaleH)
{
    for (QPointF& point : *this) {
        point -= offset;
        point = QPointF(point.x() / scaleW, point.y() / scaleH);
    }
}

void Polygon::unscale(const QPointF offset, const qreal scaleW, const qreal scaleH)
{
    for (QPointF& point : *this) {
        point = QPointF(point.x() * scaleW, point.y() * scaleH);
        point += offset;
    }
}
