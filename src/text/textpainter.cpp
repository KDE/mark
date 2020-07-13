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
    textCursor.select(QTextCursor::WordUnderCursor);
    auto beginSentence = textCursor.anchor();
    auto endSentence = textCursor.position();

    for (MarkedObject *object : m_parent->savedObjects()) {
        auto *st = static_cast<Sentence*>(object);
        if (st->objClass() == m_parent->currentObject()->objClass()) {
            // FIXME: after changing the end/beginSentence, object should not be in the
            // savedObjects anymore, mantaining it by now because of undo functionality
            // that needs it to work properly
            if ((st->XValueOf() - 1) == endSentence) {
                endSentence = st->YValueOf();
            }
            else if ((st->YValueOf() + 1) == beginSentence) {
                beginSentence = st->XValueOf();
            }
        }
    }

    auto sentence = new Sentence(m_parent->currentObject()->objClass(), beginSentence, endSentence);
    m_parent->savedObjects() << sentence;
    paintObject(sentence);
}

void TextPainter::repaint()
{
    for (int i = 0; i < undoTimes; i++)
        m_textEdit->undo();

    undoTimes = 0;

    for (MarkedObject* obj : m_parent->savedObjects())
        paintObject(obj);
}

void TextPainter::paintObject(MarkedObject *object)
{
    auto sentence = static_cast<Sentence*>(object);
    auto textCursor = m_textEdit->textCursor();
    textCursor.setPosition(sentence->XValueOf(0), QTextCursor::MoveAnchor);
    textCursor.setPosition(sentence->YValueOf(0), QTextCursor::KeepAnchor);

    QTextCharFormat fmt;
    QColor color (sentence->objClass()->color());
    color.setAlpha(85);
    fmt.setBackground(color);
    textCursor.setCharFormat(fmt);
    undoTimes++;
}

bool TextPainter::importObjects(QVector<MarkedObject*> objects)
{
    m_parent->savedObjects() = objects;
    repaint();
    return !objects.isEmpty();
}
