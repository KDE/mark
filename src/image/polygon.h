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

#include "markedobject.h"
#include "markedclass.h"

#include <QPolygonF>

class QGraphicsItem;

class Polygon : public MarkedObject, public QPolygonF
{
public:
    Polygon(MarkedClass* polygonClass = nullptr);
    
    MarkedClass* polygonClass() const { return m_polygonClass; }
    void setPolygonClass(MarkedClass* polygonClass) { m_polygonClass = polygonClass; }
    
private:
    MarkedClass* m_polygonClass;
};

#endif // POLYGON_H
