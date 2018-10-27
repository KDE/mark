#include "annotatordialog.h"
#include "ui_annotatordialog.h"

#include <QGraphicsItem>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScreen>

AnnotatorDialog::AnnotatorDialog(QString file, QString outputPath, OutputType outputType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnnotatorDialog),
    m_file(file),
    m_outputPath(outputPath),
    m_outputType(outputType)
{
    ui->setupUi(this);

    setWindowTitle("QAnnotator - " + file);

    if (!outputPath.endsWith('/'))
        outputPath.append('/');

    QGraphicsScene *scene = new QGraphicsScene;
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    QPixmap image(m_file);
    QRect screen = QGuiApplication::primaryScreen()->geometry();

    if (image.height() > screen.height())
        image = image.scaledToHeight(int(screen.height() - screen.height() * 0.3));

    if (image.width() > screen.width())
        image = image.scaledToWidth(int(screen.width() - screen.width() * 0.3));

    scene->addPixmap(image);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->installEventFilter(this);
    ui->graphicsView->adjustSize();

    ui->saveButton->setIcon(QIcon::fromTheme("document-save"));
    ui->undoButton->setIcon(QIcon::fromTheme("edit-undo"));

    setFixedHeight(int(scene->height() + 0.2 * scene->height()));
    setFixedWidth(int(scene->width() + 0.2 * scene->width()));

    connect(ui->saveButton, &QAbstractButton::clicked, this, &AnnotatorDialog::savePolygon);
    connect(ui->undoButton, &QAbstractButton::clicked, this, &AnnotatorDialog::undo);

    ui->saveButton->setEnabled(false);
    ui->undoButton->setEnabled(false);
}

AnnotatorDialog::~AnnotatorDialog()
{
    delete ui;
}

void AnnotatorDialog::savePolygon(bool checked)
{
    Q_UNUSED(checked);

    if (!m_currentPolygon.isEmpty() && m_currentPolygon.isClosed()) {
        m_savedPolygons << m_currentPolygon;
        m_currentPolygon.clear();
    }

    repaint();
}

void AnnotatorDialog::undo(bool checked)
{
    Q_UNUSED(checked);

    m_currentPolygon.pop_back();

    repaint();
}

bool AnnotatorDialog::eventFilter(QObject *watched, QEvent *event)
{
    QMouseEvent *mouseEv = dynamic_cast<QMouseEvent*>(event);

    if (mouseEv) {
        int diffW = int((ui->graphicsView->width() - ui->graphicsView->scene()->width()) / 2);
        int diffH = int((ui->graphicsView->height() - ui->graphicsView->scene()->height()) / 2);

        int posX = mouseEv->pos().x() - diffW;
        int posY = mouseEv->pos().y() - diffH;

        if (posX >= 0 && posX <= ui->graphicsView->scene()->width() && 
                posY >= 0 && posY <= ui->graphicsView->scene()->height()) {

            QPointF point(posX, posY);

            if (!m_currentPolygon.empty()) {
                QRectF rect(m_currentPolygon.first(),
                            QPointF(m_currentPolygon.first().x() + 10, m_currentPolygon.first().y() + 10));

                if (rect.contains(point))
                    point = QPointF(m_currentPolygon.first());
            }

            m_currentPolygon << point;
            repaint();
        }
    }

    return QDialog::eventFilter(watched, event);
}

void AnnotatorDialog::repaint()
{
    for (QGraphicsItem* item : m_items)
        ui->graphicsView->scene()->removeItem(item);

    for (QPolygonF polygon : m_savedPolygons)
        processPolygon(polygon, Qt::GlobalColor::blue, true);

    processPolygon(m_currentPolygon, Qt::GlobalColor::red);

    ui->saveButton->setEnabled(m_currentPolygon.size() > 1 &&
                               m_currentPolygon.first() == m_currentPolygon.last());

    ui->undoButton->setEnabled(!m_currentPolygon.empty());
}

void AnnotatorDialog::processPolygon(QPolygonF& polygon, Qt::GlobalColor color, bool fill)
{
    if (fill) {
        QColor c(color);
        c.setAlpha(20);

        QGraphicsPolygonItem *pol = ui->graphicsView->scene()->addPolygon(polygon, QPen(color, 2), QBrush(c));

        m_items << pol;
    }

    for (auto it = polygon.begin(); it != polygon.end(); it++) {
        QGraphicsRectItem *rect;

        if (it == polygon.begin())
            rect = new QGraphicsRectItem(0, 0, 10, 10);
        else
            rect = new QGraphicsRectItem(0, 0, 1, 1);

        QBrush brush(color);

        rect->setBrush(brush);
        rect->setPos(*it);

        ui->graphicsView->scene()->addItem(rect);

        m_items << rect;

        if ((it + 1) != polygon.end()) {
            QGraphicsLineItem* line = ui->graphicsView->scene()->addLine(QLineF(*it, *(it + 1)),
                                                                         QPen(QBrush(color), 2));
            m_items << line;
        }
    }
}

void AnnotatorDialog::accept()
{
    if (m_outputType == OutputType::XML)
        saveXML();
    else if (m_outputType == OutputType::JSON)
        saveJSON();
    else {
       QMessageBox message(this);
       message.setWindowTitle("Error");
       message.setText("Unknown output type");
       message.exec();
    }

    QDialog::accept();
}

void AnnotatorDialog::saveXML()
{
    // TODO: Salvar como XML
}

void AnnotatorDialog::saveJSON()
{
    // TODO: Salvar como JSON
}
