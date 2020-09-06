#ifndef MARKEDOBJECT_H
#define MARKEDOBJECT_H

#include <memory>
#include <QVariant>
#include <QString>

#include "core/markedclass.h"
#include "core/markedobject_p.h"

class MarkedClass;

/** Base class that represents all types of annotated data. */
class MarkedObject
{
public:
    enum class Type {
        Polygon,
        Sentence
    };
    /** Create a MarkedObject with given d pointer and MarkedClass's object.
     * @param d_ptr - d pointer.
     * @param objClass - pointer of a MarkedClass.
     */
    explicit MarkedObject(std::unique_ptr<MarkedObjectPrivate> d_ptr, MarkedClass* objClass);

    /** Create a MarkedObject with given MarkedClass's object, initialize a d pointer internally.
     * @param objClass - pointer of a MarkedClass.
     */
    explicit MarkedObject(MarkedClass* objClass);

    /** @return the instance of MarkedClass used. */
    MarkedClass* objClass() const;

    /** Chance the instance of MarkedClass used. */
    void setObjClass(MarkedClass* objClass);

    /** Reset MarkedObject. */
    virtual void clear() = 0;

    /** Reset MarkedObject::Type of the instance. */
    virtual MarkedObject::Type type() = 0;

    /** @return unitary name of the MarkedObject instance. */
    virtual QString unitName() const = 0;

protected:
    std::unique_ptr<MarkedObjectPrivate> d_p;
};

#endif // MARKEDOBJECT_H
