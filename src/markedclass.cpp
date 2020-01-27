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

#include "markedclass.h"

#include <QTime>
#include <QtGlobal>

MarkedClass::MarkedClass(QString name) :
    m_name(name)
{
    qsrand((uint)QTime::currentTime().msec());

    m_color = QColor(qrand() % 256, qrand() % 256, qrand() % 256);
}

MarkedClass::MarkedClass(QString name, QColor color) :
    m_name(name),
    m_color(color)
{
}
