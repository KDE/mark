#ifndef IMAGECONTAINER_H
#define IMAGECONTAINER_H

#include "ui/mark.h"
#include "ui/container.h"

#include <QGraphicsView>

class ImageContainer : public Container, public QGraphicsView
{
public:
    explicit ImageContainer(QWidget* parent = nullptr);
    ~ImageContainer() override;

public:
    QVector<MarkedObject*> savedObjects() const override;

    void mousePressEvent(QMouseEvent* event) override;

    void changeItem(const QString& path) override;

    void repaint();
    void paintObject(MarkedObject* object) override;
    QVector<MarkedClass*> importObjects(QVector<MarkedObject*> objects) override;

    void setShape(marK::Shape shape) { m_shape = shape; m_currentObject->clear(); repaint(); }

public slots:
    void undo() override;
    void reset() override;

private:
    QVector<QGraphicsItem*> m_items;
    QGraphicsPixmapItem* m_currentImage;
    // FIXME: remove m_shape from marK
    marK::Shape m_shape;

    qreal m_scaleW;
    qreal m_scaleH;
};

#endif // IMAGECONTAINER_H
