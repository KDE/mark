#ifndef SENTENCE_P_H
#define SENTENCE_P_H

#include "ui/markedobject_p.h"

class SentencePrivate : public MarkedObjectPrivate
{
public:
    quint64 m_begin;
    quint64 m_end;
};

#endif // SENTENCE_P_H