#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "polygon.h"

#include <QVector>
#include <QString>

class Serializer
{
public:
    static QString toJSON(const QVector<Polygon>& annotatedPolygons);
    static QString toXML(const QVector<Polygon>& annotatedPolygons);
};
#endif // SERIALIZER_H
