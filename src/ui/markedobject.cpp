#include "markedobject.h"
#include "markedobject_p.h"

MarkedObject::MarkedObject(std::shared_ptr<MarkedObjectPrivate> d_ptr, MarkedClass* objClass) :
    d(d_ptr)
{
    d->m_objClass = objClass;
}

MarkedClass* MarkedObject::objClass() const
{
    return d->m_objClass;
}

void MarkedObject::setObjClass(MarkedClass* objClass)
{
    d->m_objClass = objClass;
}
