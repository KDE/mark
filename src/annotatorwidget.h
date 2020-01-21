#ifndef ANNOTATORWIDGET_H
#define ANNOTATORWIDGET_H

#include "polygon.h"

#include <QVector>
#include <QWidget>

namespace Ui {
class AnnotatorWidget;
}

class QGraphicsPixmapItem;

class AnnotatorWidget : public QWidget
{
public:
    enum class Shape {
        Polygon,
        Rectangle,
        Circle,
        Triangle,
    };

public:
    explicit AnnotatorWidget(QWidget* parent = nullptr);
    
    ~AnnotatorWidget() override;

public:
    QVector<Polygon> savedPolygons() const { return m_savedPolygons; }
    
    bool eventFilter(QObject* watched, QEvent* event) override;
    void changeImage(QString imagePath);
    void clear();
    void repaint();
    void paintPolygon(const Polygon& polygon);

    // FIXME
    void setCurrentPolygonClass(PolygonClass* polygonClass) { m_currentPolygon.setPolygonClass(polygonClass); repaint(); }
    void setShape(Shape shape) { m_shape = shape; }

public slots:
    void undo();
    
private:
    Ui::AnnotatorWidget* m_ui;
    Polygon m_currentPolygon;
    QVector<Polygon> m_savedPolygons;
    QVector<QGraphicsItem*> m_items;
    QGraphicsPixmapItem* m_currentImage;
    Shape m_shape;
};

#endif // ANNOTATORWIDGET_H
