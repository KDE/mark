#ifndef ANNOTATORITEM_H
#define ANNOTATORITEM_H

#include <QColor>
#include <QPolygonF>

class AnnotatorItem : public QPolygonF
{
public:
    AnnotatorItem() : QPolygonF() { }

    AnnotatorItem(QColor color, int itemClass);
    
    AnnotatorItem(const AnnotatorItem& item);

public:

    QColor color() const { return m_color; }

    int itemClass() const { return m_itemClass; }

    void setColor(QColor color) { m_color = color; }

    void setItemClass(int itemClass) { m_itemClass = itemClass; }

private:
    QColor m_color;

    int m_itemClass;
};

#endif // ANNOTATORITEM_H
