#include "keyselector.h"

keySelector::keySelector(QString _name,inputs::CONTROLKEYS _control, QWidget *parent) :
    key(internalkey),control(internalcontrol),
    QWidget(parent)
{
    this->internalcontrol = _control;
    this->keyname = _name;
    this->internalkey =0;
    setButton = new QPushButton(tr("Set"));
    current = new QLabel(tr("Null"));
    current->setAlignment(Qt::AlignCenter);
    current->setStyleSheet("border:1px solid #AAA;margin:0px;padding:1px;background:#DDD;border-radius:2px;");
    name = new QLabel(keyname);
    keyDialog = new keySelectorDialog(this->keyname);
    container = new QHBoxLayout();
    container->addWidget(name,3);
    container->addWidget(current,1);
    container->addWidget(setButton,1);
    this->setLayout(container);
    connect(setButton,SIGNAL(clicked()),keyDialog,SLOT(show()));
    connect(keyDialog,SIGNAL(selectedKey(int)),this,SLOT(keySelected(int)));

}
void keySelector::keySelected(int keynum)
{
    emit keyChanged(internalcontrol,keynum);
    this->internalkey = keynum;
    QKeySequence currentkey(keynum);
    this->current->setText(currentkey.toString());
}
/*!
 * \brief keySelector::updateKey updates the handled key
 * \param _key key function
 *
 * \param _value key value
 */
void keySelector::updateKey(inputs::CONTROLKEYS _key, int _value)
{
    if(_key == this->internalcontrol) {
        QKeySequence currentkey(_value);
        this->internalkey = _value;
        this->current->setText(currentkey.toString());
    }
}
