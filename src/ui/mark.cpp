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

#include "ui/mark.h"
#include "ui/ui_mark.h"

#include <QAction>
#include <QActionGroup>
#include <QDir>
#include <QMenu>
#include <QToolButton>
#include <QFileSystemWatcher>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QKeySequence>
#include <QRandomGenerator>
#include <QtGlobal>
#include <QColorDialog>
#include <QFontMetrics>
#include <QMessageBox>
#include <QShortcut>

static QDir markDirectory()
{
    return QDir(QDir::tempPath().append("/mark"));
}

marK::marK(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::marK),
    m_watcher(new QFileSystemWatcher(this)),
    m_currentDirectory(""),
    m_filepath(""),
    m_autoSaveType(Serializer::OutputType::None)
{
    m_ui->setupUi(this);

    setupActions();
    setupConnections();

    updateFiles();
    addNewClass();

    if (!markDirectory().exists())
        markDirectory().mkpath(".");
}

void marK::setupActions()
{
    QMenu *fileMenu = m_ui->menuBar->addMenu("File");

    QAction *openDirAction = fileMenu->addAction("Open Directory");
    openDirAction->setShortcut(QKeySequence(Qt::Modifier::CTRL + Qt::Key::Key_O));
    connect(openDirAction, &QAction::triggered, this, &marK::changeDirectory);

    QAction *importData = fileMenu->addAction("Import");
    importData->setShortcut(QKeySequence(Qt::Modifier::CTRL + Qt::Key::Key_I));
    connect(importData, &QAction::triggered, this, &marK::importData);

    QMenu *exportMenu = fileMenu->addMenu("Export");

    QAction *toXML = exportMenu->addAction("XML");
    connect(toXML, &QAction::triggered, [&](){ saveObjects(Serializer::OutputType::XML); });

    QAction *toJson = exportMenu->addAction("JSON");
    connect(toJson, &QAction::triggered, [&](){ saveObjects(Serializer::OutputType::JSON); });

    QMenu *editMenu = m_ui->menuBar->addMenu("Edit");

    QAction *undoAction = editMenu->addAction("Undo");
    undoAction->setShortcut(QKeySequence(Qt::Modifier::CTRL + Qt::Key::Key_Z));
    connect(undoAction, &QAction::triggered, m_ui->containerWidget, &ImageContainer::undo);

    m_ui->containerWidget->setMinimumSize(860, 650);
    QMenu *autoSaveMenu = editMenu->addMenu("Auto Save");

    QActionGroup *autoSaveActionGroup = new QActionGroup(this);

    QAction *autoSaveJsonButton = autoSaveMenu->addAction("JSON");
    autoSaveJsonButton->setCheckable(true);
    connect(autoSaveJsonButton, &QAction::triggered, this, &marK::toggleAutoSave);
    autoSaveJsonButton->setActionGroup(autoSaveActionGroup);

    QAction *autoSaveXmlButton = autoSaveMenu->addAction("XML");
    autoSaveXmlButton->setCheckable(true);
    connect(autoSaveXmlButton, &QAction::triggered, this, &marK::toggleAutoSave);
    autoSaveXmlButton->setActionGroup(autoSaveActionGroup);

    QAction *autoSaveDisableButton = autoSaveMenu->addAction("Disabled");
    autoSaveDisableButton->setCheckable(true);
    autoSaveDisableButton->setChecked(true);
    connect(autoSaveDisableButton, &QAction::triggered, this, &marK::toggleAutoSave);
    autoSaveDisableButton->setActionGroup(autoSaveActionGroup);

    QShortcut *nextItemShortcut = new QShortcut(this);
    nextItemShortcut->setKey(Qt::Key_Down);
    connect(nextItemShortcut, &QShortcut::activated, [&](){ changeIndex(1); });

    QShortcut *previousItemShortcut = new QShortcut(this);
    previousItemShortcut->setKey(Qt::Key_Up);
    connect(previousItemShortcut, &QShortcut::activated, [&]() { changeIndex(-1); });
}

