#include "markedobject.h"
#include "markedobject_p.h"

MarkedObject::MarkedObject(std::shared_ptr<MarkedObjectPrivate> d_ptr, MarkedClass* objClass) :
    d_p(d_ptr)
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

QString MarkedObject::className() const
{
    return d_p->m_objClass->name();
}
