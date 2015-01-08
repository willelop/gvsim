#include "about.h"

about::about(QWidget *parent) :
    QDialog(parent)
{
    text = new QLabel(tr("GVSim predator simulator. Version 1.1"));
    author = new QLabel(tr("Author: Guillermo Lopez Matos"));
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(text);
    mainLayout->addWidget(author);
    this->setWindowTitle(tr("About GVSim"));
    this->setWindowFlags(Qt::Dialog);
    this->setLayout(mainLayout);
    this->setFixedSize(this->sizeHint());
}
