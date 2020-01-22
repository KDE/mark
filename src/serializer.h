#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "polygon.h"

#include <QVector>
#include <QString>

class Serializer
{
public:
    static QString toJSON(QVector<Polygon> annotatedPolygons);
    static QString toXML(QVector<Polygon> annotatedPolygons);
};
#endif // SERIALIZER_H
