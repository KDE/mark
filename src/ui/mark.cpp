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

#include "image/imagepainter.h"
#include "ui/container.h"
#include "ui/mark.h"
#include "ui/ui_mark.h"
#include "util/fileutils.h"

#include <QAction>
#include <QActionGroup>
#include <QColorDialog>
#include <QDir>
#include <QFileDialog>
#include <QFontMetrics>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QKeySequence>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QShortcut>
#include <QToolButton>
#include <QtGlobal>
#include <QtConcurrent/QtConcurrentRun>

#include <KActionCollection>
#include <QFutureWatcher>

static QDir markTempDirectory()
{
    return QDir(QDir::tempPath().append("/mark"));
}

marK::marK(QWidget *parent) :
    KXmlGuiWindow(parent),
    m_ui(new Ui::marK),
    m_watcher(new QFileSystemWatcher(this)),
    m_currentDirectory(QDir::currentPath()),
    m_filepath(""),
    m_autoSaveType(Serializer::OutputType::None)
{
    m_ui->setupUi(this);
    m_ui->listLabel->setText(m_currentDirectory);
    m_ui->listLabel->setToolTip(m_currentDirectory);
    m_ui->containerWidget->setMinimumSize(860, 650);

    setupActions();
    setupConnections();
    setupGUI(Default);

    updateFiles();
    addNewClass();

    if (!markTempDirectory().exists())
        markTempDirectory().mkpath(".");
}

void marK::setupActions()
{
    QAction *openDirAction = new QAction(this);
    openDirAction->setText("Open Directory");
    openDirAction->setIcon(QIcon::fromTheme("document-open"));
    actionCollection()->setDefaultShortcut(openDirAction, Qt::Modifier::CTRL + Qt::Key::Key_O);
    actionCollection()->addAction("openDirectory", openDirAction);
    connect(openDirAction, &QAction::triggered, this, &marK::changeDirectory);

    QAction *importData = new QAction(this);
    importData->setText("Import");
    actionCollection()->setDefaultShortcut(importData, Qt::Modifier::CTRL + Qt::Key::Key_I);
    actionCollection()->addAction("importData", importData);
    connect(importData, &QAction::triggered, this, &marK::importData);

    QAction *toXML = new QAction(this);
    toXML->setText("XML");
    actionCollection()->addAction("toXML", toXML);
    connect(toXML, &QAction::triggered, [&](){ saveObjects(Serializer::OutputType::XML); });

    QAction *toJson = new QAction(this);
    toJson->setText("JSON");
    actionCollection()->addAction("toJSON", toJson);
    connect(toJson, &QAction::triggered, [&](){ saveObjects(Serializer::OutputType::JSON); });

    QAction *undoAction = new QAction(this);
    undoAction->setText("Undo");
    actionCollection()->setDefaultShortcut(undoAction, Qt::Modifier::CTRL + Qt::Key::Key_Z);
    actionCollection()->addAction("undo", undoAction);
    connect(undoAction, &QAction::triggered, m_ui->containerWidget, &Container::undo);

    QAction *deleteAction  = new QAction(this);
    deleteAction->setText("Delete");
    actionCollection()->setDefaultShortcut(deleteAction, Qt::Modifier::CTRL + Qt::Key::Key_D);
    actionCollection()->addAction("delete", deleteAction);
    connect(deleteAction, &QAction::triggered, m_ui->containerWidget, &Container::deleteObject);

    QActionGroup *autoSaveActionGroup = new QActionGroup(this);

    QAction *autoSaveJsonButton = new QAction(this);
    autoSaveJsonButton->setText("JSON");
    autoSaveJsonButton->setCheckable(true);
    autoSaveJsonButton->setActionGroup(autoSaveActionGroup);
    actionCollection()->addAction("autosaveJSON", autoSaveJsonButton);
    connect(autoSaveJsonButton, &QAction::triggered, this, &marK::toggleAutoSave);

    QAction *autoSaveXmlButton = new QAction(this);
    autoSaveXmlButton->setText("XML");
    autoSaveXmlButton->setCheckable(true);
    autoSaveXmlButton->setActionGroup(autoSaveActionGroup);
    actionCollection()->addAction("autosaveXML", autoSaveXmlButton);
    connect(autoSaveXmlButton, &QAction::triggered, this, &marK::toggleAutoSave);

    QAction *autoSaveDisabledButton = new QAction(this);
    autoSaveDisabledButton->setText("Disabled");
    autoSaveDisabledButton->setCheckable(true);
    autoSaveDisabledButton->setChecked(true);
    autoSaveDisabledButton->setActionGroup(autoSaveActionGroup);
    actionCollection()->addAction("autosaveDisabled", autoSaveDisabledButton);
    connect(autoSaveDisabledButton, &QAction::triggered, this, &marK::toggleAutoSave);

    QShortcut *nextItemShortcut = new QShortcut(this);
    nextItemShortcut->setKey(Qt::Key_Down);
    connect(nextItemShortcut, &QShortcut::activated, [&](){ changeIndex(1); });

    QShortcut *previousItemShortcut = new QShortcut(this);
    previousItemShortcut->setKey(Qt::Key_Up);
    connect(previousItemShortcut, &QShortcut::activated, [&]() { changeIndex(-1); });

    QShortcut *zoomInShortcut = new QShortcut(this);
    zoomInShortcut->setKey(Qt::Modifier::CTRL + Qt::Key::Key_Equal);
    connect(zoomInShortcut, &QShortcut::activated, [&]() {
        ImagePainter *painter = dynamic_cast<ImagePainter*>(m_ui->containerWidget->painter());
        if (painter != nullptr)
            painter->zoomIn();
    });

    QShortcut *zoomOutShortcut = new QShortcut(this);
    zoomOutShortcut->setKey(Qt::Modifier::CTRL + Qt::Key::Key_Minus);
    connect(zoomOutShortcut, &QShortcut::activated, [&]() {
        ImagePainter *painter = dynamic_cast<ImagePainter*>(m_ui->containerWidget->painter());
        if (painter != nullptr)
            painter->zoomOut();
    });
}

