#include "windtools.h"

windTools::windTools(QWidget *parent) :
    QWidget(parent)
{
    sendButton = new QPushButton(tr("Update"));
    isaGroup = new QGroupBox("ISA Conditions");
    mainLayout = new QVBoxLayout(this);
    QHBoxLayout *isaLayout = new QHBoxLayout();
    isaTemp = new QSpinBox();
    isaTempLabel = new QLabel("0");
    isaTemp->setMaximum(350);
    isaTemp->setMinimum(220);
    isaTemp->setValue(288);
    isaTemp->setSingleStep(1);

    isaLayout->addWidget(isaTemp);
    isaLayout->addWidget(isaTempLabel);
    isaGroup->setLayout(isaLayout);
    mainLayout->addWidget(isaGroup);
    mainLayout->addWidget(sendButton);
    mainLayout->addStretch(1000);
    this->setLayout(mainLayout);
    connect(isaTemp,SIGNAL(valueChanged(int)),this,SLOT(isaChanged(int)));
    connect(sendButton,SIGNAL(clicked()),this,SLOT(sendPressed()));

}
void windTools::isaChanged(int)
{
    this->isaTempLabel->setText(QString::number(isaTemp->value())+" C");
}
void windTools::sendPressed()
{
    int relative = isaTemp->value()-288;
    emit changedIsaValue(relative);
}
