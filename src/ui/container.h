#ifndef CONTAINER_H
#define CONTAINER_H

#include "ui/markedclass.h"
#include "ui/markedobject.h"

#include <QVector>

/** Base class of all annotation containers. */
class Container
{
public:
    /** Set MarkedClass of the current object.
     * @param objClass - Class to define.
     */
    void setObjClass(MarkedClass* objClass);

public:
    /** @return saved annotated objects. */
    virtual QVector<MarkedObject*> savedObjects() const;

    /** Load given item/file.
     * @param itemPath - Path of the item.
     */
    virtual void changeItem(const QString& itemPath) = 0;

    /** Repaint annotated objects. */
    virtual void repaint() = 0;

    /** Paint given object.
     * @param object - object to paint.
     */
    virtual void paintObject(MarkedObject* object) = 0;

    /** Load given objects.
     * @param objects - Vector of annotated object to load.
     */
    virtual bool importObjects(QVector<MarkedObject*> objects) = 0;

public slots:
    /** Undo last action. */
    virtual void undo() = 0;

    /** Delete all annotated objects. */
    virtual void reset() = 0;

protected:
    MarkedObject* m_currentObject;
    QVector<MarkedObject*> m_savedObjects;
};

#endif // CONTAINER_H
