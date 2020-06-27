#ifndef SENTENCE_H
#define SENTENCE_H

#include "markedobject.h"

class Sentence : public MarkedObject
{
public:
    Sentence(MarkedClass *objClass, quint64 begin, quint64 end);

    void clear() override;
};

#endif // SENTENCE_H