void marK::setupConnections()
{
    connect(m_ui->listWidget, &QListWidget::currentItemChanged, this,
            qOverload<QListWidgetItem*, QListWidgetItem*>(&marK::changeItem));

    connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, [=](){ marK::updateFiles(); });

    connect(m_ui->newClassButton, &QPushButton::clicked, this, qOverload<>(&marK::addNewClass));

    connect(m_ui->undoButton, &QPushButton::clicked, m_ui->containerWidget, &ImageContainer::undo);
    connect(m_ui->resetButton, &QPushButton::clicked, m_ui->containerWidget, &ImageContainer::reset);

    connect(m_ui->comboBox, &QComboBox::editTextChanged, this, 
        [&](const QString & text) {
            if (m_ui->comboBox->count() == 0) return;

            m_ui->comboBox->setItemText(m_ui->comboBox->currentIndex(), text);
            m_objClasses[m_ui->comboBox->currentIndex()]->setName(text);
        }
    );

    connect(m_ui->comboBox, QOverload<int>::of(&QComboBox::activated), this, 
        [&](int index) {
            m_ui->containerWidget->setObjClass(m_objClasses[index]);
        }
    );

    connect(m_ui->selectClassColorButton, &QPushButton::clicked, this, &marK::selectClassColor);

    connect(m_ui->polygonButton, &QPushButton::clicked, this,
        [&](bool checked) {
        // probably temporary, made this so Shape can be in imagecontainer
            auto polygonShape = m_ui->containerWidget->Shape::Polygon;
            m_ui->containerWidget->setShape(polygonShape);
        }
    );

    connect(m_ui->rectButton, &QPushButton::clicked, this,
        [&](bool checked) {
            auto rectangleShape = m_ui->containerWidget->Shape::Rectangle;
            m_ui->containerWidget->setShape(rectangleShape);
        }
    );
}

void marK::updateFiles()
{
    updateFiles(m_currentDirectory);
}

void marK::updateFiles(const QString &path)
{
    int index = m_ui->listWidget->currentRow();
    m_ui->listWidget->clear();

    QDir resDirectory(path);
    QStringList items = resDirectory.entryList(QStringList() << "*.jpg" << "*.JPG" 
                                                             << "*.jpeg" << "*.JPEG"
                                                             << "*.png" << "*.PNG"
                                                             << "*.txt" << "*.TXT", QDir::Files);

    for (const QString &item : qAsConst(items)) {
        QPixmap item_pix = (item.endsWith(".txt") || item.endsWith(".TXT")) ? 
                            QIcon::fromTheme("document-edit-sign").pixmap(20, 20) :
                            QPixmap(resDirectory.filePath(item));

        item_pix = item_pix.scaledToWidth(20);

        QListWidgetItem *itemW = new QListWidgetItem(item_pix, item);
        m_ui->listWidget->addItem(itemW);
    }

    m_ui->listWidget->setCurrentRow(index);
}

void marK::changeIndex(const int count)
{
    int newIndex = m_ui->listWidget->currentRow() + count;
    if (newIndex >= m_ui->listWidget->count())
        newIndex = 0;

    else if (newIndex < 0)
        newIndex = m_ui->listWidget->count() - 1;

    m_ui->listWidget->setCurrentRow(newIndex);
    QListWidgetItem *currentItem = m_ui->listWidget->item(newIndex);
    changeItem(currentItem, nullptr);
}

void marK::changeItem(QListWidgetItem *current, QListWidgetItem *previous)
{
    // TODO: verify file extension and change annotatorwidget according to that
    if (current != nullptr) {
        QString itemPath = QDir(m_currentDirectory).filePath(current->text());

        if (itemPath != m_filepath) {
            makeTempFile();

            if (m_autoSaveType != Serializer::OutputType::None)
                autoSave();

            m_filepath = itemPath;
            m_ui->containerWidget->changeItem(itemPath);
            retrieveTempFile();
        }
    }
}

void marK::changeDirectory()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select Directory", QDir::homePath(),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (m_currentDirectory == path)
        return;

    if (!path.isEmpty()) {
        if (m_currentDirectory != "")
            m_watcher->removePath(m_currentDirectory);

        m_currentDirectory = path;
        m_watcher->addPath(m_currentDirectory);
        m_ui->containerWidget->reset();
        m_filepath.clear();
        updateFiles(path);

        QFontMetrics metrics(m_ui->listLabel->font());
        QString elidedText = metrics.elidedText(m_currentDirectory, Qt::ElideMiddle,
                                                m_ui->listLabel->maximumWidth() - int(m_ui->listLabel->maximumWidth() * 0.05));

        m_ui->listLabel->setText(elidedText);
        m_ui->listLabel->setToolTip(m_currentDirectory);
    }
}

