#ifndef TEXTANNOTATORWIDGET_H
#define TEXTANNOTATORWIDGET_H

#include "annotatorwidget.h"

namespace Ui {
class TextAnnotatorWidget;
}

// TODO: annotatorwidget needs to contain a text component
class TextAnnotatorWidget : public AnnotatorWidget
{
public:
    explicit TextAnnotatorWidget(QWidget* parent = nullptr);
    ~TextAnnotatorWidget();

private:
    Ui::TextAnnotatorWidget* m_ui;
};

#endif // TEXTANNOTATORWIDGET_H