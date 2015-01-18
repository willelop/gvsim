#include "fueltools.h"

fuelTools::fuelTools(QWidget *parent) :
    QWidget(parent)
{
    number = new QLabel("100%");
    slide = new QSlider(Qt::Horizontal);
    set = new QPushButton("Set");
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(slide,0,0,1,2);
    layout->addWidget(number,1,0);
    layout->addWidget(set,1,1);
    layout->setRowStretch(3,100);
    slide->setRange(0,100);
    slide->setSingleStep(1);
    slide->setValue(100);
    connect(slide,SIGNAL(valueChanged(int)),this,SLOT(updateNumber()));
    connect(set,SIGNAL(clicked()),this,SLOT(setPressed()));
}
void fuelTools::updateNumber()
{
    this->number->setText(QString::number(slide->value())+" %");
}
void fuelTools::setPressed()
{
    emit newFuelCuantity((float)slide->value()/100.0f);
}
