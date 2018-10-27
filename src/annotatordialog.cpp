#include "annotatordialog.h"
#include "ui_annotatordialog.h"

#include <QComboBox>
#include <QColorDialog>
#include <QtGlobal>
#include <QGraphicsItem>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScreen>
#include <QShortcut>
#include <QTime>

#include <QDebug>

#define POLYGON_BEGIN_RECT_DIST 10

AnnotatorDialog::AnnotatorDialog(QString file, QString outputPath, OutputType outputType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnnotatorDialog),
    m_file(file),
    m_outputPath(outputPath),
    m_outputType(outputType),
    m_classQuantity(1),
    m_currentPolygon(AnnotatorItem(Qt::GlobalColor::red, 0))
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

    ui->saveButton->setIcon(QIcon::fromTheme("borderpainter"));
    ui->undoButton->setIcon(QIcon::fromTheme("edit-undo"));
    ui->restartButton->setIcon(QIcon::fromTheme("view-refresh"));
    ui->addClassButton->setIcon(QIcon::fromTheme("list-add"));
    ui->colorButton->setIcon(QIcon::fromTheme("color-picker"));

    setFixedHeight(int(scene->height() + 0.3 * scene->height()));
    setFixedWidth(int(scene->width() + 0.3 * scene->width()));

    adjustSize();

    if (width() < 550)
        setFixedWidth(550);

    connect(ui->saveButton, &QAbstractButton::clicked, this, &AnnotatorDialog::finishPolygon);
    connect(ui->undoButton, &QAbstractButton::clicked, this, &AnnotatorDialog::undo);
    connect(ui->colorButton, &QAbstractButton::clicked, this, &AnnotatorDialog::pickColor);
    connect(ui->addClassButton, &QPushButton::clicked, this, &AnnotatorDialog::addClass);
    connect(ui->restartButton, &QPushButton::clicked, this, &AnnotatorDialog::restartPolygons);
    connect(ui->classNumber, QOverload<int>::of(&QComboBox::activated), this, &AnnotatorDialog::changeClass);

    QShortcut *finishShortcut = new QShortcut(QKeySequence(Qt::Modifier::CTRL + Qt::Key::Key_S), this);
    connect(finishShortcut, &QShortcut::activated, [this](){ finishPolygon(false); });

    QShortcut *undoShortcut = new QShortcut(QKeySequence(Qt::Modifier::CTRL + Qt::Key::Key_U), this);
    connect(undoShortcut, &QShortcut::activated, [this](){ undo(false); });

    QShortcut *colorShortcut = new QShortcut(QKeySequence(Qt::Modifier::CTRL + Qt::Key::Key_Z), this);
    connect(colorShortcut, &QShortcut::activated, [this](){ pickColor(false); });

    QShortcut *restartShortcut = new QShortcut(QKeySequence(Qt::Modifier::CTRL + Qt::Key::Key_R), this);
    connect(restartShortcut, &QShortcut::activated, [this](){ restartPolygons(false); });

    QShortcut *addClassShortcut = new QShortcut(QKeySequence(Qt::Modifier::CTRL + Qt::Key::Key_N), this);
    connect(addClassShortcut, &QShortcut::activated, [this](){ addClass(false); });

    ui->saveButton->setEnabled(false);
    ui->undoButton->setEnabled(false);
    ui->restartButton->setEnabled(false);

    m_classColors << m_currentPolygon.color();

    updateClasses();
}

AnnotatorDialog::~AnnotatorDialog()
{
    delete ui;
}

void AnnotatorDialog::finishPolygon(bool checked)
{
    Q_UNUSED(checked)

    if (m_currentPolygon.isEmpty())
        return;
    else if (m_currentPolygon.isClosed()) {
        m_savedPolygons << m_currentPolygon;
        m_currentPolygon.clear();

        repaint();
    }
}

void AnnotatorDialog::undo(bool checked)
{
    Q_UNUSED(checked)

    if (!m_currentPolygon.isEmpty()) {
        m_currentPolygon.pop_back();

        repaint();
    }
}

void AnnotatorDialog::pickColor(bool checked)
{
    Q_UNUSED(checked)

    QColorDialog colorDialog(m_currentPolygon.color(), this);

    if (colorDialog.exec() == QDialog::DialogCode::Accepted) {
        m_currentPolygon.setColor(colorDialog.selectedColor());

        int index = ui->classNumber->currentIndex();

        m_classColors[index] = colorDialog.selectedColor();

        updateClasses(checked);

        repaint();
    }
}

