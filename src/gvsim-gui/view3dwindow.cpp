#include "view3dwindow.h"

view3dWindow::view3dWindow(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *vlayout = new QVBoxLayout();
    view3d = new view3dWidget(this);
    this->setContentsMargins(0,0,0,0);
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    view3d->setFixedSize(250,250);
    vlayout->addWidget(view3d);
    vlayout->setMargin(0);
    this->setLayout(vlayout);
    this->setFixedSize(this->sizeHint());
}
