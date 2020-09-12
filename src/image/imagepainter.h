#ifndef IMAGEPAINTER_H
#define IMAGEPAINTER_H

#include "ui/container.h"
#include "ui/painter.h"

#include <QVector>

class ImagePainter : public Painter
{
public:
    /** Shape of the painter. */
    enum class Shape {
        Polygon,
        Rectangle
    };

    explicit ImagePainter(Container* parent);
    ~ImagePainter();
    
    void paint(QPoint point, bool isDragging) override;
    void paintObject(MarkedObject* object) override;
    void repaint() override;
    void undo() override;
    void deleteCurrentObject() override;
    void changeItem(const QString& filepath) override;

    bool importObjects(QVector<MarkedObject*> objects) override;
    
    void setShape(Shape shape) { m_shape = shape; m_parent->currentObject()->clear(); repaint(); }

private:
    /** @return a scaled Polygon based on given Polygon.
     * @params obj - Polygon to copy
     */
    MarkedObject* scale(const MarkedObject* obj);

    QVector<QGraphicsItem*> m_items;
    QGraphicsItem* m_currentItem;
    qreal m_scaleW;
    qreal m_scaleH;

    Shape m_shape;
};

#endif // IMAGEPAINTER_H
