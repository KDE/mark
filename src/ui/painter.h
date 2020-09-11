#ifndef PAINTER_H
#define PAINTER_H

#include "ui/container.h"

#include <QVector>

class QGraphicsItem;

/** Base class, responsible for load, display and annotate data. */
class Painter
{
public:
    /** Constructor of Painter.
     * @param parent - Container instance to use.
     */
    explicit Painter(Container* parent);
    virtual ~Painter();

    /** paint given point.
     * @param point - point to paint.
     * @param isDragging = to know if it is a mouse drag event
     */
    virtual void paint(QPoint point, bool isDragging) = 0;

    /** Display given object.
     * @param object - MarkedObject instance to display.
     */
    virtual void paintObject(MarkedObject* object) = 0;

    /** Load the current MarkedObjects. */
    virtual void repaint() = 0;

    /** Undo last paint. */
    virtual void undo() = 0;

    /** Delete the current instance of MarkedObject. */
    virtual void deleteCurrentObject() = 0;

    /** Load given item base on its path.
     * @param path - item path.
     */
    virtual void changeItem(const QString& path) = 0;

    /** Import Objects to the parent.
     * @param objects - objects to load.
     */
    virtual bool importObjects(QVector<MarkedObject*> objects) = 0;
protected:
    Container* m_parent;
};

#endif // PAINTER_H
