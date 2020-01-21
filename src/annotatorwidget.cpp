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

#include "annotatorwidget.h"
#include "ui_annotatorwidget.h"

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QMouseEvent>

#include <QDebug>

AnnotatorWidget::AnnotatorWidget(QWidget* parent):
    QWidget(parent),
    m_ui(new Ui::AnnotatorWidget),
    m_currentImage(nullptr),
    m_shape(AnnotatorWidget::Shape::Polygon)
{
    m_ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_ui->graphicsView->setScene(scene);
    m_ui->graphicsView->setMinimumSize(860, 600);

    m_ui->graphicsView->installEventFilter(this);
}

AnnotatorWidget::~AnnotatorWidget()
{
}

bool AnnotatorWidget::eventFilter(QObject* watched, QEvent* event)
{
    QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);

    if (m_currentImage != nullptr && mouseEvent != nullptr) {
        QPoint point = mouseEvent->pos();
        QPointF clickedPoint = m_ui->graphicsView->mapToScene(point);

        auto isFirstPolClicked = [&](const QPointF& p) {
            if (!m_currentPolygon.empty()) {
                qreal firstX = m_currentPolygon.first().x() + 10;
                qreal firstY = m_currentPolygon.first().y() + 10;
                
                QRectF rect(m_currentPolygon.first(), QPointF(firstX, firstY));

                return rect.contains(p);
            }
            return false;
        };

        if (isFirstPolClicked(clickedPoint) ||
                m_ui->graphicsView->scene()->itemAt(clickedPoint, m_ui->graphicsView->transform()) == m_currentImage) {

            // check if any other polygon has been clicked
            auto it = std::find_if(
                m_savedPolygons.begin(), m_savedPolygons.end(),
                [&](const Polygon& pol) {
                    qreal x = pol.first().x() + 10;
                    qreal y = pol.first().y() + 10;
                    QRectF rect(pol.first(), QPointF(x, y));

                    return rect.contains(clickedPoint);
                }
            );

            if (it != m_savedPolygons.end()) {
                m_currentPolygon = *it;
                m_savedPolygons.erase(it);
            }

            if (isFirstPolClicked(clickedPoint))
                clickedPoint = QPointF(m_currentPolygon.first());

            m_currentPolygon << clickedPoint;

            if (m_currentPolygon.size() > 1 && m_currentPolygon.isClosed()) {
                m_savedPolygons << m_currentPolygon;
                m_currentPolygon.clear();
            }

            repaint();
        }
    }

    return QWidget::eventFilter(watched, event);
}

void AnnotatorWidget::repaint()
{
    for (QGraphicsItem* item : m_items)
        m_ui->graphicsView->scene()->removeItem(item);

    m_items.clear();

    for (const Polygon& polygon : m_savedPolygons)
        paintPolygon(polygon);

    paintPolygon(m_currentPolygon);
}

void AnnotatorWidget::paintPolygon(const Polygon& polygon)
{
    QGraphicsScene *scene = m_ui->graphicsView->scene();

    if (polygon.size() > 1 && polygon.isClosed()) {
        QColor color(polygon.polygonClass()->color());
        color.setAlpha(35);

        QGraphicsPolygonItem *pol = scene->addPolygon(polygon, QPen(polygon.polygonClass()->color(), 2), QBrush(color));

        m_items << pol;
    }
    else {
        for (auto it = polygon.begin(); it != polygon.end(); ++it) {
            QGraphicsItem* item;

            if (it == polygon.begin()) {
                QBrush brush(polygon.polygonClass()->color());

                item = scene->addRect((*it).x(), (*it).y(), 10, 10, QPen(brush, 2), brush);
            }
            else
                item = scene->addLine(QLineF(*(it - 1), *it), QPen(QBrush(polygon.polygonClass()->color()), 2));

            m_items << item;
        }
    }
}

void AnnotatorWidget::undo()
{
    if (!m_currentPolygon.empty()) {
        m_currentPolygon.pop_back();

        repaint();
    }
}

void AnnotatorWidget::changeImage(QString imagePath)
{
    // TODO: create a temp file where the polygons from this image will be temporary stored, so they can be loaded again when
    // the image is reopened
    m_savedPolygons.clear();
    m_items.clear();
    m_currentPolygon.clear();

    QGraphicsScene *scene = m_ui->graphicsView->scene();
    scene->setSceneRect(0, 0, 850, 640);
    clear();

    QPixmap image(imagePath);

    if (image.height() >= 960)
        image = image.scaledToHeight(int(960 * 0.6));
    if (image.width() >= 720)
        image = image.scaledToWidth(int(720 * 0.6));

    QGraphicsPixmapItem *pixmapItem = scene->addPixmap(image);

    int x_scene = scene->width() / 2;
    int y_scene = scene->height() / 2;
    int x_image = image.width() / 2;
    int y_image = image.height() / 2;

    pixmapItem->setPos(x_scene - x_image, y_scene - y_image);

    m_currentImage = pixmapItem;
}

void AnnotatorWidget::clear()
{
    m_ui->graphicsView->scene()->clear();
}
