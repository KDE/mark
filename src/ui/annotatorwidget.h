/*************************************************************************
 *  Copyright (C) 2020 by Caio Jordão Carvalho <caiojcarvalho@gmail.com> *
 *                                                                       *
 *  This program is free software; you can redistribute it and/or        *
 *  modify it under the terms of the GNU General Public License as       *
 *  published by the Free Software Foundation; either version 3 of       *
 *  the License, or (at your option) any later version.                  *
 *                                                                       *
 *  This program is distributed in the hope that it will be useful,      *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 *************************************************************************/

#ifndef ANNOTATORWIDGET_H
#define ANNOTATORWIDGET_H

#include "ui/mark.h"
#include "image/polygon.h"

#include <QVector>
#include <QWidget>

namespace Ui {
class AnnotatorWidget;
}

class QGraphicsPixmapItem;

// TODO: create a imagecontainer and textcontainer
class AnnotatorWidget : public QWidget
{
public:
    explicit AnnotatorWidget(QWidget* parent = nullptr);
    ~AnnotatorWidget() override;

public:
    QVector<Polygon> savedPolygons() const;

    void mousePressEvent(QMouseEvent* event) override;

    void changeItem(QString itemPath);
    void clearScene();
    void repaint();
    void paintPolygon(Polygon& polygon);

    // FIXME
    void setCurrentPolygonClass(MarkedClass* polygonClass) { m_currentPolygon.setPolygonClass(polygonClass); repaint(); }
    void setShape(marK::Shape shape) { m_shape = shape; m_currentPolygon.clear(); repaint(); }

    // TODO: scale a point
    QPointF scaledPoint(const QPointF& point) const { return QPointF(point.x(), point.y()); }

public slots:
    void undo();
    void reset();
    
private:
    Ui::AnnotatorWidget* m_ui;
    Polygon m_currentPolygon;
    QVector<Polygon> m_savedPolygons;
    QVector<QGraphicsItem*> m_items;
    QGraphicsPixmapItem* m_currentImage;
    marK::Shape m_shape;

    qreal m_scaleW;
    qreal m_scaleH;
};

#endif // ANNOTATORWIDGET_H
