#ifndef CONTAINER_H
#define CONTAINER_H

#include "core/markedclass.h"
#include "core/markedobject.h"

#include <memory>

#include <QGraphicsView>
#include <QVector>

class Painter;

/** Annotation container. */
class Container : public QGraphicsView
{
    Q_OBJECT
    
    friend class Painter;
    
public:
    explicit Container(QWidget* parent = nullptr);
    ~Container() override;

public:
    /** Represents the possible types of a Painter. */
    enum class PainterType {
        None,
        Image,
        Text
    };

    /** Filter events in the container, if it is a mouse event treat it, otherwise ignore.
     * @param watched - watched object.
     * @param event - event to treat.
     */
    bool eventFilter(QObject* watched, QEvent* event);

    /** @return pointer of the current Painter. */
    Painter* painter() const { return m_painter.get(); }

public:
    /** Set MarkedClass of the current object.
     * @param objClass - Class to define.
     */
    void setObjClass(MarkedClass* objClass);

public:
    /** @return saved annotated objects. */
    QVector<MarkedObject*>& savedObjects() { return m_savedObjects; }

    /** @return temporary annotated objects. */
    QVector<MarkedObject*>& tempObjects() { return m_tempObjects; }

    /** @return pointer of current MarkedObject. */
    MarkedObject* currentObject() { return m_currentObject; }

    /** Append MarkedObject instance to savedObjects. */
    void appendObject(MarkedObject* object);

    /** Set current object.
     * @param currentObject - object to set as current object.
     */
    void setCurrentObject(MarkedObject* currentObject) { m_currentObject = currentObject; }

    /** Load given item/file.
     * @param itemPath - Path of the item.
     */
    void changeItem(const QString& filepath);

    /** Repaint annotated objects. */
    void repaint();

    /** Clear all visual elements of Container. */
    void clear();

    /** Paint given object.
     * @param object - object to paint.
     */
    void paintObject(MarkedObject* object);

    /** Load given objects.
     * @param objects - Vector of annotated object to load.
     */
    bool importObjects(QVector<MarkedObject*> objects);

public slots:
    /** Undo last action. */
    void undo();

    /** Delete a MarkedObject's instance. */
    void deleteObject();

    /** Delete all annotated objects. */
    void reset();

signals:

    void painterChanged(PainterType painterType);

    void savedObjectsChanged();

protected:
    MarkedObject* m_currentObject;
    QVector<MarkedObject*> m_savedObjects;
    QVector<MarkedObject*> m_tempObjects;

    PainterType m_painterType;
    std::unique_ptr<Painter> m_painter;
};

#endif // CONTAINER_H
