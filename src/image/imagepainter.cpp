#include "image/imagepainter.h"
#include "image/polygon.h"

#include <QGraphicsItem>


ImagePainter::ImagePainter(Container* parent) :
    Painter(parent),
    m_shape(Shape::Polygon)
{
    m_parent->setCurrentObject(new Polygon(m_parent->currentObject()->objClass()));
    m_parent->viewport()->installEventFilter(m_parent);
    m_parent->viewport()->setMouseTracking(false);
}

ImagePainter::~ImagePainter()
{
    if (m_items.isEmpty())
        return;

    for (QGraphicsItem* item : m_items)
        m_parent->scene()->removeItem(item);

    m_items.clear();

    m_parent->scene()->removeItem(m_currentItem);
}

void ImagePainter::paint(QPoint point, bool isDragging)
{
    QGraphicsPixmapItem* currentItem = static_cast<QGraphicsPixmapItem*>(m_currentItem);
    if (currentItem != nullptr) {
        QPointF clickedPoint = m_parent->mapToScene(point);

        bool isImageClicked = m_parent->scene()->itemAt(clickedPoint, m_parent->transform()) == currentItem;
        
        Polygon* currentPolygon = static_cast<Polygon*>(m_parent->currentObject());

        if (m_shape == Shape::Polygon) {
            int idxSavedPolygClicked = -1;
            QPointF scaledClickedPoint = QPointF(-m_currentItem->pos());
            scaledClickedPoint += QPointF(scaledClickedPoint.x()/m_scaleW, scaledClickedPoint.y()/m_scaleH);
            for (int i = 0; i < m_parent->savedObjects().size(); i++) {
                const Polygon* polygon = static_cast<const Polygon*>(m_parent->savedObjects()[i]);
                if (polygon->containsPoint(scaledClickedPoint, Qt::OddEvenFill)) {
                    idxSavedPolygClicked = i;
                    break;
                }
            }

            bool isSavedPolygClicked = idxSavedPolygClicked != -1;
            if (isSavedPolygClicked) {
                if (isDragging)
                    return;
                delete currentPolygon;
                m_parent->setCurrentObject(m_parent->savedObjects()[idxSavedPolygClicked]);
                m_parent->savedObjects().remove(idxSavedPolygClicked);
                currentPolygon = static_cast<Polygon*>(m_parent->currentObject());
                currentPolygon->unscale(m_currentItem->pos(), m_scaleW, m_scaleH);
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
                    currentPolygon->scale(m_currentItem->pos(), m_scaleW, m_scaleH);
                    m_parent->appendObject(currentPolygon);
                    m_parent->setCurrentObject(new Polygon(currentPolygon->objClass()));
                }

                repaint();
            }
        }
        else if (m_shape == Shape::Rectangle) {
            bool toSave = point.isNull() && currentPolygon->size() > 1;
            if (isImageClicked) {
                if (currentPolygon->empty())
                    *currentPolygon << clickedPoint;

                else {
                    QPointF firstPt = currentPolygon->first();
                    if (isDragging) {
                        currentPolygon->clear();
                        *currentPolygon << firstPt;
                    }
                    else
                        toSave = true;
                    *currentPolygon << QPointF(clickedPoint.x(), firstPt.y()) << clickedPoint << QPointF(firstPt.x(), clickedPoint.y()) << firstPt;
                }

                repaint();
            }
            if (toSave) {
                currentPolygon->scale(m_currentItem->pos(), m_scaleW, m_scaleH);
                m_parent->appendObject(currentPolygon);
                m_parent->setCurrentObject(new Polygon(m_parent->currentObject()->objClass()));
            }
        }
    }
}

void ImagePainter::changeItem(const QString& filepath)
{
    QGraphicsPixmapItem* pixmapItem = nullptr;

    if (filepath != "") {
        QPixmap image(filepath);
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

        pixmapItem = m_parent->scene()->addPixmap(image);

        int x_scene = int(m_parent->scene()->width() / 2);
        int y_scene = int(m_parent->scene()->height() / 2);
        int x_image = int(image.width() / 2);
        int y_image = int(image.height() / 2);

        pixmapItem->setPos(x_scene - x_image, y_scene - y_image);
    }

    m_currentItem = pixmapItem;
    m_filepath = filepath;
}

void ImagePainter::repaint()
{
    for (QGraphicsItem* item : m_items)
        m_parent->scene()->removeItem(item);

    m_items.clear();

    for (MarkedObject* obj : m_parent->savedObjects())
        paintObject(obj);

    paintObject(m_parent->currentObject());
}

void ImagePainter::undo()
{
    Polygon* polygon = static_cast<Polygon*>(m_parent->currentObject());

    if (!polygon->empty()) {
        polygon->pop_back();
        repaint();
    }
}

void ImagePainter::deleteCurrentObject()
{
    Polygon* polygon = static_cast<Polygon*>(m_parent->currentObject());

    if (!polygon->isClosed()) {
        polygon->clear();
        repaint();
    }
}

void ImagePainter::paintObject(MarkedObject* object)
{
    Polygon polygon (static_cast<const Polygon*>(object));
    if (object != m_parent->currentObject()) {
        QPointF offset = m_currentItem->pos();
        for (QPointF& point : polygon) {
            point = QPointF(point.x() * m_scaleW, point.y() * m_scaleH);
            point += offset;
        }
    }

    QColor color(polygon.objClass()->color());
    QBrush brush(color);
    QPen pen(brush, 2);

    if (polygon.size() > 1 && polygon.isClosed()) {
        color.setAlpha(35);

        QGraphicsPolygonItem* pol = m_parent->scene()->addPolygon(polygon, pen, QBrush(color));

        m_items << pol;
    }
    else {
        for (auto it = polygon.begin(); it != polygon.end(); ++it) {
            QGraphicsItem* item;
            if (it == polygon.begin())
                item = m_parent->scene()->addRect((*it).x(), (*it).y(), 10, 10, pen, brush);
            else
                item = m_parent->scene()->addLine(QLineF(*(it - 1), *it), pen);

            m_items << item;
        }
    }
}

bool ImagePainter::importObjects(QVector<MarkedObject*> objects)
{
    if (objects.isEmpty() || objects.first()->type() != MarkedObject::Type::Polygon)
        return false;

    m_parent->savedObjects() = objects;
    repaint();

    return true;
}

void ImagePainter::zoomIn()
{
    m_parent->currentObject()->clear();
    if (m_scaleH < 4.0 && m_scaleW < 4.0)
        scaleImage(1.25);
}

void ImagePainter::zoomOut()
{
    m_parent->currentObject()->clear();
    if (m_scaleH > 0.25 && m_scaleW > 0.25)
        scaleImage(0.75);
}

void ImagePainter::scaleImage(double factor)
{
    if (m_filepath != "") {
        m_scaleH *= factor;
        m_scaleW *= factor;

        QPixmap image(m_filepath);
        QPixmap scaledImage;

        scaledImage = image.scaledToHeight(int(image.height() * m_scaleH));
        scaledImage = image.scaledToWidth(int(image.width() * m_scaleW));

        image = scaledImage;

        m_parent->scene()->removeItem(m_currentItem);
        QGraphicsPixmapItem* pixmapItem = m_parent->scene()->addPixmap(image);

        int x_scene = int(m_parent->scene()->width() / 2);
        int y_scene = int(m_parent->scene()->height() / 2);
        int x_image = int(image.width() / 2);
        int y_image = int(image.height() / 2);

        pixmapItem->setPos(x_scene - x_image, y_scene - y_image);
        m_currentItem = pixmapItem;

        repaint();
    }
}
