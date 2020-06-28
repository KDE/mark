#ifndef IMAGEPAINTER_H
#define IMAGEPAINTER_H

//#include "ui/painter.h"

#include "ui/container.h"

#include <QVector>

class QGraphicsItem;

class ImagePainter //: public Painter
{
public:
    /** Shape of the painter. */
    enum class Shape {
        Polygon,
        Rectangle
    };

public:
    explicit ImagePainter(Container* parent);
    ~ImagePainter();
    
    QGraphicsItem* currentItem() const { return m_currentItem; }
    qreal scaleW() const { return m_scaleW; }
    qreal scaleH() const { return m_scaleH; }
    
    void paint(QPoint point);
    void paintObject(MarkedObject* object);
    void repaint();
    void changeItem(const QString& path);

    MarkedObject* scale(const MarkedObject* obj);
    
    void setShape(Shape shape) { m_shape = shape; m_parent->currentObject()->clear(); repaint(); }
    
    bool importObjects(QVector<MarkedObject*> objects);
    
private:
    Container* m_parent;

    QVector<QGraphicsItem*> m_items;
    QGraphicsItem* m_currentItem;

    qreal m_scaleW;
    qreal m_scaleH;

    Shape m_shape;
};

#endif // IMAGEPAINTER_H
