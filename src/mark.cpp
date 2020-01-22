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

#include "mark.h"
#include "ui_mark.h"
#include "serializer.h"

#include <QAction>
#include <QDir>
#include <QMenu>
#include <QToolButton>
#include <QDebug>
#include <QFileSystemWatcher>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QKeySequence>
#include <QTime>
#include <QtGlobal>
#include <QColorDialog>
#include <QFontMetrics>

#include <QDebug>

marK::marK(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::marK),
    m_watcher(new QFileSystemWatcher(this)),
    m_currentDirectory("")
{
    m_ui->setupUi(this);

    QMenu *fileMenu = m_ui->menuBar->addMenu("File");

    QAction *openDirAction = fileMenu->addAction("Open Directory");
    openDirAction->setShortcut(QKeySequence(Qt::Modifier::CTRL + Qt::Key::Key_O));
    connect(openDirAction, &QAction::triggered, this, &marK::changeDirectory);

    QMenu *exportMenu = fileMenu->addMenu("Export");

    QAction *toXML = exportMenu->addAction("XML");
    connect(toXML, &QAction::triggered, this, &marK::saveToXml);

    QAction *toJson = exportMenu->addAction("JSON");
    connect(toJson, &QAction::triggered, this, &marK::saveToJson);

    QMenu *editMenu = m_ui->menuBar->addMenu("Edit");

    QAction *undoAction = editMenu->addAction("Undo");
    undoAction->setShortcut(QKeySequence(Qt::Modifier::CTRL + Qt::Key::Key_Z));
    connect(undoAction, &QAction::triggered, m_ui->annotatorWidget, &AnnotatorWidget::undo);

    m_ui->annotatorWidget->setMinimumSize(860, 650);

    updateFiles();
    addNewClass();

    connect(m_ui->listWidget, &QListWidget::currentItemChanged, this,
            static_cast<void (marK::*)(QListWidgetItem *, QListWidgetItem *)>(&marK::changeImage));

    connect(m_watcher, &QFileSystemWatcher::directoryChanged, this,
            static_cast<void (marK::*)(const QString &)>(&marK::updateFiles));

    connect(m_ui->newClassButton, &QPushButton::clicked, this, &marK::addNewClass);

    connect(m_ui->undoButton, &QPushButton::clicked, m_ui->annotatorWidget, &AnnotatorWidget::undo);

    connect(m_ui->comboBox, &QComboBox::editTextChanged, 
        [&](const QString & text) {
            m_ui->comboBox->setItemText(m_ui->comboBox->currentIndex(), text);
        }
    );

    connect(m_ui->comboBox, QOverload<int>::of(&QComboBox::activated), 
        [&](int index) {
            m_ui->annotatorWidget->setCurrentPolygonClass(m_polygonClasses[index]);
        }
    );

    connect(m_ui->selectClassColorButton, &QPushButton::clicked, this, &marK::selectClassColor);
}

void marK::updateFiles()
{
    updateFiles(m_currentDirectory);
}

void marK::updateFiles(const QString &path)
{
    QListWidgetItem *previousSelectedItem = m_ui->listWidget->currentItem();
    QString previousText;
    if (previousSelectedItem != nullptr)
        previousText = previousSelectedItem->text();

    m_ui->listWidget->clear();

    QDir resDirectory(path);
    QStringList images = resDirectory.entryList(QStringList() << "*.jpg" << "*.jpeg" << "*.JPG" <<
                                                "*.JPEG" << "*.png" << "*.PNG", QDir::Files);

    for (const QString &image : images) {
        QPixmap item_pix(QDir(path).filePath(image));
        item_pix = item_pix.scaledToWidth(20);

        QListWidgetItem *item = new QListWidgetItem(item_pix, image);
        m_ui->listWidget->addItem(item);

        if (previousText != "" and previousText == image) {
            int currentIndex = m_ui->listWidget->count() - 1;
            m_ui->listWidget->setCurrentRow(currentIndex);
            changeImage(currentIndex);
        }
    }

    if (previousText == "")
        m_ui->annotatorWidget->clear();
}

void marK::changeImage(int currentRow)
{
    QListWidgetItem *currentItem = m_ui->listWidget->item(currentRow);
    changeImage(currentItem, nullptr);
}

void marK::changeImage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current != nullptr) {
        QString imagePath = QDir(m_currentDirectory).filePath(current->text());

        if (imagePath != m_filepath) {
            m_filepath = imagePath;
            m_ui->annotatorWidget->changeImage(imagePath);
        }
    }
}

void marK::changeDirectory()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select Directory", QDir::homePath(),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!path.isEmpty()) {
        if (m_currentDirectory != "")
            m_watcher->removePath(m_currentDirectory);
        m_currentDirectory = path;
        m_watcher->addPath(m_currentDirectory);
        updateFiles();

        QFontMetrics metrics(m_ui->listLabel->font());
        QString elidedText = metrics.elidedText(m_currentDirectory, Qt::ElideMiddle,
                                                m_ui->listLabel->maximumWidth() - int(m_ui->listLabel->maximumWidth() * 0.05));
        m_ui->listLabel->setText(elidedText);
        m_ui->listLabel->setToolTip(m_currentDirectory);
    }
}

void marK::addNewClass()
{
    int classQt = m_polygonClasses.size();

    PolygonClass* newClass = new PolygonClass(QString::number(classQt));
    m_polygonClasses << newClass;
    
    QPixmap colorPix(70, 45);
    colorPix.fill(newClass->color());

    m_ui->comboBox->addItem(QIcon(colorPix), newClass->name());
    m_ui->comboBox->setCurrentIndex(classQt);

    m_ui->annotatorWidget->setCurrentPolygonClass(newClass);
}

void marK::selectClassColor()
{
    qsrand((uint)QTime::currentTime().msec());
    QColorDialog colorDialog(QColor(qrand() % 256, qrand() % 256, qrand() % 256), this);

    if (colorDialog.exec() == QDialog::DialogCode::Accepted) {
        QPixmap colorPix(70, 45);
        colorPix.fill(colorDialog.selectedColor());
        m_ui->comboBox->setItemIcon(m_ui->comboBox->currentIndex(), QIcon(colorPix));

        m_polygonClasses[m_ui->comboBox->currentIndex()]->setColor(colorDialog.selectedColor());
    }

    m_ui->annotatorWidget->repaint();
}

void marK::savePolygons(OutputType type)
{
    QString document;

    if (type == OutputType::XML)
        document = Serializer::toXML(m_ui->annotatorWidget->savedPolygons());
    else if (type == OutputType::JSON)
        document = Serializer::toJSON(m_ui->annotatorWidget->savedPolygons());

    if (document != nullptr)
    {
        QString outputFile(m_filepath);
        outputFile.replace(QRegularExpression(".jpg|.png|.xpm"), (type == OutputType::XML ? ".xml" : ".json"));

        QFile fileOut(outputFile);

        if (!fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        fileOut.write(document.toUtf8());

        fileOut.close();
    }
}

marK::~marK() = default;