void marK::setupConnections()
{
    m_ui->newClassButton->setEnabled(false);
    m_ui->undoButton->setEnabled(false);
    m_ui->deleteButton->setEnabled(false);
    m_ui->resetButton->setEnabled(false);
    m_ui->comboBox->setEnabled(false);
    m_ui->selectClassColorButton->setEnabled(false);
    m_ui->polygonButton->setEnabled(false);
    m_ui->rectButton->setEnabled(false);

    KStandardAction::quit(qApp, SLOT(quit()), actionCollection());

    connect(m_ui->listWidget, &QListWidget::currentItemChanged, this,
            qOverload<QListWidgetItem*>(&marK::changeItem));

    connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, &marK::updateFiles);

    connect(m_ui->newClassButton, &QPushButton::clicked, this, qOverload<>(&marK::addNewClass));

    connect(m_ui->undoButton, &QPushButton::clicked, m_ui->containerWidget, &Container::undo);
    connect(m_ui->deleteButton, &QPushButton::clicked, m_ui->containerWidget, &Container::deleteObject);
    connect(m_ui->resetButton, &QPushButton::clicked, m_ui->containerWidget, &Container::reset);

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
            auto polygonShape = ImagePainter::Shape::Polygon;
            ImagePainter* imgPainter = dynamic_cast<ImagePainter*>(m_ui->containerWidget->painter());
            if (imgPainter != nullptr)
                imgPainter->setShape(polygonShape);
        }
    );

    connect(m_ui->rectButton, &QPushButton::clicked, this,
        [&](bool checked) {
            auto rectangleShape = ImagePainter::Shape::Rectangle;
            ImagePainter* imgPainter = dynamic_cast<ImagePainter*>(m_ui->containerWidget->painter());
            if (imgPainter != nullptr)
                imgPainter->setShape(rectangleShape);
        }
    );

    connect(m_ui->containerWidget, &Container::painterChanged, this,
        [&](Container::PainterType painterType) {
            m_objClasses.clear();
            m_ui->comboBox->clear();
            addNewClass();
            bool isFileLoaded = painterType != Container::PainterType::None;
            m_ui->newClassButton->setEnabled(isFileLoaded);
            m_ui->undoButton->setEnabled(isFileLoaded);
            m_ui->resetButton->setEnabled(isFileLoaded);
            m_ui->comboBox->setEnabled(isFileLoaded);
            m_ui->selectClassColorButton->setEnabled(isFileLoaded);
            m_ui->polygonButton->setEnabled(isFileLoaded);
            m_ui->rectButton->setEnabled(isFileLoaded);
            m_ui->deleteButton->setEnabled(isFileLoaded);
            if (painterType == Container::PainterType::Image || !isFileLoaded)
                m_ui->groupBox_2->setVisible(true);
            else
                m_ui->groupBox_2->setVisible(false);
        }
    );

    connect(m_ui->containerWidget, &Container::savedObjectsChanged, this, &marK::autoSave);
}

