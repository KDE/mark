#ifndef ANNOTATORDIALOG_H
#define ANNOTATORDIALOG_H

#include "annotatoritem.h"

#include <QDialog>
#include <QGraphicsItem>

namespace Ui {
class AnnotatorDialog;
}

class AnnotatorDialog : public QDialog
{
    Q_OBJECT

public:
    enum OutputType {
        XML,
        JSON,
        Unknown
    };

public:
    explicit AnnotatorDialog(QString file, QString outputPath, OutputType outputType, QWidget *parent = nullptr);

    ~AnnotatorDialog() override;

    bool eventFilter(QObject *watched, QEvent *event) override;

    void accept() override;

    void repaint();

private slots:

    void finishPolygon(bool checked);

    void undo(bool checked);

    void pickColor(bool checked);

    void restartPolygons(bool checked);

    void updateClasses(bool checked = false);

    void addClass(bool checked);

    void changeClass(int index);

private:

    void processItem(AnnotatorItem& polygon, bool fill = false);

    void saveXML();

    void saveJSON();

private:
    Ui::AnnotatorDialog *ui;

    QString m_file;
    QString m_outputPath;

    OutputType m_outputType;

    int m_classQuantity;

    AnnotatorItem m_currentPolygon;
    QVector<AnnotatorItem> m_savedPolygons;

    QVector<QGraphicsItem*> m_items;
    QVector<QColor> m_classColors;
};

#endif // ANNOTATORDIALOG_H
