#ifndef TEXTCONTAINER_H
#define TEXTCONTAINER_H

#include "annotatorwidget.h"

namespace Ui {
class TextContainer;
}

// TODO: annotatorwidget needs to contain a text component
class TextContainer : public Container, public QTextEdit
{
public:
    explicit TextContainer(QWidget* parent = nullptr);
    ~TextContainer();

private:
    Ui::TextContainer* m_ui;
};

#endif // TEXTCONTAINER_H
