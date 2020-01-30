#include "ui/imagecontainer.h"

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
QVector<MarkedObject> ImageContainer::savedObjects() const
{
    QVector<MarkedObject> copyObjects(m_savedObjects);

    for (MarkedObject& obj : copyObjects) {
        Polygon& pol = static_cast<Polygon&>(obj);
        for (QPointF& point : pol) {
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
                [&](const MarkedObject& obj) {
                    const Polygon& polygon = static_cast<const Polygon&>(obj);
                    return polygon.containsPoint(clickedPoint, Qt::OddEvenFill);
                }
            );
            bool isSavedPolygClicked = savedPolygClicked != m_savedObjects.end();
            if (isSavedPolygClicked) {
                m_currentObject = *savedPolygClicked;
                m_savedObjects.erase(savedPolygClicked);
                (static_cast<Polygon*>(&m_currentObject)).pop_back();
            }

            bool isPolygFirstPtClicked = false;
            if (!m_currentObject.empty()) {
                Polygon* currentPolygon = static_cast<Polygon*>(&m_currentObject);
                QPointF cPolygFirstPt = currentPolygon->first();
                QRectF cPolygFirstPtRect(cPolygFirstPt, QPointF(cPolygFirstPt.x() + 10, cPolygFirstPt.y() + 10));
                isPolygFirstPtClicked = cPolygFirstPtRect.contains(clickedPoint);
                if (isPolygFirstPtClicked)
                    clickedPoint = cPolygFirstPt;
            }

            if (isSavedPolygClicked || isPolygFirstPtClicked || isImageClicked) {
                Polygon* currentPolygon = static_cast<Polygon*>(&m_currentObject);

                if (currentPolygon->size() > 1 && !currentPolygon->isClosed()) {
                    m_savedObjects << m_currentObject;
                    m_currentObject.clear();
                }

                repaint();
            }
        }
        else if (m_shape == marK::Shape::Rectangle) {
            if (isImageClicked) {
                Polygon* currentPolygon = static_cast<Polygon*>(&m_currentObject);

                if (currentPolygon->empty())
                    currentPolygon << clickedPoint;
                else {
                    QPointF firstPt = currentPolygon->first();
                    currentPolygon << QPointF(clickedPoint.x(), firstPt.y()) << clickedPoint << QPointF(firstPt.x(), clickedPoint.y()) << firstPt;
                    m_savedObjects << m_currentObject;
                    m_currentObject.clear();
                }

                repaint();
            }
        }
    }

    QWidget::mousePressEvent(event);
}

void ImageContainer::changeItem(const QString& path)
{
    m_savedObjects.clear();
    m_items.clear();
    m_currentObject.clear();
}

void ImageContainer::clearScene()
{
}
