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

#include "core/markedobject.h"
#include "core/markedclass.h"

/** Class that represent the annotated data resulted from text annotation. */
class Sentence : public MarkedObject
{
public:
    /**  Create a Sentence.
     * @param objClass - MarkedClass's instance to assign.
     * @param begin - Location of the first letter of Sentence.
     * @param end - Location of the last letter of sentence.
     */
    explicit Sentence(MarkedClass *objClass, quint64 begin = 0, quint64 end = 0);

    void clear() override;
    QString unitName() const override;

    /** Verify if a sentence is valid (its begin is different and smaller than its end). */
    bool isValid();

    /** Verify if a number is between the begin and end of a sentence. */
    bool hasBetween(int number);

    /** Move the position of a sentence.
     * @param anchor - new beggining of the sentence
     * @param end - new end of the sentence
     */
    void move(double begin, double end);

    /** @return the begin of the sentence. */
    qreal begin() const;

    /** @return the end of the sentence. */
    qreal end() const;
};

#endif // SENTENCE_H
