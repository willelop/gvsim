#include "autopilottools.h"

autopilotTools::autopilotTools(QWidget *parent) :
    QWidget(parent)
{
    QLabel *explanation = new QLabel(tr("This altitude control affects only to pitch and speed. To prevent lateral deviations, when activated, this module will lock Roll and Yaw Degrees of Freedom(<i>Simulation-> DOF Lock</i>)"));
    explanation->setWordWrap(true);
    lastAlt = 0.0;
    lastVel = 0.0;
    setCurrent_alt = new QPushButton(tr("Set Curr. Alt"));
    setCurrent_spd = new QPushButton(tr("Set Curr. Spd"));

    speed = new QSpinBox();
    altitude = new QSpinBox();
    speed->setRange(0,300);
    altitude->setRange(0,11000);
    speedLabel = new QLabel(tr("Speed m/s"));
    altitudeLabel = new QLabel(tr("Altitude m"));
    enabled_vel = new QCheckBox(tr("Enable"));
    enabled_alt = new QCheckBox(tr("Enable"));

    QGridLayout *layout = new QGridLayout(this);
    QHBoxLayout *setLayout = new QHBoxLayout;
    setLayout->addWidget(setCurrent_alt);
    setLayout->addWidget(setCurrent_spd);

    layout->addWidget(explanation,0,0,1,3);
    layout->addLayout(setLayout,1,0,1,3);
    layout->addWidget(altitudeLabel,2,0);
    layout->addWidget(altitude,2,1);
    layout->addWidget(enabled_alt,2,2);
    layout->addWidget(speedLabel,3,0);
    layout->addWidget(speed,3,1);
    layout->addWidget(enabled_vel,3,2);
    layout->setRowStretch(100,100);
    connect(enabled_alt,SIGNAL(toggled(bool)),this,SLOT(changeState(bool)));
    connect(enabled_vel,SIGNAL(toggled(bool)),this,SLOT(changeState(bool)));
    connect(altitude,SIGNAL(valueChanged(int)),this,SLOT(changeState(int)));
    connect(speed,SIGNAL(valueChanged(int)),this,SLOT(changeState(int)));
    connect(setCurrent_alt,SIGNAL(clicked()),this,SLOT(setCurrentAlt()));
    connect(setCurrent_spd,SIGNAL(clicked()),this,SLOT(setCurrentVel()));


}
void autopilotTools::changeState(bool)
{
    emit changedAPSettings((float)altitude->value(),(float)speed->value(),enabled_alt->isChecked(),enabled_vel->isChecked());
    emit lockLateralModes(enabled_alt->isChecked());
}
void autopilotTools::changeState(int)
{
    emit changedAPSettings((float)altitude->value(),(float)speed->value(),enabled_alt->isChecked(),enabled_vel->isChecked());
    emit lockLateralModes(enabled_alt->isChecked());
}
void autopilotTools::updateLastValues(planeStatusData _state)
{
    this->lastAlt = _state.state.position[2];
    this->lastVel = _state.state.speed_air;
}
void autopilotTools::setCurrentAlt()
{
    this->altitude->setValue(std::ceil(this->lastAlt));
}
void autopilotTools::setCurrentVel()
{
    this->speed->setValue(std::ceil(this->lastVel));
}
