#ifndef IMAGEPAINTER_H
#define IMAGEPAINTER_H

#include "ui/painter.h"
#include "ui/container.h"

#include <QVector>

class ImagePainter : public Painter
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
    
    qreal scaleW() const { return m_scaleW; }
    qreal scaleH() const { return m_scaleH; }
    
    void paint(QPoint point, bool isDragging) override;
    void paintObject(MarkedObject* object) override;
    void repaint() override;
    void undo() override;
    void deleteCurrentObject() override;
    void changeItem(const QString& path) override;

    bool importObjects(QVector<MarkedObject*> objects) override;

    MarkedObject* scale(const MarkedObject* obj);
    
    void setShape(Shape shape) { m_shape = shape; m_parent->currentObject()->clear(); repaint(); }
    
private:
    qreal m_scaleW;
    qreal m_scaleH;

    Shape m_shape;
};

#endif // IMAGEPAINTER_H
