#include "image/imagepainter.h"
#include "image/polygon.h"

#include <QGraphicsItem>

ImagePainter::ImagePainter(Container* parent) :
    m_parent(parent),
    m_shape(Shape::Polygon)
{
}

ImagePainter::~ImagePainter()
{
    // TODO: destroy items to avoid memory leak when you change the painter
    for (QGraphicsItem* item : m_items)
        m_parent->scene()->removeItem(item);

    m_items.clear();
    
    m_parent->scene()->removeItem(m_currentItem);
}

void ImagePainter::paint(QPoint point)
{
    QGraphicsPixmapItem* currentItem = static_cast<QGraphicsPixmapItem*>(m_currentItem);
    if (currentItem != nullptr) {
        QPointF clickedPoint = m_parent->mapToScene(point);

        bool isImageClicked = m_parent->scene()->itemAt(clickedPoint, m_parent->transform()) == currentItem;
        
        Polygon* currentPolygon = static_cast<Polygon*>(m_parent->m_currentObject);

        if (m_shape == Shape::Polygon) {

            int idxSavedPolygClicked = -1;
            for (int i = 0; i < m_parent->m_savedObjects.size(); i++) {
                const Polygon* polygon = static_cast<const Polygon*>(m_parent->m_savedObjects[i]);
                if (polygon->containsPoint(clickedPoint, Qt::OddEvenFill)) {
                    idxSavedPolygClicked = i;
                    break;
                }
            }
            bool isSavedPolygClicked = idxSavedPolygClicked != -1;
            if (isSavedPolygClicked) {
                delete currentPolygon;
                m_parent->setCurrentObject(m_parent->m_savedObjects[idxSavedPolygClicked]);
                m_parent->m_savedObjects.remove(idxSavedPolygClicked);
                currentPolygon = static_cast<Polygon*>(m_parent->m_currentObject);
                currentPolygon->pop_back();
            }

            bool isPolygFirstPtClicked = false;
            if (!currentPolygon->empty()) {
                QPointF cPolygFirstPt = currentPolygon->first();
                QRectF cPolygFirstPtRect(cPolygFirstPt, QPointF(cPolygFirstPt.x() + 10, cPolygFirstPt.y() + 10));
                isPolygFirstPtClicked = cPolygFirstPtRect.contains(clickedPoint);
                if (isPolygFirstPtClicked)
                    clickedPoint = cPolygFirstPt;
            }

            if (isSavedPolygClicked || isPolygFirstPtClicked || isImageClicked) {
                *currentPolygon << clickedPoint;

                if (currentPolygon->size() > 1 && currentPolygon->isClosed()) {
                    m_parent->m_savedObjects << currentPolygon;
                    Polygon* newPolygon = new Polygon(currentPolygon->objClass());
                    m_parent->setCurrentObject(newPolygon);
                }

                repaint();
            }
        }
        else if (m_shape == Shape::Rectangle) {
            if (isImageClicked) {
                if (currentPolygon->empty())
                    *currentPolygon << clickedPoint;

                else {
                    QPointF firstPt = currentPolygon->first();
                    *currentPolygon << QPointF(clickedPoint.x(), firstPt.y()) << clickedPoint << QPointF(firstPt.x(), clickedPoint.y()) << firstPt;
                    m_parent->m_savedObjects << currentPolygon;
                    Polygon* newPolygon = new Polygon(currentPolygon->objClass());
                    m_parent->setCurrentObject(newPolygon);
                }

                repaint();
            }
        }
    }
}

void ImagePainter::changeItem(const QString& path)
{
    QPixmap image(path);
    QPixmap scaledImage;

    if (image.height() >= 1280)
        scaledImage = image.scaledToHeight(int(1280 * 0.8));

    if (image.width() >= 960)
        scaledImage = image.scaledToWidth(int(960 * 0.8));

    if (!scaledImage.isNull()) {
        m_scaleW = qreal(scaledImage.width()) / qreal(image.width());
        m_scaleH = qreal(scaledImage.height()) / qreal(image.height());
        image = scaledImage;
    }
    else {
        m_scaleW = 1.0;
        m_scaleH = 1.0;
    }

    QGraphicsPixmapItem* pixmapItem = m_parent->scene()->addPixmap(image);

    int x_scene = int(m_parent->scene()->width() / 2);
    int y_scene = int(m_parent->scene()->height() / 2);
    int x_image = int(image.width() / 2);
    int y_image = int(image.height() / 2);

    pixmapItem->setPos(x_scene - x_image, y_scene - y_image);

    m_currentItem = pixmapItem;
}

void ImagePainter::repaint()
{
    for (QGraphicsItem* item : m_items)
        m_parent->scene()->removeItem(item);

    m_items.clear();

    for (MarkedObject* obj : m_parent->m_savedObjects)
        paintObject(obj);

    paintObject(m_parent->currentObject());
}

void ImagePainter::paintObject(MarkedObject* object)
{
    Polygon* polygon = static_cast<Polygon*>(object);
    QColor color(polygon->objClass()->color());
    QBrush brush(color);
    QPen pen(brush, 2);

    if (polygon->size() > 1 && polygon->isClosed()) {
        color.setAlpha(35);

        QGraphicsPolygonItem* pol = m_parent->scene()->addPolygon(*polygon, pen, QBrush(color));

        m_items << pol;
    }
    else {
        for (auto it = polygon->begin(); it != polygon->end(); ++it) {
            QGraphicsItem* item;

            if (it == polygon->begin())
                item = m_parent->scene()->addRect((*it).x(), (*it).y(), 10, 10, pen, brush);

            else
                item = m_parent->scene()->addLine(QLineF(*(it - 1), *it), pen);

            m_items << item;
        }
    }
}

bool ImagePainter::importObjects(QVector<MarkedObject*> objects)
{
    QPointF offset = m_currentItem->pos();

    m_parent->reset();

    for (MarkedObject* object : objects) {
        Polygon* polygon = static_cast<Polygon*>(object);
        for (QPointF& point : *polygon) {
            point = QPointF(point.x() * m_scaleW, point.y() * m_scaleH);
            point += offset;
        }

        // TODO: use a method
        m_parent->m_savedObjects << object;
    }

    repaint();

    return !objects.isEmpty();
}

MarkedObject* ImagePainter::scale(const MarkedObject* obj)
{
    Polygon* pol = new Polygon(static_cast<const Polygon*>(obj));
    for (QPointF& point : *pol) {
        point -= m_currentItem->pos();
        point = QPointF(point.x() / m_scaleW, point.y() / m_scaleH);
    }
    return pol;
}
