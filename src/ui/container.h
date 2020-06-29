#ifndef CONTAINER_H
#define CONTAINER_H

#include "ui/markedclass.h"
#include "ui/markedobject.h"
//#include "image/imagepainter.h"

#include <QGraphicsView>
#include <QVector>

class ImagePainter;

/** Annotation container. */
class Container : public QGraphicsView
{
    
    friend class ImagePainter;
    
public:
    explicit Container(QWidget* parent = nullptr);
    ~Container() override;
    
public:
    /** Treat mouse press event.
     * @param event - mouse event to treat.
     */
    void mousePressEvent(QMouseEvent* event) override;
    
    /** @return pointer of the current Painter. */
    ImagePainter* painter() const { return m_painter; }
    
public:
    /** Set MarkedClass of the current object.
     * @param objClass - Class to define.
     */
    void setObjClass(MarkedClass* objClass);

public:
    /** @return saved annotated objects. */
    QVector<MarkedObject*> savedObjects() const;

    /** @return pointer of current MarkedObject. */
    MarkedObject* currentObject() const { return m_currentObject; }

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

protected:
    MarkedObject* m_currentObject;
    QVector<MarkedObject*> m_savedObjects;
    
    ImagePainter* m_painter;
};

#endif // CONTAINER_H
