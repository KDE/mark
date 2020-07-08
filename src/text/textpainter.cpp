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
    m_textEdit->resize(850, 640);
    m_parent->scene()->addWidget(m_textEdit);
    m_textEdit->setReadOnly(true);
}

TextPainter::~TextPainter()
{
    delete m_textEdit;
}

#include <QDebug>
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
    auto textCursor = m_textEdit->textCursor();
    auto beginSentence = textCursor.anchor();
    auto endSentence = textCursor.position();

    QTextCharFormat fmt;
    QColor color (m_parent->currentObject()->objClass()->color());
    color.setAlpha(85);
    fmt.setBackground(color);
    textCursor.setCharFormat(fmt);

    auto sentence = new Sentence(m_parent->currentObject()->objClass(), beginSentence, endSentence);
    m_parent->setCurrentObject(sentence);
    m_parent->savedObjects() << sentence;
    undoTimes++;
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
