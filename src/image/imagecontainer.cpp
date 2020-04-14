#include "image/imagecontainer.h"
#include "image/polygon.h"

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QMouseEvent>

ImageContainer::ImageContainer(QWidget* parent) :
    QGraphicsView(parent),
    m_currentImage(nullptr),
    m_shape(marK::Shape::Polygon),
    m_scaleW(0.0),
    m_scaleH(0.0)
{
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);
    setMinimumSize(860, 600);

    installEventFilter(this);

    setMouseTracking(true);
}

ImageContainer::~ImageContainer()
{
}

// TODO: improve me and remove this method
QVector<MarkedObject*> ImageContainer::savedObjects() const
{
    QVector<MarkedObject*> copyObjects(m_savedObjects);

    for (MarkedObject* obj : copyObjects) {
        Polygon* pol = static_cast<Polygon*>(obj);
        for (QPointF& point : *pol) {
            point -= m_currentImage->pos();
            point = scaledPoint(point);
        }
    }

    return copyObjects;
}

void ImageContainer::mousePressEvent(QMouseEvent* event)
{
    if (m_currentImage != nullptr) {
        QPoint point = event->pos();
        QPointF clickedPoint = mapToScene(point);

        bool isImageClicked = scene()->itemAt(clickedPoint, transform()) == m_currentImage;

        if (m_shape == marK::Shape::Polygon) {
            auto savedPolygClicked = std::find_if(
                m_savedObjects.begin(), m_savedObjects.end(),
                [&](const MarkedObject* obj) {
                    const Polygon* polygon = static_cast<const Polygon*>(obj);
                    return polygon->containsPoint(clickedPoint, Qt::OddEvenFill);
                }
            );
            bool isSavedPolygClicked = savedPolygClicked != m_savedObjects.end();
            if (isSavedPolygClicked) {
                m_currentObject = *savedPolygClicked;
                m_savedObjects.erase(savedPolygClicked);
                (static_cast<Polygon*>(m_currentObject))->pop_back();
            }

            bool isPolygFirstPtClicked = false;
            Polygon* currentPolygon = static_cast<Polygon*>(m_currentObject);
            if (!currentPolygon->empty()) {
                QPointF cPolygFirstPt = currentPolygon->first();
                QRectF cPolygFirstPtRect(cPolygFirstPt, QPointF(cPolygFirstPt.x() + 10, cPolygFirstPt.y() + 10));
                isPolygFirstPtClicked = cPolygFirstPtRect.contains(clickedPoint);
                if (isPolygFirstPtClicked)
                    clickedPoint = cPolygFirstPt;
            }

            if (isSavedPolygClicked || isPolygFirstPtClicked || isImageClicked) {
                Polygon* currentPolygon = static_cast<Polygon*>(m_currentObject);

                if (currentPolygon->size() > 1 && !currentPolygon->isClosed()) {
                    m_savedObjects << m_currentObject;
                    m_currentObject->clear();
                }

                repaint();
            }
        }
        else if (m_shape == marK::Shape::Rectangle) {
            if (isImageClicked) {
                Polygon* currentPolygon = static_cast<Polygon*>(m_currentObject);

                if (currentPolygon->empty())
                    *currentPolygon << clickedPoint;
                else {
                    QPointF firstPt = currentPolygon->first();
                    *currentPolygon << QPointF(clickedPoint.x(), firstPt.y()) << clickedPoint << QPointF(firstPt.x(), clickedPoint.y()) << firstPt;
                    m_savedObjects << m_currentObject;
                    m_currentObject->clear();
                }

                repaint();
            }
        }
    }

    QWidget::mousePressEvent(event);
}

void ImageContainer::changeItem(const QString& path)
{
    // TODO: create a temp file where the polygons from this image will be temporary stored, so they can be loaded again when the image is reopened
    m_savedObjects.clear();
    m_currentObject->clear();
    m_items.clear();

    scene()->setSceneRect(0, 0, 850, 640);
    scene()->clear();

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

    QGraphicsPixmapItem* pixmapItem = scene()->addPixmap(image);

    int x_scene = int(scene()->width() / 2);
    int y_scene = int(scene()->width() / 2);
    int x_image = int(image.width() / 2);
    int y_image = int(image.height() / 2);

    pixmapItem->setPos(x_scene - x_image, y_scene - y_image);

    m_currentImage = pixmapItem;
}

void ImageContainer::repaint()
{
    for (QGraphicsItem* item : m_items)
        scene()->removeItem(item);

    m_items.clear();

    for (MarkedObject* obj : m_savedObjects)
        paintObject(obj);

    paintObject(m_currentObject);
}

void ImageContainer::paintObject(MarkedObject* object)
{
    Polygon* polygon = static_cast<Polygon*>(object);

    if (polygon->size() > 1 && polygon->isClosed()) {
        QColor color(polygon->objClass()->color());
        color.setAlpha(35);

        QGraphicsPolygonItem* pol = scene()->addPolygon(*polygon, QPen(polygon->objClass()->color(), 2), QBrush(color));

        m_items << pol;
    }
    else {
        for (auto it = polygon->begin(); it != polygon->end(); ++it) {
            QGraphicsItem* item;

            if (it == polygon->begin()) {
                QBrush brush(polygon->objClass()->color());

                item = scene()->addRect((*it).x(), (*it).y(), 10, 10, QPen(brush, 2), brush);
            }
            else
                item = scene()->addLine(QLineF(*(it - 1), *it), QPen(QBrush(polygon->objClass()->color()), 2));

            m_items << item;
        }
    }
}

void ImageContainer::undo()
{
    Polygon* polygon = static_cast<Polygon*>(m_currentObject);

    if (!polygon->empty()) {
        polygon->pop_back();

        repaint();
    }
}

void ImageContainer::reset()
{
    m_savedObjects.clear();
    m_currentObject->clear();
    repaint();
}
