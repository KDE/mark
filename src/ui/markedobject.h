#ifndef MARKEDOBJECT_H
#define MARKEDOBJECT_H

#include <memory>

class MarkedClass;
class MarkedObjectPrivate;

class MarkedObject
{
public:
    explicit MarkedObject(std::shared_ptr<MarkedObjectPrivate> d_ptr, MarkedClass* objClass);

    MarkedClass* objClass() const { return d->m_objClass; }
    void setObjClass(MarkedClass* objClass) { d->m_objClass = objClass; }

    virtual void clear() = 0;

private:
    std::shared_ptr<MarkedObjectPrivate> d;
};

#endif // MARKEDOBJECT_H
