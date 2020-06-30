#ifndef PAINTER_H
#define PAINTER_H

#include "ui/container.h"

#include <QVector>

class QGraphicsItem;

class Painter
{
public:
    explicit Painter(Container* parent);
    ~Painter();

    QGraphicsItem* currentItem() const { return m_currentItem; }
    
    virtual void paint(QPoint point) = 0;
    virtual void paintObject(MarkedObject* object) = 0;
    virtual void repaint() = 0;
    virtual void changeItem(const QString& path) = 0;

    virtual bool importObjects(QVector<MarkedObject*> objects) = 0;

protected:
    Container* m_parent;

    QVector<QGraphicsItem*> m_items;
    QGraphicsItem* m_currentItem;
};

#endif // PAINTER_H
