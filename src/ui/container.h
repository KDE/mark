#ifndef CONTAINER_H
#define CONTAINER_H

#include "ui/markedclass.h"
#include "ui/markedobject.h"

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
    /** Treat mouse press event.
     * @param event - mouse event to treat.
     */
    void mousePressEvent(QMouseEvent* event) override;

    /** Treat mouse release event.
     * @param event - mouse event to treat.
     */
    void mouseReleaseEvent(QMouseEvent* event) override;

    /** Treat mouse move event.
     * @param event - mouse event to treat.
     */
    void mouseMoveEvent(QMouseEvent* event) override;

    /** Treat mouse double click event.
     * @param event - mouse event to treat.
     */
    void mouseDoubleClickEvent(QMouseEvent* event) override;

    /** @return pointer of the current Painter. */
    Painter* painter() const { return m_painter; }

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
    void changeItem(const QString& itemPath);

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

    void painterChanged(bool shouldShapesBeVisible = true);

    void savedObjectsChanged();

protected:
    MarkedObject* m_currentObject;
    QVector<MarkedObject*> m_savedObjects;
    QVector<MarkedObject*> m_tempObjects;

    Painter* m_painter;
};

#endif // CONTAINER_H
