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

#include "ui/mark.h"
#include "ui/markedobject.h"

#include <QVector>
#include <QString>

class Serializer
{
public:
    explicit Serializer();

    explicit Serializer(const QVector<MarkedObject*>& items);

    bool write(const QString& filepath, marK::OutputType output_type);

    QVector<MarkedObject*> read(const QString& filename);

private:
    QByteArray getData(const QString& filename);
    QString toJSON();
    QString toXML();

    QVector<MarkedObject*> readJSON(const QString& filename);
    QVector<MarkedObject*> readXML(const QString& filename);

    QString serialize(marK::OutputType output_type);

    MarkedClass* getMarkedClass(const QString& className, const QVector<MarkedObject*>& objects);

private:
    QVector<MarkedObject*> m_items;
};
#endif // SERIALIZER_H
