#include "core/markedobject.h"

MarkedObject::MarkedObject(std::unique_ptr<MarkedObjectPrivate> d_ptr, MarkedClass* objClass) :
    d_p(std::move(d_ptr))
{
    d_p->m_objClass = objClass;
}

MarkedObject::MarkedObject(MarkedClass* objClass) :
    d_p(std::make_unique<MarkedObjectPrivate>())
{
    d_p->m_objClass = objClass;
}

MarkedClass* MarkedObject::objClass() const
{
    return d_p->m_objClass;
}

void MarkedObject::setObjClass(MarkedClass* objClass)
{
    d_p->m_objClass = objClass;
}
