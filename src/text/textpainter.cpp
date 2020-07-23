#include "text/textpainter.h"
#include "text/sentence.h"

#include <QTextEdit>
#include <QFile>
#include <QString>
#include <QTextStream>

TextPainter::TextPainter(Container* parent) :
    Painter(parent),
    m_textEdit(new QTextEdit(parent))
{
    m_parent->scene()->addWidget(m_textEdit);
    m_textEdit->resize(parent->size());

    // FIXME: find another way to get click mouse events to containerWidget
    // as this one doesn't allow use the use of wheel in textEdit
    m_textEdit->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_textEdit->setAttribute(Qt::WA_DeleteOnClose);
    m_textEdit->setReadOnly(true);
    m_textEdit->setVisible(true);
    m_parent->setCurrentObject(new Sentence(m_parent->currentObject()->objClass(), 0, 0));
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

void TextPainter::paint(QPoint point)
{
    auto textCursor = m_textEdit->cursorForPosition(point);
    auto beginSentence = textCursor.anchor() - 1;
    auto endSentence = textCursor.position();
    if (m_lastPos == beginSentence)
        return;

    m_lastPos = beginSentence;
    MarkedObject *sentence = nullptr;

    for (auto *obj : m_parent->savedObjects()) {
        if (obj->objClass() == m_parent->currentObject()->objClass()) {
            if (endSentence == obj->XValueOf()) {
                obj->append(beginSentence, obj->YValueOf());
                sentence = obj;
                break;
            }
            else if (beginSentence == obj->YValueOf()) {
                obj->append(obj->XValueOf(), endSentence);
                sentence = obj;
                break;
            }
            else if (beginSentence == obj->YValueOf() - 2) {
                obj->append(obj->XValueOf(), obj->YValueOf() - 1);
                sentence = obj;
                break;
            }
            else if (endSentence == obj->XValueOf() + 2) {
                obj->append(obj->XValueOf() + 1, obj->YValueOf());
                sentence = obj;
                break;
            }
            else if (endSentence == obj->YValueOf())
                return;
        }

        else if (beginSentence >= obj->XValueOf() && endSentence <= obj->YValueOf())
            return;
    }

    if (!sentence) {
        sentence = new Sentence(m_parent->currentObject()->objClass(), beginSentence, endSentence);
        m_parent->appendObject(sentence);
        paintObject(sentence);
    }
    else
        repaint();
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
