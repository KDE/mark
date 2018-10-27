#include "annotatoritem.h"

AnnotatorItem::AnnotatorItem(QColor color, int itemClass) :
    QPolygonF(),
    m_color(color),
    m_itemClass(itemClass)
{
}

AnnotatorItem::AnnotatorItem(const AnnotatorItem& item) :
    QPolygonF(item),
    m_color(item.color()),
    m_itemClass(item.itemClass())
{
}
