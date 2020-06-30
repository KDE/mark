#include "ui/painter.h"

Painter::Painter(Container* parent) :
    m_parent(parent)
{
}

Painter::~Painter()
{
    for (QGraphicsItem* item : m_items)
        m_parent->scene()->removeItem(item);

    m_items.clear();

    m_parent->scene()->removeItem(m_currentItem);
}
