#include "sentence.h"

Sentence::Sentence(std::shared_ptr<SentencePrivate> d_ptr, MarkedClass* objClass, quint64 begin, quint64 end) :
    MarkedObject(std::static_pointer_cast<MarkedObjectPrivate>(d_ptr), objClass)
{
    d_ptr->m_begin = begin;
    d_ptr->m_end = end;
}
