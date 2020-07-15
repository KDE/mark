#include "container.h"
#include "image/polygon.h"
#include "image/imagepainter.h"

#include <QGraphicsScene>
#include <QMouseEvent>
#include <memory>

Container::Container(QWidget* parent) :
    QGraphicsView(parent),
    m_currentObject(new Polygon),
    m_painter(new ImagePainter(this))
{
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);
    setMinimumSize(860, 600);

    installEventFilter(this);

    setMouseTracking(true);
}

Container::~Container()
{
}

void Container::mousePressEvent(QMouseEvent* event)
{
    m_painter->paint(event->pos());
    
    QWidget::mousePressEvent(event);
}

void Container::changeItem(const QString& path)
{
    reset();

    scene()->setSceneRect(0, 0, 850, 640);
    scene()->clear();

    // TODO: check if file format of path then change
    m_painter->changeItem(path);

    emit changed(!scene()->items().empty());
}

void Container::setObjClass(MarkedClass* objClass)
{
    m_currentObject->setObjClass(objClass);
    repaint();
}

void Container::repaint()
{
    m_painter->repaint();
}

void Container::paintObject(MarkedObject* object)
{
    m_painter->paintObject(object);
}

bool Container::importObjects(QVector<MarkedObject*> objects)
{
    return m_painter->importObjects(objects);
}

void Container::appendObject(MarkedObject* object)
{
    m_savedObjects << object;
    Q_EMIT savedObjectsChanged();
}

void Container::undo()
{
    m_painter->undo();
}

void Container::deleteObject()
{
    m_painter->deleteCurrentObject();
}

void Container::reset()
{
    m_savedObjects.clear();
    m_currentObject->clear();
    m_painter->repaint();
}
