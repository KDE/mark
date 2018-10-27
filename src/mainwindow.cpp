#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "annotatordialog.h"

#include <QDir>
#include <QFileDialog>
#include <QAbstractButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("QAnnotator");

    setWindowIcon(QIcon(":res/pencil.jpg"));

    connect(ui->chooseFileButton, &QAbstractButton::clicked, this, &MainWindow::annotateFile);
    connect(ui->chooseDirButton, &QAbstractButton::clicked, this, &MainWindow::annotateDir);
    connect(ui->selectOutputButton, &QAbstractButton::clicked, this, &MainWindow::selectOutputPath);

    ui->outputType->addItems({"XML", "JSON"});

    ui->outputPath->setText(QDir::homePath());

    setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::annotateFile(bool checked)
{
    Q_UNUSED(checked);

    AnnotatorDialog::OutputType outputType;

    if (ui->outputType->currentText() == "XML")
        outputType = AnnotatorDialog::OutputType::XML;
    else if (ui->outputType->currentText() == "JSON")
        outputType = AnnotatorDialog::OutputType::JSON;
    else
        outputType = AnnotatorDialog::OutputType::Unknown;

    QString outputPath = ui->outputPath->text();

    QString file = QFileDialog::getOpenFileName(this,
                                                "Open Image",
                                                QDir::homePath(),
                                                "Images (*.png *.xpm *.jpg)");

    if (!file.isEmpty()) {
        AnnotatorDialog annotator(file, outputPath, outputType, this);
        annotator.exec();
    }
}

void MainWindow::annotateDir(bool checked)
{
    Q_UNUSED(checked);

    AnnotatorDialog::OutputType outputType;

    if (ui->outputType->currentText() == "XML")
        outputType = AnnotatorDialog::OutputType::XML;
    else if (ui->outputType->currentText() == "JSON")
        outputType = AnnotatorDialog::OutputType::JSON;
    else
        outputType = AnnotatorDialog::OutputType::Unknown;

    QString outputPath = ui->outputPath->text();

    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if (!path.isEmpty()) {
        QDir dir(path);

        QFileInfoList files = dir.entryInfoList({"*.png", "*.xpm", "*.jpg"});

        for (const QFileInfo& file : files) {
            AnnotatorDialog annotator(file.absoluteFilePath(), outputPath, outputType, this);

            if (annotator.exec() == QDialog::DialogCode::Rejected)
                break;
        }
    }
}

void MainWindow::selectOutputPath(bool checked)
{
    Q_UNUSED(checked);

    QString path = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if (!path.isEmpty())
        ui->outputPath->setText(path);
}
