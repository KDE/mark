#include "ui/imagecontainer.h"

ImageContainer::ImageContainer(QWidget* parent) :
    QGraphicsView(parent),
    m_currImage(nullptr),
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
        for (int i = 0; i < obj.size(); i++) {
            QPointF point = obj.item(i).toPointF();
            point -= m_currentImage->pos();
            point = scaledPoint(point);
            obj.setItem(i, point);
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
                    QPolygonF polygon;
                    // TODO: remove this loop
                    for (int i = 0; i < obj.size(); i++)
                        polygon << obj.item(i).toPointF();
                    return polygon.containsPoint(clickedPoint, Qt::OddEvenFill);
                }
            );
            bool isSavedPolygClicked = savedPolygClicked != m_savedObjects.end();
            if (isSavedPolygClicked) {
                m_currentObject = *savedPolygClicked;
                m_savedObjects.erase(savedPolygClicked);
                m_currentObject.pop_back();
            }
        }
    
    }
}

void ImageContainer::changeItem(const QString& path)
{
    m_savedObjects.clear();
    // TODO
}

void ImageContainer::clearScene()
{
}
