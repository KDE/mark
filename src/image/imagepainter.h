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

    void zoomIn();
    void zoomOut();
    void scaleImage(double factor);

private:
    QVector<QGraphicsItem*> m_items;
    QGraphicsItem* m_currentItem;
    qreal m_scaleW;
    qreal m_scaleH;

    Shape m_shape;

    QString m_filepath;
};

#endif // IMAGEPAINTER_H
