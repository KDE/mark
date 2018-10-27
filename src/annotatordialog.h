#ifndef ANNOTATORDIALOG_H
#define ANNOTATORDIALOG_H

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

    void savePolygon(bool checked);

    void undo(bool checked);

private:

    void processPolygon(QPolygonF& polygon, Qt::GlobalColor color, bool fill = false, bool focusFirst = false);

    void saveXML();

    void saveJSON();

private:
    Ui::AnnotatorDialog *ui;

    QString m_file;
    QString m_outputPath;

    OutputType m_outputType;

    QPolygonF m_currentPolygon;
    QVector<QPolygonF> m_savedPolygons;

    QVector<QGraphicsItem*> m_items;
};

#endif // ANNOTATORDIALOG_H
