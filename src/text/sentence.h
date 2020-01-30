#ifndef SENTENCE_H
#define SENTENCE_H

#include "markedobject.h"
#include "sentence_p.h"

class Sentence : public MarkedObject
{
public:
    Sentence(std::shared_ptr<SentencePrivate> d_ptr, MarkedClass *objClass, quint64 begin, quint64 end);
};

#endif // SENTENCE_H
