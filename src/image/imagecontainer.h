#ifndef IMAGECONTAINER_H
#define IMAGECONTAINER_H

#include "ui/container.h"

#include <QGraphicsView>

/** Image container for image annotation. */
class ImageContainer : public Container, public QGraphicsView
{
public:
    /** Shape of the drawer. */
    enum class Shape {
        Polygon,
        Rectangle
    };

public:
    explicit ImageContainer(QWidget* parent = nullptr);
    ~ImageContainer() override;

public:
    QVector<MarkedObject*> savedObjects() const override;

    void mousePressEvent(QMouseEvent* event) override;

    void changeItem(const QString& path) override;

    void repaint();
    void paintObject(MarkedObject* object) override;
    bool importObjects(QVector<MarkedObject*> objects) override;

    /** Change shape of the drawer. */
    void setShape(Shape shape) { m_shape = shape; m_currentObject->clear(); repaint(); }

public slots:
    void undo() override;
    void reset() override;

private:
    QVector<QGraphicsItem*> m_items;
    QGraphicsPixmapItem* m_currentImage;
    Shape m_shape;

    qreal m_scaleW;
    qreal m_scaleH;
};

#endif // IMAGECONTAINER_H
