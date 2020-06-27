#include "sentence.h"
#include "sentence_p.h"

#include <memory>

Sentence::Sentence(MarkedClass* objClass, quint64 begin, quint64 end)
{
    d_p = std::make_unique<SentencePrivate>();
    d_ptr->m_begin = begin;
    d_ptr->m_end = end;
}

void Sentence::clear()
{
    d_ptr->m_begin = 0;
    d_ptr->m_end = 0;
}
