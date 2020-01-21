#include "polygonclass.h"

#include <QTime>
#include <QtGlobal>

PolygonClass::PolygonClass(QString text) :
    m_text(text)
{
    qsrand((uint)QTime::currentTime().msec());

    m_color = QColor(qrand() % 256, qrand() % 256, qrand() % 256);
}

PolygonClass::PolygonClass(QString text, QColor color) :
    m_text(text),
    m_color(color)
{
}
