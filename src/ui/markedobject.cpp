#include "markedobject.h"
#include "markedobject_p.h"

MarkedObject::MarkedObject(std::shared_ptr<MarkedObjectPrivate> d_ptr, MarkedClass* objClass) :
    d(d_ptr)
{
    d->m_objClass = objClass;
}
