#include "textcontainer.h"
#include "ui_textcontainer.h"

TextContainer::TextContainer(QWidget* parent) :
    Container(parent),
    m_ui(new Ui::TextContainer)
{
    m_ui->setupUi(this);
}

TextContainer::~TextContainer()
{
}
