#include "container.h"
#include "image/polygon.h"
#include "image/imagepainter.h"
#include "text/textpainter.h"
#include "text/sentence.h"

#include <QGraphicsScene>
#include <QMouseEvent>

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
}

Container::~Container()
{
}

void Container::mousePressEvent(QMouseEvent* event)
{
    lastPosition = event->pos();
    m_painter->paint(lastPosition);

    QWidget::mousePressEvent(event);
}

void Container::mouseMoveEvent(QMouseEvent* event)
{
    if (lastPosition != event->pos()) {
        lastPosition = event->pos();
        m_painter->paint(lastPosition);
    }

    QWidget::mouseMoveEvent(event);
}

void Container::mouseReleaseEvent(QMouseEvent* event)
{
    if (lastPosition != event->pos()) {
        lastPosition = event->pos();
        m_painter->paint(lastPosition);
    }

    QWidget::mouseReleaseEvent(event);
}

void Container::changeItem(const QString& path)
{
    reset();

    scene()->setSceneRect(0, 0, 850, 640);
    scene()->clear();

    //IMPROVEME: only change painter when a different type
    // of file is loaded, and not always
    if (path.endsWith(".txt") || path.endsWith(".TXT")) {
        emit painterChanged(false);
        Painter *oldPainter = m_painter;
        m_painter = new TextPainter(this);
        delete oldPainter;
    }
    else {
        emit painterChanged();
        Painter *oldPainter = m_painter;
        m_painter = new ImagePainter(this);
        delete oldPainter;
    }

    viewport()->setMouseTracking(false);
    m_painter->changeItem(path);
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
    emit savedObjectsChanged();
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
