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
    m_painter(nullptr),
    m_painterType(PainterType::None)
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

bool Container::eventFilter(QObject* watched, QEvent* event)
{
    QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent*>(event);
    if (!mouseEvent || event->type() == QEvent::Wheel || m_painterType == PainterType::None)
        return false;

    if (event->type() == QEvent::MouseButtonPress)
        m_painter->paint(mouseEvent->pos(), false);
    else if (event->type() == QEvent::MouseButtonDblClick) {
        if (m_painterType == PainterType::Text)
            m_painter->paint(mouseEvent->pos(), false);
    }
    else if (event->type() == QEvent::MouseMove)
        m_painter->paint(mouseEvent->pos(), true);
    else if (event->type() == QEvent::MouseButtonRelease)
        m_painter->paint(QPoint(), false);

    return true;
}

void Container::changeItem(const QString& path)
{
    reset();

    scene()->setSceneRect(0, 0, 850, 640);
    scene()->clear();

    if (path.endsWith(".txt") || path.endsWith(".TXT")) {
        if (m_painterType != PainterType::Text) {
            delete m_painter;
            m_painter = new TextPainter(this);
            m_painterType = PainterType::Text;
            Q_EMIT painterChanged(m_painterType);
        }
    }
    else {
        if (m_painterType != PainterType::Image) {
            delete m_painter;
            m_painter = new ImagePainter(this);
            m_painterType = PainterType::Image;
            Q_EMIT painterChanged(m_painterType);
        }
    }

    m_painter->changeItem(path);
}

void Container::setObjClass(MarkedClass* objClass)
{
    m_currentObject->setObjClass(objClass);
    repaint();
}

void Container::repaint()
{
    if (m_painterType != PainterType::None)
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
    m_tempObjects.clear();
    m_currentObject->clear();
    if (m_painterType != PainterType::None)
        m_painter->repaint();
}

void Container::clear()
{
    reset();
    scene()->clear();
    delete m_painter;
    m_painterType = PainterType::None;
    Q_EMIT painterChanged(m_painterType);
}
