#include "keyselectordialog.h"

keySelectorDialog::keySelectorDialog(QString _name,QWidget *parent) :
    QDialog(parent)
{
    this->name = _name;
    this->setWindowTitle(name);
    this->setFixedSize(QSize(150,100));
    this->setFocus();
}
void keySelectorDialog::keyPressEvent(QKeyEvent *e)
{
    emit selectedKey(e->key());
    this->reject();
}
