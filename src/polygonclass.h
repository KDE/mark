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

#ifndef POLYGONCLASS_H
#define POLYGONCLASS_H

#include <QColor>
#include <QString>

class PolygonClass
{
public:
    explicit PolygonClass(QString text);
    explicit PolygonClass(QString text, QColor color);
    
public:
    QString text() const { return m_text; }
    QColor color() const { return m_color; }
    
    void setText(const QString& text) { m_text = text; }
    void setColor(const QColor& color) { m_color = color; }

private:
    QString m_text;
    QColor m_color;
};

#endif // POLYGONCLASS_H
