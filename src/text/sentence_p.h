#ifndef SENTENCE_P_H
#define SENTENCE_P_H

#include "ui/markedobject_p.h"

class SentencePrivate : public MarkedObjectPrivate
{
public:
    SentencePrivate(quint64 begin, quint64 end) : m_begin(begin), m_end(end) {}

    quint64 m_begin;
    quint64 m_end;
};

#endif // SENTENCE_P_H
