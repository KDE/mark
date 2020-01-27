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

#ifndef MARKEDCLASS_H
#define MARKEDCLASS_H

#include <QColor>
#include <QString>

class MarkedClass
{
public:
    explicit MarkedClass(QString name);
    explicit MarkedClass(QString name, QColor color);
    
public:
    QString name() const { return m_name; }
    QColor color() const { return m_color; }
    
    void setName(const QString& name) { m_name = name; }
    void setColor(const QColor& color) { m_color = color; }

private:
    QString m_name;
    QColor m_color;
};

#endif // MARKEDCLASS_H
