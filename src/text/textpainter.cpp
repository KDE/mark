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
    auto textCursor = m_textEdit->cursorForPosition(point);

    Sentence *currentSentence = static_cast<Sentence*>(m_parent->currentObject());

    bool toSave = point.isNull() && !isDragging && currentSentence->isValid();
    if (!toSave) {
        if (!isDragging)
            m_anchor = textCursor.position() - 1;
        double end = textCursor.position();

        bool toPaint = true;
        int idxNearbySentence = -1;
        for (int i = 0; i < m_parent->savedObjects().size(); i++) {
            Sentence *obj = static_cast<Sentence*>(m_parent->savedObjects()[i]);
            if (obj->hasBetween(m_anchor) || obj->hasBetween(end)) {
                toPaint = false;
                break;
            }
            else if (m_anchor == obj->YValueOf() || end == obj->XValueOf()) {
                idxNearbySentence = i;
                break;
            }
        }

        if (idxNearbySentence != -1) {
            MarkedObject* obj = m_parent->savedObjects()[idxNearbySentence];
            if (currentSentence->objClass() == obj->objClass()) {
                if (end == obj->XValueOf())
                    toSave = true;
                m_anchor = qMin(m_anchor, obj->XValueOf());
                end = qMax(end, obj->YValueOf());
                m_parent->savedObjects().remove(idxNearbySentence);
            }
        }

        if (toPaint) {
            currentSentence->append(std::min(m_anchor, end), std::max(end, m_anchor));
            repaint();
        }
    }
    if (toSave) {
        m_parent->appendObject(currentSentence);
        m_parent->setCurrentObject(new Sentence(currentSentence->objClass()));
    }
}

void TextPainter::repaint()
{
    QString plainText = m_textEdit->toPlainText();
    m_textEdit->setPlainText(plainText);

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
