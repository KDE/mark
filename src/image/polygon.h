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

#ifndef POLYGON_H
#define POLYGON_H

#include "core/markedobject.h"
#include "core/markedclass.h"

#include <QPolygonF>

/** Class that represent the annotated data of image annotation. */
class Polygon : public MarkedObject, public QPolygonF
{
public:
    /** Create a Polygon, if given MarkedClass, assign it.
     * @param objClass - MarkedClass instance to assign.
     */
    explicit Polygon(MarkedClass* objClass = nullptr);

    /** Create a Polygon copying pol.
     * @param pol - Polygon to copy.
     */
    Polygon(const Polygon* pol);

    /** Create a Polygon with given MarkedClass and copying given polygon.
     * @param objClass - MarkedClass instance to assign.
     * @param values - Polygon to copy from.
     */
    Polygon(MarkedClass* objClass, const QPolygonF& polygon);

    void clear() override;
    QString unitName() const override;
    MarkedObject::Type type() override;
};

#endif // POLYGON_H
