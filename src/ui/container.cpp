#include "container.h"
#include "image/imagepainter.h"
#include "image/polygon.h"
#include "text/textpainter.h"
#include "util/fileutils.h"

#include <QGraphicsScene>
#include <QMouseEvent>

Container::Container(QWidget* parent) :
    QGraphicsView(parent),
    m_currentObject(new Polygon),
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

void Container::changeItem(const QString& filepath)
{
    reset();

    scene()->setSceneRect(0, 0, 850, 640);
    scene()->clear();

    if (m_painterType != PainterType::Text && FileUtils::isTextFile(filepath)) {
        m_painter = std::make_unique<TextPainter>(this);
        m_painterType = PainterType::Text;
        emit painterChanged(m_painterType);
    }
    else if (m_painterType != PainterType::Image && FileUtils::isImageFile(filepath)) {
        m_painter = std::make_unique<ImagePainter>(this);
        m_painterType = PainterType::Image;
        emit painterChanged(m_painterType);
    }

    m_painter->changeItem(filepath);
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
    if (m_painterType != PainterType::None)
        m_painter->repaint();
}

void Container::clear()
{
    reset();
    scene()->clear();
    m_painter.reset();
    m_painterType = PainterType::None;
    emit painterChanged(m_painterType);
}
