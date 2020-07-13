#ifndef MARKEDOBJECT_H
#define MARKEDOBJECT_H

#include <memory>
#include <QVariant>
#include <QString>

#include "ui/markedclass.h"
#include "ui/markedobject_p.h"

class MarkedClass;

/** Base class that represents all types of annotated data. */
class MarkedObject
{
public:
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

    /** @return the name of instance MarkedClass. */
    QString className() const;

    /** Delete all elements. */
    virtual void clear() = 0;

    /** Add element to MarkedObject.
     * @param memberX - first member/location of annotation.
     * @param memberY - second member/location of annotation.
     */
    virtual void append(double memberX, double memberY) = 0;

    /** @return how much elements a MarkedObject has. */
    virtual int size() const = 0;

    /** @return unitary name of the MarkedObject. */
    virtual QString unitName() const = 0;

    /** @return type of annotation. */
    virtual QString type() const = 0;

    /** @return name of first member of annotation element.*/
    virtual QString memberX() const = 0;

    /** @return name of second member of annotation element.*/
    virtual QString memberY() const = 0;

    /** @return the first member/location of given element.
     * @param element - element to get the member/location.
     */
    virtual qreal XValueOf(int element = 0) const = 0;

    /** @return the second member/location of given element.
     * @param element - element to get the member/location.
     */
    virtual qreal YValueOf(int element = 0) const = 0;

protected:
    std::unique_ptr<MarkedObjectPrivate> d_p;
};

#endif // MARKEDOBJECT_H
