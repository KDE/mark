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
    QVector<Polygon> savedPolygons() const;

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
