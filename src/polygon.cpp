#include "polygon.h"

#include <QBrush>
#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <QPen>

Polygon::Polygon(PolygonClass* polygonClass) :
    m_polygonClass(polygonClass)
{
}
