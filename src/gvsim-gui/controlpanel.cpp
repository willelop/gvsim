#include "controlpanel.h"

controlPanel::controlPanel(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowIcon(QIcon(QPixmap(":/icons/cockpit.png")));
    this->setWindowTitle(tr("Cockpit"));
    surf_sign[0] = 1;
    surf_sign[1] = 1;
    surf_sign[2] = -1;
    mainLayout = new QGridLayout(this);
    surfacesGroup = new QGroupBox();
    payloadGroup = new QGroupBox(tr("Payload"));
    gearGroup = new QGroupBox(tr("Landing Gear"));
    surfaces[0] = new QSlider(Qt::Vertical);
    surfaces[1] = new QSlider(Qt::Horizontal);
    surfaces[2] = new QSlider(Qt::Horizontal);
    wheelBreak = new QPushButton(tr("Break"));
    wheelBreak->setCheckable(true);
    enableSurfaces = new QCheckBox(tr("Surfaces"));

    rpmLabel = new QLabel("0 rpm");


    for(int i = 0; i < 3;i++)
    {
        surfaces_values[i] = new QDoubleSpinBox();
        surfaces_trim[i] = new QDoubleSpinBox();
        surfaces_trim[i]->setRange(-1,1);
        surfaces_values[i]->setRange(-1,1);
        surfaces_trim[i]->setSingleStep(0.001);
        surfaces_values[i]->setDecimals(3);
        surfaces_trim[i]->setDecimals(3);
        surfaces_values[i]->setSingleStep(0.001);
        surfaces[i]->setRange(-1000,1000);
    }
    engineGroup = new QGroupBox();
    throttle_enabled = new QCheckBox(tr("Throttle"));
    throttle_enabled->setChecked(false);
    engine[0] = new QSlider(Qt::Vertical);
    engine[1] = new QSlider(Qt::Vertical);
    engine_values[0] = new QDoubleSpinBox();
    engine_values[1] = new QDoubleSpinBox();
    engine_values[0]->setRange(0,1);
    engine_values[1]->setRange(0,1);
    engine_values[0]->setDecimals(3);
    engine_values[1]->setDecimals(3);
    engine_values[0]->setSingleStep(0.001);
    engine_values[1]->setSingleStep(0.001);
    engine[0]->setRange(0,1000);
    engine[1]->setRange(0,1000);
    mix_throttle_check = new QCheckBox(tr("Prop = Throttle"));
    takeoffmode = new QCheckBox(tr("T-O Performance"));
    flapsGroup = new QGroupBox(tr("Flaps"));
    flaps[0] = new QRadioButton(tr("0 deg"));
    flaps[0]->setChecked(true);
    flaps[1] = new QRadioButton(tr("15 deg"));
    flaps[2] = new QRadioButton(tr("30 deg"));
    missile[0] = new QCheckBox(tr("L Missile"));
    missile[1] = new QCheckBox(tr("R Missile"));
    missile[0]->setChecked(true);
    missile[1]->setChecked(true);
    gear = new QCheckBox(tr("Gear Down"));
    gear->setChecked(false);

    aileronLabel = new QLabel(tr("Airlerons"));
    elevatorLabel = new QLabel(tr("Elevator"));
    rudderLabel = new QLabel(tr("Rudder"));
    QGridLayout *surfacesLayout = new QGridLayout();
    surfacesLayout->addWidget(surfaces[0],0,0,3,2,Qt::AlignCenter);
    surfacesLayout->addWidget(elevatorLabel,3,0,1,1,Qt::AlignCenter);
    surfacesLayout->addWidget(surfaces_values[0],3,1,1,1,Qt::AlignCenter);
    surfacesLayout->addWidget(surfaces[1],0,2,1,2);
    surfacesLayout->addWidget(aileronLabel,1,2,1,1,Qt::AlignCenter);
    surfacesLayout->addWidget(surfaces_values[1],1,3,1,1,Qt::AlignCenter);
    surfacesLayout->addWidget(surfaces[2],2,2,1,2);
    surfacesLayout->addWidget(rudderLabel,3,2,1,1,Qt::AlignCenter);
    surfacesLayout->addWidget(surfaces_values[2],3,3,1,1,Qt::AlignCenter);

    surfacesLayout->addWidget(enableSurfaces,0,4,1,2);
    surfacesLayout->addWidget(new QLabel(tr("Trim Ele")),1,4);
    surfacesLayout->addWidget(new QLabel(tr("Trim Ail")),2,4);
    surfacesLayout->addWidget(new QLabel(tr("Trim Rud")),3,4);
    surfacesLayout->addWidget(surfaces_trim[0],1,5);
    surfacesLayout->addWidget(surfaces_trim[1],2,5);
    surfacesLayout->addWidget(surfaces_trim[2],3,5);
    surfacesGroup->setLayout(surfacesLayout);
    surfacesGroup->setMinimumSize(surfacesGroup->sizeHint());

    QGridLayout *engineLayout = new QGridLayout();
    engineLayout->addWidget(engine[0],0,0,2,1);
    engineLayout->addWidget(engine[1],0,1,2,1);
    engineLayout->addWidget(throttle_enabled,0,2);
    engineLayout->addWidget(takeoffmode,1,2);
    engineLayout->addWidget(mix_throttle_check,2,2);
    throttleLabel = new QLabel(tr("Throttle"));
    engineLayout->addWidget(throttleLabel,2,0);
    engineLayout->addWidget(new QLabel(tr("Propeller")),2,1);
    engineLayout->addWidget(engine_values[0],3,0);
    engineLayout->addWidget(engine_values[1],3,1);
    engineLayout->addWidget(rpmLabel,3,2,Qt::AlignRight);
    engineGroup->setLayout(engineLayout);
    engineGroup->setMinimumSize(engineGroup->sizeHint());

    QHBoxLayout *flapsLayout = new QHBoxLayout();
    flapsLayout->addWidget(flaps[0]);
    flapsLayout->addWidget(flaps[1]);
    flapsLayout->addWidget(flaps[2]);
    flapsGroup->setLayout(flapsLayout);
    flapsGroup->setMinimumSize(flapsGroup->sizeHint());
    QHBoxLayout *payloadLayout = new QHBoxLayout();
    payloadLayout->addWidget(missile[0]);
    payloadLayout->addWidget(missile[1]);
    payloadGroup->setLayout(payloadLayout);
    payloadGroup->setMinimumSize(payloadGroup->sizeHint());

    QHBoxLayout *gearLayout = new QHBoxLayout();
    gearLayout->addWidget(gear);
    gearLayout->addWidget(wheelBreak);
    gearGroup->setLayout(gearLayout);

    mainLayout->addWidget(surfacesGroup,0,1,2,2);
    mainLayout->addWidget(payloadGroup,3,1,1,1);
    mainLayout->addWidget(gearGroup,3,2,1,1);

    mainLayout->addWidget(flapsGroup,3,3,1,2);
    mainLayout->addWidget(engineGroup,0,3,2,2);
    mainLayout->setColumnStretch(5,100);
    mainLayout->setColumnStretch(0,100);
    mainLayout->setRowStretch(4,100);
    this->setMinimumSize(this->sizeHint());
    connect(engine[0],SIGNAL(valueChanged(int)),this,SLOT(updatedThrottle(int)));
    for(int i =0; i< 3; i++)
    {
        connect(surfaces[i],SIGNAL(valueChanged(int)),this,SLOT(updateControls()));
        connect(surfaces[i],SIGNAL(valueChanged(int)),this,SLOT(sliderschanged()));
        connect(surfaces_values[i],SIGNAL(valueChanged(double)),this,SLOT(spinboxchanged()));
        connect(surfaces_trim[i],SIGNAL(valueChanged(double)),this,SLOT(spinboxchanged()));

    }
    connect(enableSurfaces,SIGNAL(clicked()),this,SLOT(updateControls()));
    connect(engine[0],SIGNAL(valueChanged(int)),this,SLOT(updateControls()));
    connect(engine[1],SIGNAL(valueChanged(int)),this,SLOT(updateControls()));
    connect(flaps[0],SIGNAL(clicked()),this,SLOT(updateControls()));
    connect(flaps[1],SIGNAL(clicked()),this,SLOT(updateControls()));
    connect(flaps[2],SIGNAL(clicked()),this,SLOT(updateControls()));
    connect(missile[0],SIGNAL(clicked()),this,SLOT(updateControls()));
    connect(missile[1],SIGNAL(clicked()),this,SLOT(updateControls()));
    connect(gear,SIGNAL(clicked()),this,SLOT(updateControls()));
    connect(engine[0],SIGNAL(valueChanged(int)),this,SLOT(sliderschanged()));
    connect(engine[1],SIGNAL(valueChanged(int)),this,SLOT(sliderschanged()));
    connect(engine_values[0],SIGNAL(valueChanged(double)),this,SLOT(spinboxchanged()));
    connect(engine_values[1],SIGNAL(valueChanged(double)),this,SLOT(spinboxchanged()));


    connect(throttle_enabled,SIGNAL(clicked()),this,SLOT(updateControls()));
    connect(takeoffmode,SIGNAL(clicked()),this,SLOT(updateControls()));
    connect(surfacesGroup,SIGNAL(clicked()),this,SLOT(updateControls()));

    connect(wheelBreak,SIGNAL(toggled(bool)),this,SLOT(updateControls()));
    this->updateControls();
}
void controlPanel::updatedThrottle(int _throt)
{
    if(mix_throttle_check->isChecked())    this->engine[1]->setValue(_throt);
}
void controlPanel::updateControls(int)
{
    commandStatusData command;

    command.throttle = (float)engine[0]->value()   / (float)engine[0]->maximum();

    if(mix_throttle_check->isChecked()) command.propeller = command.throttle;
    else command.propeller = (float)engine[1]->value() / (float)engine[1]->maximum();

    command.elevator = surf_sign[0]*(float)surfaces[0]->value() / (float)surfaces[0]->maximum();
    command.ailerons = surf_sign[1]*(float)surfaces[1]->value() / (float)surfaces[1]->maximum();
    command.rudder   = surf_sign[2]*(float)surfaces[2]->value() / (float)surfaces[2]->maximum();
    command.elevatorTrim = (float)surfaces_trim[0]->value() / (float)surfaces_trim[0]->maximum();
    command.aileronTrim = (float)surfaces_trim[1]->value() / (float)surfaces_trim[1]->maximum();
    command.rudderTrim = (float)surfaces_trim[2]->value() / (float)surfaces_trim[2]->maximum();

    if(flaps[1]->isChecked()) command.flaps = 1;
    else if(flaps[2]->isChecked()) command.flaps = 2;
    else command.flaps = 0;
    command.missile[0] = this->missile[0]->isChecked();
    command.missile[1] = this->missile[1]->isChecked();
    command.takeoffmode = this->takeoffmode->isChecked();
    command.gearDown = this->gear->isChecked();
    command.wheelBreak = this->wheelBreak->isChecked();
    for(int i = 0; i< 3; i++)
    {
        this->surfaces[i]->setEnabled(this->enableSurfaces->isChecked());
        this->surfaces_values[i]->setEnabled(this->enableSurfaces->isChecked());
    }
    this->aileronLabel->setEnabled(this->enableSurfaces->isChecked());
    this->elevatorLabel->setEnabled(this->enableSurfaces->isChecked());
    this->rudderLabel->setEnabled(this->enableSurfaces->isChecked());
    this->throttleLabel->setEnabled(this->throttle_enabled->isChecked());
    this->engine[0]->setEnabled(this->throttle_enabled->isChecked());
    this->engine_values[0]->setEnabled(this->throttle_enabled->isChecked());
    emit newControlPacket(command,this->enableSurfaces->isChecked(),this->throttle_enabled->isChecked(),this->mix_throttle_check->isChecked());
}
void controlPanel::sliderschanged()
{
    for(int i = 0; i < 3; i++)
    {
        bool oldstate = surfaces_values[i]->blockSignals(true);
        surfaces_values[i]->setValue(surf_sign[i]*(float)surfaces[i]->value() / (float) surfaces[i]->maximum());
        surfaces_values[i]->blockSignals(oldstate);
    }
    for(int i = 0; i < 2; i++)
    {
        bool oldstate = engine_values[i]->blockSignals(true);
        engine_values[i]->setValue((float)engine[i]->value() / (float) engine[i]->maximum());
        engine_values[i]->blockSignals(oldstate);
    }
}
void controlPanel::spinboxchanged()
{
    for(int i = 0; i < 3; i++)
    {
        bool oldstate = surfaces[i]->blockSignals(true);
        surfaces[i]->setValue((int)(surf_sign[i]*(float)surfaces_values[i]->value() * (float) surfaces[i]->maximum()));
        surfaces[i]->blockSignals(oldstate);
    }
    for(int i = 0; i < 2; i++)
    {
        bool oldstate = engine[i]->blockSignals(true);
        engine[i]->setValue((int)((float)engine_values[i]->value() * (float) engine[i]->maximum()));
        engine[i]->blockSignals(oldstate);
    }
    this->updateControls();
}
void controlPanel::getNewTrimmedCommand(commandStatusData _commanded)
{
    this->throttle_enabled->setChecked(true);
    this->surfacesGroup->setChecked(true);
    this->surfaces_trim[0]->setValue(_commanded.elevatorTrim);
    this->surfaces_trim[1]->setValue(_commanded.aileronTrim);
    this->surfaces_trim[2]->setValue(_commanded.rudderTrim);
    this->engine_values[0]->setValue(_commanded.throttle);
    this->engine_values[1]->setValue(_commanded.propeller);
    this->surfaces[0]->setValue(_commanded.elevator);
    this->surfaces[1]->setValue(_commanded.ailerons);
    this->surfaces[2]->setValue(_commanded.rudder);
    this->gear->setChecked(_commanded.gearDown);
    this->wheelBreak->setChecked(_commanded.wheelBreak);

}
void controlPanel::getStatusFromJoy(commandStatusData _commanded)
{
    float oldPropeller = 0;
    bool oldstate = this->blockSignals(true);
    if(!this->surfacesGroup->isChecked())
    {
        this->surfaces_values[0]->setValue(_commanded.elevator);
        this->surfaces_values[1]->setValue(_commanded.ailerons);
        this->surfaces_values[2]->setValue(_commanded.rudder);
        this->spinboxchanged();
    }
    if(!this->throttle_enabled->isChecked())
    {
        this->engine_values[0]->setValue(_commanded.throttle);
        this->engine[0]->setValue(_commanded.throttle*1000);
        if(this->mix_throttle_check->isChecked()){
            oldPropeller = this->engine_values[1]->value();
            this->engine_values[1]->setValue(_commanded.throttle);
            this->engine[1]->setValue(_commanded.throttle*1000);
        }
    }
    this->wheelBreak->setChecked(_commanded.wheelBreak);
    this->gear->setChecked(_commanded.gearDown);
    this->blockSignals(oldstate);

}
void controlPanel::closeEvent(QCloseEvent *)
{
    emit widgetClose();
}
void controlPanel::showEvent(QShowEvent *)
{
    emit widgetShow();
}
void controlPanel::receivePackage(planeStatusData _status)
{
    this->rpmLabel->setText(QString::number((int)_status.engine.engine_rpm)+" rpm");
}
