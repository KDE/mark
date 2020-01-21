#ifndef POLYGONCLASS_H
#define POLYGONCLASS_H

#include <QColor>
#include <QString>

class PolygonClass
{
public:
    explicit PolygonClass(QString text);
    explicit PolygonClass(QString text, QColor color);
    
public:
    QString text() const { return m_text; }
    QColor color() const { return m_color; }
    
    void setText(const QString& text) { m_text = text; }
    void setColor(const QColor& color) { m_color = color; }

private:
    QString m_text;
    QColor m_color;
};

#endif // POLYGONCLASS_H