void AnnotatorDialog::restartPolygons(bool checked)
{
    Q_UNUSED(checked)

    if (!m_savedPolygons.isEmpty()) {
        m_savedPolygons.clear();
        repaint();
    }
}

void AnnotatorDialog::updateClasses(bool checked)
{
    Q_UNUSED(checked)

    ui->classNumber->clear();

    for (qint8 i = 0; i < m_classQuantity; i++) {
        QPixmap colorPix(70, 45);
        colorPix.fill(m_classColors[i]);

        ui->classNumber->addItem(QIcon(colorPix), QString::number(i + 1));
    }

    ui->classNumber->setCurrentIndex(m_currentPolygon.itemClass());
}

void AnnotatorDialog::addClass(bool checked)
{
    m_classQuantity++;

    QTime time = QTime::currentTime();

    qsrand((uint)time.msec());

    m_classColors << QColor(qrand() % 256, qrand() % 256, qrand() % 256);

    updateClasses(checked);

    ui->classNumber->setCurrentIndex(m_classQuantity - 1);

    if (!m_currentPolygon.isEmpty()) {
        m_currentPolygon.setColor(m_classColors[m_classQuantity - 1]);
        m_currentPolygon.setItemClass(m_classQuantity - 1);

        repaint();
    }
}

void AnnotatorDialog::changeClass(int index)
{
    m_currentPolygon.setItemClass(index);
    m_currentPolygon.setColor(m_classColors[index]);

    if (!m_currentPolygon.isEmpty() || !m_savedPolygons.isEmpty())
        repaint();
}

bool AnnotatorDialog::eventFilter(QObject *watched, QEvent *event)
{
    QMouseEvent *mouseEv = dynamic_cast<QMouseEvent*>(event);

    if (mouseEv) {
        // TODO: Check if the touched region is the origin of a previous polygon
        // And if it is, you should set the selected polygon as the current one
        int diffW = int((ui->graphicsView->width() - ui->graphicsView->scene()->width()) / 2);
        int diffH = int((ui->graphicsView->height() - ui->graphicsView->scene()->height()) / 2);

        int posX = mouseEv->pos().x() - diffW;
        int posY = mouseEv->pos().y() - diffH;

        if (posX >= 0 && posX <= ui->graphicsView->scene()->width() && 
                posY >= 0 && posY <= ui->graphicsView->scene()->height()) {

            QPointF point(posX, posY);

            if (!m_currentPolygon.empty()) {
                QRectF rect(m_currentPolygon.first(),
                            QPointF(m_currentPolygon.first().x() + POLYGON_BEGIN_RECT_DIST, m_currentPolygon.first().y() + POLYGON_BEGIN_RECT_DIST));

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

    for (AnnotatorItem& polygon : m_savedPolygons)
        processItem(polygon, true);

    processItem(m_currentPolygon, false);

    ui->saveButton->setEnabled(m_currentPolygon.size() > 1 &&
                               m_currentPolygon.first() == m_currentPolygon.last());

    // TODO: Click on polygon first point and select its class

    ui->undoButton->setEnabled(!m_currentPolygon.isEmpty());
    ui->restartButton->setEnabled(!m_savedPolygons.isEmpty());
}

void AnnotatorDialog::processItem(AnnotatorItem& polygon, bool fill)
{
    if (fill) {
        QColor c(polygon.color());
        c.setAlpha(20);

        QGraphicsPolygonItem *pol = ui->graphicsView->scene()->addPolygon(polygon, QPen(polygon.color(), 2), QBrush(c));

        m_items << pol;
    }

    for (auto it = polygon.begin(); it != polygon.end(); it++) {
        QGraphicsRectItem *rect;

        if (it == polygon.begin()) {
            rect = new QGraphicsRectItem(0, 0, POLYGON_BEGIN_RECT_DIST, POLYGON_BEGIN_RECT_DIST);

            QBrush brush(polygon.color());

            rect->setBrush(brush);
            rect->setPos(*it);

            ui->graphicsView->scene()->addItem(rect);

            m_items << rect;
        }

        if ((it + 1) != polygon.end()) {
            QGraphicsLineItem* line = ui->graphicsView->scene()->addLine(QLineF(*it, *(it + 1)),
                                                                         QPen(QBrush(polygon.color()), 2));
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
