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
    
    Painter* painter() const { return m_painter; }

public:
    /** Set MarkedClass of the current object.
     * @param objClass - Class to define.
     */
    void setObjClass(MarkedClass* objClass);

public:
    /** @return saved annotated objects. */
    QVector<MarkedObject*>& savedObjects() { return m_savedObjects; }

    MarkedObject* currentObject() { return m_currentObject; }
    void setCurrentObject(MarkedObject* currentObject) { m_currentObject = currentObject; }

    /** Load given item/file.
     * @param itemPath - Path of the item.
     */
    void changeItem(const QString& itemPath);

    /** Repaint annotated objects. */
    void repaint();

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

    /** Delete all annotated objects. */
    void reset();

signals:

    void changed(bool isEmpty);

protected:
    MarkedObject* m_currentObject;
    QVector<MarkedObject*> m_savedObjects;
    
    Painter* m_painter;
};

#endif // CONTAINER_H
