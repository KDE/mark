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

#ifndef SENTENCE_H
#define SENTENCE_H

#include "ui/markedobject.h"
#include "ui/markedclass.h"

/** Class that represent the annotated data resulted from text annotation. */
class Sentence : public MarkedObject
{
public:
    /**  Create a Sentence.
     * @param objClass - MarkedClass's instance to assign.
     * @param begin - Location of the first letter of Sentence.
     * @param end - Location of the last letter of sentence.
     */
    explicit Sentence(MarkedClass *objClass, quint64 begin, quint64 end);

    void clear() override;
    void append(double memberX, double memberY) override;
    int size() const override;
    QString unitName() const override;
    QString type() const override;
    QString memberX() const override;
    QString memberY() const override;
    qreal XValueOf(int element) const override;
    qreal YValueOf(int element) const override;
};

#endif // SENTENCE_H
