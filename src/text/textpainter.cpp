#include "text/textpainter.h"
#include "text/sentence.h"

#include <QTextEdit>
#include <QFile>
#include <QString>
#include <QTextStream>

TextPainter::TextPainter(Container* parent) :
    Painter(parent),
    m_textEdit(new QTextEdit)
{
    // temporary, with this geometry textEdit is in the right position
    m_textEdit->setGeometry(-13, -25, 877, 690);
    m_parent->scene()->addWidget(m_textEdit);
    m_textEdit->setReadOnly(true);
    m_parent->setCurrentObject(new Sentence(m_parent->currentObject()->objClass(), 0, 0));
}

TextPainter::~TextPainter()
{
    delete m_textEdit;
}

void TextPainter::changeItem(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
        return;

    QTextStream textStream (&file);

    m_textEdit->setText(textStream.readAll());
}

void TextPainter::paint(QPoint point)
{
    auto textCursor = m_textEdit->cursorForPosition(point);
    if (textCursor == m_lastTextCursor)
        return;

    m_lastTextCursor = textCursor;
    auto beginSentence = textCursor.anchor() - 1;
    auto endSentence = textCursor.position();

    for (auto *obj : m_parent->savedObjects()) {
        if (obj->objClass() == m_parent->currentObject()->objClass()) {
            if (endSentence == obj->XValueOf()) {
                obj->append(beginSentence, obj->YValueOf());
                repaint();
                return;
            }
            else if (beginSentence == obj->YValueOf()) {
                obj->append(obj->XValueOf(), endSentence);
                repaint();
                return;
            }
            else if (beginSentence == obj->YValueOf() - 2) {
                obj->append(obj->XValueOf(), obj->YValueOf() - 1);
                repaint();
                return;
            }
            else if (beginSentence == obj->XValueOf()) {
                obj->append(obj->XValueOf() + 1, obj->YValueOf());
                repaint();
                return;
            }
            else if (endSentence == obj->YValueOf())
                return;
        }
        else if (beginSentence >= obj->XValueOf() && endSentence <= obj->YValueOf())
            return;
    }

    auto sentence = new Sentence(m_parent->currentObject()->objClass(), beginSentence, endSentence);
    m_parent->appendObject(sentence);
    paintObject(sentence);
}

void TextPainter::repaint()
{
    QString plainText = m_textEdit->toPlainText();
    m_textEdit->setPlainText(plainText);

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
    auto sentence = static_cast<Sentence*>(object);
    auto textCursor = m_textEdit->textCursor();
    textCursor.setPosition(sentence->XValueOf(), QTextCursor::MoveAnchor);
    textCursor.setPosition(sentence->YValueOf(), QTextCursor::KeepAnchor);

    QTextCharFormat fmt;
    QColor color (sentence->objClass()->color());
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
