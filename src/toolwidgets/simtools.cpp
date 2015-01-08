#include "simtools.h"

simTools::simTools(QWidget *parent) :
    QWidget(parent)
{
    simModLayout = new QGridLayout();
    simSpeedGroup = new QGroupBox(tr("Simulation Speed"));
    stallBox = new QGroupBox(tr("Other Options"));
    normalspeed = new QRadioButton(tr("Real Time"));
    speed2x = new QRadioButton(tr("2x Speed"));
    speed4x = new QRadioButton(tr("4x Speed"));
    speed8x = new QRadioButton(tr("8x Speed"));
    speed16x = new QRadioButton(tr("Max Speed"));
    simSpeedLayout = new QVBoxLayout();
    simSpeedLayout->addWidget(normalspeed);
    simSpeedLayout->addWidget(speed2x);
    simSpeedLayout->addWidget(speed4x);
    simSpeedLayout->addWidget(speed8x);
    simSpeedLayout->addWidget(speed16x);
    simSpeedGroup->setLayout(simSpeedLayout);
    normalspeed->setChecked(true);



    simModLayout->addWidget(simSpeedGroup,0,0);
    simAxeLock = new QGroupBox(tr("DOF Lock"));
    simAxeLock->setCheckable(true);
    simAxeLock->setChecked(false);
    lockPitch = new QCheckBox(tr("Pitch"));
    lockRoll = new QCheckBox(tr("Roll"));
    lockYaw = new QCheckBox(tr("Yaw"));
    QHBoxLayout *lockLayout = new QHBoxLayout();
    lockLayout->addWidget(lockPitch);
    lockLayout->addWidget(lockRoll);
    lockLayout->addWidget(lockYaw);
    simAxeLock->setLayout(lockLayout);

    simModLayout->addWidget(simAxeLock,1,0);

    connect(speed2x,SIGNAL(clicked()),this,SLOT(speedChanged()));
    connect(speed4x,SIGNAL(clicked()),this,SLOT(speedChanged()));
    connect(speed8x,SIGNAL(clicked()),this,SLOT(speedChanged()));
    connect(speed16x,SIGNAL(clicked()),this,SLOT(speedChanged()));    connect(normalspeed,SIGNAL(clicked()),this,SLOT(speedChanged()));

    connect(lockPitch,SIGNAL(released()),this,SLOT(dofLockChanged()));
    connect(lockRoll,SIGNAL(released()),this,SLOT(dofLockChanged()));
    connect(lockYaw,SIGNAL(released()),this,SLOT(dofLockChanged()));
    connect(simAxeLock,SIGNAL(toggled(bool)),this,SLOT(dofLockChanged(bool)));

    simModLayout->setRowStretch(100,1000);

    this->setLayout(simModLayout);
}
void simTools::speedChanged()
{
    if(normalspeed->isChecked())  emit updatedSpeed(1);
    else if(speed2x->isChecked()) emit updatedSpeed(2);
    else if(speed4x->isChecked()) emit updatedSpeed(4);
    else if(speed8x->isChecked()) emit updatedSpeed(8);
    else if(speed16x->isChecked()) emit updatedSpeed(170);
}
void simTools::dofLockChanged(bool state)
{
    if(state){
        emit updatedDofLock(this->lockRoll->isChecked(),this->lockPitch->isChecked(),this->lockYaw->isChecked());
    }else{
        this->lockPitch->setChecked(false);
        this->lockRoll->setChecked(false);
        this->lockYaw->setChecked(false);
        emit updatedDofLock(false,false,false);
    }
}
void simTools::setLockLateral(bool lock){
    this->simAxeLock->setChecked(true);
    this->lockRoll->setChecked(lock);
    this->lockYaw->setChecked(lock);
    this->dofLockChanged(true);
}