void marK::addNewClass()
{
    auto classSize = QString::number(m_objClasses.size() + 1);
    MarkedClass* newClass = new MarkedClass(classSize);
    m_objClasses << newClass;
    
    QPixmap colorPix(70, 45);
    colorPix.fill(newClass->color());

    m_ui->comboBox->addItem(QIcon(colorPix), newClass->name());
    m_ui->comboBox->setCurrentIndex(m_objClasses.size() - 1);

    m_ui->containerWidget->setObjClass(newClass);
}

void marK::updateComboBox()
{
    m_ui->comboBox->clear();

    for (const auto& markedClass : m_objClasses) {
        QPixmap colorPix(70, 45);
        colorPix.fill(markedClass->color());
        m_ui->comboBox->addItem(QIcon(colorPix), markedClass->name());
    }

    m_ui->comboBox->setCurrentIndex(m_objClasses.size() - 1);
    MarkedClass* currentClass = m_objClasses[m_ui->comboBox->currentIndex() - 1];
    m_ui->containerWidget->setObjClass(currentClass);
}

void marK::selectClassColor()
{
    auto rand = QRandomGenerator().global();
    QColorDialog colorDialog(QColor(rand->bounded(0, 256), rand->bounded(0, 256), rand->bounded(0, 256)), this);

    if (colorDialog.exec() == QDialog::DialogCode::Accepted) {
        QPixmap colorPix(70, 45);
        colorPix.fill(colorDialog.selectedColor());
        m_ui->comboBox->setItemIcon(m_ui->comboBox->currentIndex(), QIcon(colorPix));

        m_objClasses[m_ui->comboBox->currentIndex()]->setColor(colorDialog.selectedColor());
    }

    m_ui->containerWidget->repaint();
}

void marK::saveObjects(Serializer::OutputType type)
{
    // FIXME, either do not need outputtype in its parameters or have
    // one specific for each outputtype
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                           m_currentDirectory,
                           tr("JSON and XML files (*.json *.xml)"));


    Serializer serializer = Serializer(m_ui->containerWidget->savedObjects());
    bool success = serializer.write(fileName, type);

    if (!success) {
        QMessageBox msgBox;
        msgBox.setText("failed to save annotation");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
}

void marK::importData()
{
    if (m_filepath.isEmpty()) return; //exiting because this is no image loaded

    QString filepath = QFileDialog::getOpenFileName(this, "Select File", m_currentDirectory,
                                                     "JSON and XML files (*.json *.xml)");

    Serializer serializer = Serializer(&m_objClasses);
    auto objects = serializer.read(filepath);

    auto success = m_ui->containerWidget->importObjects(objects);

    if (!success) {
        QMessageBox msgBox;
        msgBox.setText("failed to load annotation");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    updateComboBox();
}

void marK::retrieveTempFile()
{
    QString tempFilePath = markDirectory().filePath(QString(m_filepath).replace("/", "_"));
    tempFilePath.replace(QRegularExpression(".jpg|.jpeg|.png|.xpm|.txt"), ".json");

    if (!QFile::exists(tempFilePath)) return;

    Serializer serializer = Serializer(&m_objClasses);
    auto objects = serializer.read(tempFilePath);

    auto success = m_ui->containerWidget->importObjects(objects);

    if (!success) return;

    updateComboBox();

    m_ui->containerWidget->repaint();
}

void marK::makeTempFile()
{
    QString tempFilePath = markDirectory().filePath(QString(m_filepath).replace("/", "_"));

    Serializer serializer = Serializer(m_ui->containerWidget->savedObjects());
    serializer.write(tempFilePath, Serializer::OutputType::JSON);
}

void marK::toggleAutoSave()
{
    QAction *button = qobject_cast<QAction*>(sender());
    QString type = button->text();

    if (type == "Disabled")
        m_autoSaveType = Serializer::OutputType::None;

    else if (type == "XML")
        m_autoSaveType = Serializer::OutputType::XML;

    else if (type == "JSON")
        m_autoSaveType = Serializer::OutputType::JSON;

}

void marK::autoSave()
{
    Serializer serializer = Serializer(m_ui->containerWidget->savedObjects());
    serializer.write(m_filepath, m_autoSaveType);
}

marK::~marK()
{
    if (markDirectory().exists())
        markDirectory().removeRecursively();
}