void marK::updateFiles()
{
    int index = m_ui->listWidget->currentRow();
    m_ui->listWidget->clear();

    QDir resDirectory(m_currentDirectory);
    QStringList items = resDirectory.entryList(QDir::Files);
    // removing json and xml files that are resulted from autosave
    for (const QString &item : qAsConst(items)) {
        items.removeOne(FileUtils::placeSuffix(item, Serializer::OutputType::JSON));
        items.removeOne(FileUtils::placeSuffix(item, Serializer::OutputType::XML));
    }

    for (const QString &item : qAsConst(items)) {
        if (FileUtils::isTextFile(item) || FileUtils::isImageFile(item)) {
            QPixmap item_pix = (FileUtils::isTextFile(item)) ?
                QIcon::fromTheme("document-edit-sign").pixmap(20, 20) :
                QPixmap(resDirectory.filePath(item));

            item_pix = item_pix.scaledToWidth(20);

            QListWidgetItem *itemW = new QListWidgetItem(item_pix, item);
            itemW->setToolTip(item);

            m_ui->listWidget->addItem(itemW);
        }
    }

    m_ui->listWidget->setGridSize(QSize(
            m_ui->listWidget->sizeHintForColumn(0),
            m_ui->listWidget->sizeHintForRow(0)));

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
    changeItem(currentItem);
}

void marK::changeItem(QListWidgetItem *item)
{
    if (item != nullptr) {
        QString itemPath = QDir(m_currentDirectory).filePath(item->text());

        if (itemPath != m_filepath) {
            makeTempFile();
            m_filepath = itemPath;
            m_ui->containerWidget->changeItem(itemPath);
            retrieveTempFile();
        }
    }
}

void marK::changeDirectory()
{
    QString newDirectoryPath = QFileDialog::getExistingDirectory(this, "Select Directory", QDir::homePath(),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (m_currentDirectory == newDirectoryPath)
        return;

    if (!newDirectoryPath.isEmpty()) {
        if (m_currentDirectory != "")
            m_watcher->removePath(m_currentDirectory);

        m_currentDirectory = newDirectoryPath;
        m_watcher->addPath(m_currentDirectory);
        m_ui->listWidget->clear();
        m_ui->containerWidget->clear();
        m_filepath.clear();
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
    MarkedClass* currentClass = m_objClasses[m_ui->comboBox->currentIndex()];
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

void marK::saveObjects(Serializer::OutputType outputType)
{
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save File"),
                           m_currentDirectory,
                           tr(FileUtils::filterString(outputType)));

    if (filepath.isEmpty())
        return;

    bool success = Serializer::write(filepath, m_ui->containerWidget->savedObjects(), outputType);

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

    if (filepath.isEmpty())
        return;

    QVector<MarkedObject*> objects = Serializer::read(filepath);

    bool success = m_ui->containerWidget->importObjects(objects);

    if (!success) {
        QMessageBox msgBox;
        msgBox.setText("failed to load annotation");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    m_objClasses.clear();
    for (MarkedObject *obj : objects) {
        if (!m_objClasses.contains(obj->objClass()))
            m_objClasses << obj->objClass();
    }
    updateComboBox();
}

void marK::retrieveTempFile()
{
    QString tempFilePath = markTempDirectory().filePath(QString(m_filepath).replace("/", "_"));
    tempFilePath = FileUtils::placeSuffix(tempFilePath, Serializer::OutputType::JSON);

    QVector<MarkedObject*> objects = Serializer::read(tempFilePath);

    bool success = m_ui->containerWidget->importObjects(objects);

    if (!success)
        return;

    m_objClasses.clear();
    for (MarkedObject *obj : objects) {
        if (!m_objClasses.contains(obj->objClass()))
            m_objClasses << obj->objClass();
    }

    updateComboBox();

    m_ui->containerWidget->repaint();
}

void marK::makeTempFile()
{
    QString tempFilePath = markTempDirectory().filePath(QString(m_filepath).replace("/", "_"));

    Serializer::write(tempFilePath, m_ui->containerWidget->savedObjects(), Serializer::OutputType::JSON);
}

void marK::toggleAutoSave()
{
    QAction *button = qobject_cast<QAction*>(sender());
    QString type = button->text();

    if (type == "&Disabled")
        m_autoSaveType = Serializer::OutputType::None;

    else if (type == "&XML")
        m_autoSaveType = Serializer::OutputType::XML;

    else if (type == "&JSON")
        m_autoSaveType = Serializer::OutputType::JSON;
}

void marK::autoSave()
{
    if (m_autoSaveType == Serializer::OutputType::None)
        return;

    auto *watcher = new QFutureWatcher<bool>();
    connect(watcher, &QFutureWatcher<bool>::finished, this, [watcher]() {
        bool success = watcher->result();
        delete watcher;

        if (!success) {
            QMessageBox msgBox;
            msgBox.setText("Failed to auto-save annotation.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    });

    QFuture<bool> future = QtConcurrent::run(
        Serializer::write, m_filepath, m_ui->containerWidget->savedObjects(), m_autoSaveType
    );
    watcher->setFuture(future);
}

marK::~marK()
{
    markTempDirectory().removeRecursively();
}

#include "moc_mark.cpp"
