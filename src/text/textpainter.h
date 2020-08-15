#ifndef TEXTPAINTER_H
#define TEXTPAINTER_H

#include "ui/painter.h"
#include "ui/container.h"

#include <QVector>

class QTextEdit;

class TextPainter : public Painter
{
public:
    explicit TextPainter(Container* parent);
    ~TextPainter();

    void paint(QPoint point, bool isDragging) override;
    void paintObject(MarkedObject* object) override;
    void repaint() override;
    void undo() override;
    void deleteCurrentObject() override;
    void changeItem(const QString& path) override;

    bool importObjects(QVector<MarkedObject*> objects) override;

private:
    QTextEdit *m_textEdit;
    double m_anchor;
};

#endif // TEXTPAINTER_H
