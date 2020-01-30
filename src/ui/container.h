#ifndef CONTAINER_H
#define CONTAINER_H

#include "ui/markedclass.h"
#include "ui/markedobject.h"

#include <QVector>

class Container
{
public:
    void setObjClass(MarkedClass* objClass);

public:
    virtual QVector<MarkedObject> savedObjects() const;

    virtual void changeItem(const QString& itemPath) = 0;
    virtual void repaint() = 0;
    virtual void paintObject(MarkedObject& object) = 0;

public slots:
    virtual void undo() = 0;
    virtual void reset() = 0;

protected:
    MarkedObject m_currentObject;
    QVector<MarkedObject> m_savedObjects;
};

#endif // CONTAINER_H
