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
    ~Painter();

    /** @return pointer to the current instance of QGraphicsItem. */
    QGraphicsItem* currentItem() const { return m_currentItem; }

    /** paint given point.
     * @param point - point to paint.
     */
    virtual void paint(QPoint point) = 0;

    /** Display given object.
     * @param object - MarkedObject instance to display.
     */
    virtual void paintObject(MarkedObject* object) = 0;

    /** Load the current MarkedObjects. */
    virtual void repaint() = 0;

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

    QVector<QGraphicsItem*> m_items;
    QGraphicsItem* m_currentItem;
};

#endif // PAINTER_H