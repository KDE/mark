#include "text/textpainter.h"
#include "text/sentence.h"

#include <QTextEdit>
#include <QFile>
#include <QScrollBar>
#include <QString>
#include <QTextStream>

TextPainter::TextPainter(Container* parent) :
    Painter(parent),
    m_textEdit(new QTextEdit(parent))
{
    m_textEdit->resize(parent->size());
    m_textEdit->viewport()->installEventFilter(m_parent);
    m_textEdit->viewport()->setMouseTracking(false);
    m_textEdit->setAttribute(Qt::WA_DeleteOnClose);
    m_textEdit->setReadOnly(true);
    m_textEdit->setVisible(true);
    m_parent->setCurrentObject(new Sentence(m_parent->currentObject()->objClass()));
}

TextPainter::~TextPainter()
{
    m_textEdit->close();
}

void TextPainter::changeItem(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
        return;

    QTextStream textStream (&file);

    m_textEdit->setText(textStream.readAll());
}

void TextPainter::paint(QPoint point, bool isDragging)
{
    QTextCursor textCursor = m_textEdit->cursorForPosition(point);

    Sentence *currentSentence = static_cast<Sentence*>(m_parent->currentObject());

    bool toSave = point.isNull() && !isDragging && currentSentence->isValid();
    if (!toSave) {
        if (!isDragging)
            m_anchor = textCursor.position() - 1;
        double end = textCursor.position();

        int idxNearbySentence = -1;
        int idxDoubleClick = -1;
        for (int i = 0; i < m_parent->savedObjects().size(); i++) {
            Sentence *obj = static_cast<Sentence*>(m_parent->savedObjects()[i]);
            if (obj->hasBetween(m_anchor) || obj->hasBetween(end))
                return;
            else if (m_anchor == obj->YValueOf() || end == obj->XValueOf()) {
                idxNearbySentence = i;
                break;
            }
            else if (obj->YValueOf() - obj->XValueOf() == 1 && end == obj->YValueOf()) {
                idxDoubleClick = i;
                break;
            }
        }

        if (idxDoubleClick != -1) {
            toSave = true;
            textCursor.select(QTextCursor::WordUnderCursor);
            m_anchor = textCursor.anchor();
            end = textCursor.position();
            m_parent->savedObjects().remove(idxDoubleClick);
        }

        else if (idxNearbySentence != -1) {
            MarkedObject* obj = m_parent->savedObjects()[idxNearbySentence];
            if (currentSentence->objClass() == obj->objClass()) {
                if (end == obj->XValueOf())
                    toSave = true;
                m_anchor = qMin(m_anchor, obj->XValueOf());
                end = qMax(end, obj->YValueOf());
                m_parent->savedObjects().remove(idxNearbySentence);
            }
        }

        currentSentence->append(std::min(m_anchor, end), std::max(end, m_anchor));
        repaint();
    }
    if (toSave) {
        m_parent->appendObject(currentSentence);
        m_parent->setCurrentObject(new Sentence(currentSentence->objClass()));
    }
}

void TextPainter::repaint()
{
    int scrollPreviousPos = m_textEdit->verticalScrollBar()->value();
    QString plainText = m_textEdit->toPlainText();
    m_textEdit->setPlainText(plainText);
    m_textEdit->verticalScrollBar()->setValue(scrollPreviousPos);

    for (MarkedObject* obj : m_parent->savedObjects())
        paintObject(obj);

    paintObject(m_parent->currentObject());
}

void TextPainter::undo()
{
    if (m_parent->savedObjects().isEmpty())
        return;

    m_parent->savedObjects().pop_back();
    repaint();
}

void TextPainter::deleteCurrentObject()
{
    if (m_parent->savedObjects().isEmpty())
        return;

    m_parent->savedObjects().pop_back();
    repaint();
}

void TextPainter::paintObject(MarkedObject *object)
{
    auto textCursor = m_textEdit->textCursor();
    textCursor.setPosition(object->XValueOf(), QTextCursor::MoveAnchor);
    textCursor.setPosition(object->YValueOf(), QTextCursor::KeepAnchor);

    QTextCharFormat fmt;
    QColor color (object->objClass()->color());
    color.setAlpha(85);
    fmt.setBackground(color);
    textCursor.setCharFormat(fmt);
}

bool TextPainter::importObjects(QVector<MarkedObject*> objects)
{
    m_parent->savedObjects() = objects;
    m_parent->tempObjects() = objects;
    repaint();
    return !objects.isEmpty();
}
