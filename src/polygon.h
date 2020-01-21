#ifndef POLYGON_H
#define POLYGON_H

#include "polygonclass.h"

#include <QPolygonF>

class QGraphicsItem;

class Polygon : public QPolygonF
{
public:
    Polygon(PolygonClass* polygonClass = nullptr);
    
    PolygonClass* polygonClass() const { return m_polygonClass; }
    void setPolygonClass(PolygonClass* polygonClass) { m_polygonClass = polygonClass; }
    
private:
    PolygonClass* m_polygonClass;
};

#endif // POLYGON_H
