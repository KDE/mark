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
    /** Create a MarkedClass with given name.
     * @param name - name of the MarkedClass.
     */
    explicit MarkedClass(QString name);

    /** Create a MarkedClass with given name and color.
     * @param name - name of the MarkedClass.
     */
    explicit MarkedClass(QString name, QColor color);
    
public:

    QString name() const { /** @return the name of MarkedClass. */
        return m_name;
    }

    QColor color() const { /** @return the name of MarkedClass. */
        return m_color;
    }
    
    /** Change name of the instance.
     * @param name - new name of the instance.
     */
    void setName(const QString& name) { m_name = QString(name); }

    /** change the color of the instance.
     * @param color - new color of the instance.
     */
    void setColor(const QColor& color) { m_color = color; }

private:
    QString m_name;
    QColor m_color;
};

#endif // MARKEDCLASS_H
