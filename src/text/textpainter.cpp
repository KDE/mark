#include "text/sentence.h"
#include "text/textpainter.h"

#include <QFile>
#include <QScrollBar>
#include <QString>
#include <QTextEdit>
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

void TextPainter::changeItem(const QString& filepath)
{
    QFile textFile(filepath);
    if (!textFile.open(QIODevice::ReadOnly|QIODevice::Text))
        return;

    QTextStream textStream (&textFile);

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
            else if (m_anchor == obj->end() || end == obj->begin()) {
                idxNearbySentence = i;
                break;
            }
            else if (obj->end() - obj->begin() == 1 && end == obj->end()) {
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
            Sentence* obj = static_cast<Sentence*>(m_parent->savedObjects()[idxNearbySentence]);
            if (currentSentence->objClass() == obj->objClass()) {
                if (end == obj->begin())
                    toSave = true;
                m_anchor = qMin(m_anchor, obj->begin());
                end = qMax(end, obj->end());
                m_parent->savedObjects().remove(idxNearbySentence);
            }
        }

        currentSentence->move(std::min(m_anchor, end), std::max(end, m_anchor));

        if (currentSentence->isValid()) {
            if (isDragging) {
                int scrollBarPreviousPos = m_textEdit->verticalScrollBar()->value();
                m_textEdit->undo();
                m_textEdit->verticalScrollBar()->setValue(scrollBarPreviousPos);
            }

            paintObject(currentSentence);
        }
        else
            currentSentence->move(m_anchor, end + 1);
    }
    if (toSave) {
        m_parent->appendObject(currentSentence);
        repaint();
        m_parent->setCurrentObject(new Sentence(currentSentence->objClass()));
    }
}

void TextPainter::repaint()
{
    QTextCharFormat fmt;
    fmt.setBackground(QBrush(QColor(), Qt::NoBrush));
    m_textEdit->setCurrentCharFormat(fmt);

    int scrollPreviousPos = m_textEdit->verticalScrollBar()->value();
    QString plainText = m_textEdit->toPlainText();
    m_textEdit->setPlainText(plainText);
    m_textEdit->verticalScrollBar()->setValue(scrollPreviousPos);

    for (MarkedObject* obj : m_parent->savedObjects())
        paintObject(obj);
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
    Sentence *sentence = static_cast<Sentence*>(object);

    QTextCursor textCursor = m_textEdit->textCursor();
    textCursor.setPosition(sentence->begin(), QTextCursor::MoveAnchor);
    textCursor.setPosition(sentence->end(), QTextCursor::KeepAnchor);

    QTextCharFormat fmt;
    QColor color (object->objClass()->color());
    color.setAlpha(85);
    fmt.setBackground(color);
    textCursor.setCharFormat(fmt);
}

bool TextPainter::importObjects(QVector<MarkedObject*> objects)
{
    if (objects.isEmpty() || objects.first()->type() != MarkedObject::Type::Sentence)
        return false;

    m_parent->savedObjects() = objects;
    repaint();
    return true;
}
