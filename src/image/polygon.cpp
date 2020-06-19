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

#include "polygon.h"

Polygon::Polygon(MarkedClass* objClass) :
    MarkedObject(std::make_unique<MarkedObjectPrivate>(), objClass)
{
    d_p->m_objClass = objClass;
}

Polygon::Polygon(const MarkedObject* pol) :
    MarkedObject (std::make_unique<MarkedObjectPrivate>(), pol->objClass())
{
    const Polygon* p = static_cast<const Polygon*>(pol);
    for (const QPointF& point : *p)
        QPolygonF::append(point);
}

void Polygon::clear()
{
    QPolygonF::clear();
}

void Polygon::append(double memberX, double memberY)
{
    QPolygonF::append(QPointF(memberX, memberY));
}

int Polygon::size() const
{
    return QPolygonF::size();
}

QString Polygon::unitName() const
{
    return "pt";
}

QString Polygon::type() const
{
    return "Polygon";
}
QString Polygon::memberX() const
{
    return "x";
}

QString Polygon::memberY() const
{
    return "y";
}

qreal Polygon::XValueOf(int element) const
{
    return this->at(element).x();
}

qreal Polygon::YValueOf(int element) const
{
    return this->at(element).y();
}
