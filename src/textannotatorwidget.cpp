#include "textannotatorwidget.h"
#include "ui_textannotatorwidget.h"

TextAnnotatorWidget::TextAnnotatorWidget(QWidget* parent) :
    AnnotatorWidget(parent),
    m_ui(new Ui::TextAnnotatorWidget)
{
    m_ui->setupUi(this);
}

TextAnnotatorWidget::~TextAnnotatorWidget()
{
}
