#include "container.h"

void Container::setObjClass(MarkedClass* objClass)
{
    m_currentObject.setClass(objClass);
    repaint();
}

QVector<MarkedObject> Container::savedObjects() const
{
    return m_savedObjects;
}
