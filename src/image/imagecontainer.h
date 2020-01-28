#ifndef IMAGECONTAINER_H
#define IMAGECONTAINER_H

#include "ui/container.h"

#include <QGraphicsView>

class ImageContainer : public Container, public QGraphicsView
{
public:
    explicit ImageContainer(QWidget* parent = nullptr);
    ~ImageContainer() override;

public:
    QVector<MarkedObject> savedObjects() const;

    void mousePressEvent(QMouseEvent* event) override;

    void clearScene();
    void repaint();
    void paintObject(MarkedObject& object);

    void setCurrentClass(MarkedClass* objClass) { m_currentObject.setClass(objClass); repaint(); }
    void setShape(marK::Shape shape) { m_shape = shape; m_currentObject.clear(); repaint(); }

    QPointF scaledPoint(const QPointF& point) const { return QPointF(point.x(), point.y()); }

public slots:
    void undo();
    void reset();

private:
    MarkedObject m_currentObject; // TODO: move to container
    QVector<MarkedObject> m_savedObjects; // TODO: move to container
    QVector<QGraphicsItem*> m_items;
    QGraphicsPixmapItem* m_currImage;
    marK::Shape m_shape;

    qreal m_scaleW;
    qreal m_scaleH;
};

#endif // IMAGECONTAINER_H
