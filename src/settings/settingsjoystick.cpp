#include "settingsjoystick.h"

settingsJoystick::settingsJoystick(QWidget *parent) :
    QDialog(parent)
{
    this->islocked = false;
    settings = new QSettings();
    loaded = false;
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();
    this->inverted[0] = false;
    this->inverted[1] = false;
    this->inverted[2] = false;
    this->inverted[3] = false;
    haveJoystick = false;
    if ( SDL_Init(SDL_INIT_JOYSTICK) == 0 ) {
        int i;
        joystickNum = SDL_NumJoysticks();
        for (i = 0; i < SDL_NumJoysticks(); i++) joystickNames.append(SDL_JoystickName(i));
        haveJoystick = true;
        joyenabled = false;
    } else {
        joystickNum = 0;
        haveJoystick = false;
    }
        this->setWindowTitle(tr("Input Options"));
        this->setWindowIcon(QIcon(QPixmap(":/res/icons/joystick.png")));
        joystick = NULL;
        QVBoxLayout *verLayout = new QVBoxLayout();
        QHBoxLayout *joyLayout = new QHBoxLayout();
        stick1 = new stickDisplay();
        stick2 = new stickDisplay();
        stick1->setFixedSize(QSize(150,150));
        stick2->setFixedSize(QSize(150,150));
        joyLayout->addWidget(stick1);
        joyLayout->addWidget(stick2);
        controlSelectorBox = new QHBoxLayout();
        aileronSelect = new QComboBox();
        elevatorSelect = new QComboBox();
        rudderSelect = new QComboBox();
        throttleSelect = new QComboBox();
        aileronICheck = new QCheckBox();
        elevatorICheck = new QCheckBox();
        rudderICheck = new QCheckBox();
        throttleICheck = new QCheckBox();
        //KEYS
        keysMainLayout = new QHBoxLayout();
        keysLeftLayout = new QVBoxLayout();
        keysRightLayout = new QVBoxLayout();
        elevatorup = new keySelector(tr("Elevator +"),inputs::ELEVATORUP);
        elevatordown = new keySelector(tr("Elevator -"),inputs::ELEVATORDOWN);
        aileronleft = new keySelector(tr("Aileron +"),inputs::AILERONLEFT);
        aileronright = new keySelector(tr("Aileron -"),inputs::AILERONRIGHT);
        rudderleft = new keySelector(tr("Rudder +"),inputs::RUDDERLEFT);
        rudderright = new keySelector(tr("Rudder -"),inputs::RUDDERRIGHT);
        throttleup = new keySelector(tr("Throt +"),inputs::THROTTLEUP);
        throttledown = new keySelector(tr("Throt -"),inputs::THROTTLEDOWN);
        breaks = new keySelector(tr("Break"),inputs::BREAKS);
        geartoogle = new keySelector(tr("LandingGear"),inputs::GEARTOGGLE);
        keysRightLayout->addWidget(elevatorup);
        keysRightLayout->addWidget(elevatordown);
        keysRightLayout->addWidget(aileronleft);
        keysRightLayout->addWidget(aileronright);
        keysRightLayout->addWidget(geartoogle);
        keysLeftLayout->addWidget(rudderleft);
        keysLeftLayout->addWidget(rudderright);
        keysLeftLayout->addWidget(throttleup);
        keysLeftLayout->addWidget(throttledown);
        keysLeftLayout->addWidget(breaks);
        keysLeftLayout->addStretch();
        //right


        keysRightLayout->addStretch();

        //change to qsettings
        controlSelectorBox->addWidget(new QLabel(tr("R")),0);
        controlSelectorBox->addWidget(rudderSelect,3);
        controlSelectorBox->addWidget(rudderICheck);

        controlSelectorBox->addWidget(new QLabel(tr("T")),0);
        controlSelectorBox->addWidget(throttleSelect,3);
        controlSelectorBox->addWidget(throttleICheck);

        controlSelectorBox->addWidget(new QLabel(tr("P")),0);
        controlSelectorBox->addWidget(elevatorSelect,3);
        controlSelectorBox->addWidget(elevatorICheck);


        controlSelectorBox->addWidget(new QLabel(tr("A")),0);
        controlSelectorBox->addWidget(aileronSelect,3);
        controlSelectorBox->addWidget(aileronICheck);

        //qDebug() << "NumJoystick"<<SDL_NumJoysticks();
        QLabel *numJoy = new QLabel("Select Joystick");
        enableJoyBox = new QCheckBox("Enable Joystick");
        enableJoyBox->setChecked(true);
        joystickSelectBox = new QComboBox();
        for( int i=0; i < joystickNames.size(); i++ )
        {
            joystickSelectBox->addItem(joystickNames.at(i)+ QString(" ") + QString::number(i+1));
        }
        connect(enableJoyBox,SIGNAL(toggled(bool)),this,SLOT(toggledJoystick(bool)));
        connect(joystickSelectBox,SIGNAL(currentIndexChanged(int)),this,SLOT(selectJoy(int)));
        //connect(this,SIGNAL(axisValueChanged(int,Sint16)),this,SLOT(printValue(int,Sint16)));
        connect(aileronSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(changeAilerons(int)));
        connect(elevatorSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(changeElevator(int)));
        connect(rudderSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(changeRudder(int)));
        connect(throttleSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(changeThrottle(int)));
        connect(aileronICheck,SIGNAL(clicked(bool)),this,SLOT(invertAilerons(bool)));
        connect(elevatorICheck,SIGNAL(clicked(bool)),this,SLOT(invertElevator(bool)));
        connect(throttleICheck,SIGNAL(clicked(bool)),this,SLOT(invertThrottle(bool)));
        connect(rudderICheck,SIGNAL(clicked(bool)),this,SLOT(invertRudder(bool)));

        //keys signals
        connect(elevatorup,SIGNAL(keyChanged(inputs::CONTROLKEYS,int)),this,SLOT(saveKeyValue(inputs::CONTROLKEYS,int)));
        connect(elevatordown,SIGNAL(keyChanged(inputs::CONTROLKEYS,int)),this,SLOT(saveKeyValue(inputs::CONTROLKEYS,int)));
        connect(aileronleft,SIGNAL(keyChanged(inputs::CONTROLKEYS,int)),this,SLOT(saveKeyValue(inputs::CONTROLKEYS,int)));
        connect(aileronright,SIGNAL(keyChanged(inputs::CONTROLKEYS,int)),this,SLOT(saveKeyValue(inputs::CONTROLKEYS,int)));
        connect(rudderleft,SIGNAL(keyChanged(inputs::CONTROLKEYS,int)),this,SLOT(saveKeyValue(inputs::CONTROLKEYS,int)));
        connect(rudderright,SIGNAL(keyChanged(inputs::CONTROLKEYS,int)),this,SLOT(saveKeyValue(inputs::CONTROLKEYS,int)));
        connect(throttleup,SIGNAL(keyChanged(inputs::CONTROLKEYS,int)),this,SLOT(saveKeyValue(inputs::CONTROLKEYS,int)));
        connect(breaks,SIGNAL(keyChanged(inputs::CONTROLKEYS,int)),this,SLOT(saveKeyValue(inputs::CONTROLKEYS,int)));
        connect(geartoogle,SIGNAL(keyChanged(inputs::CONTROLKEYS,int)),this,SLOT(saveKeyValue(inputs::CONTROLKEYS,int)));
        connect(throttledown,SIGNAL(keyChanged(inputs::CONTROLKEYS,int)),this,SLOT(saveKeyValue(inputs::CONTROLKEYS,int)));
        connect(this,SIGNAL(setInternalKeyValue(inputs::CONTROLKEYS,int)),elevatorup,SLOT(updateKey(inputs::CONTROLKEYS,int)));
        connect(this,SIGNAL(setInternalKeyValue(inputs::CONTROLKEYS,int)),elevatordown,SLOT(updateKey(inputs::CONTROLKEYS,int)));
        connect(this,SIGNAL(setInternalKeyValue(inputs::CONTROLKEYS,int)),aileronleft,SLOT(updateKey(inputs::CONTROLKEYS,int)));
        connect(this,SIGNAL(setInternalKeyValue(inputs::CONTROLKEYS,int)),aileronright,SLOT(updateKey(inputs::CONTROLKEYS,int)));
        connect(this,SIGNAL(setInternalKeyValue(inputs::CONTROLKEYS,int)),rudderleft,SLOT(updateKey(inputs::CONTROLKEYS,int)));
        connect(this,SIGNAL(setInternalKeyValue(inputs::CONTROLKEYS,int)),rudderright,SLOT(updateKey(inputs::CONTROLKEYS,int)));
        connect(this,SIGNAL(setInternalKeyValue(inputs::CONTROLKEYS,int)),throttleup,SLOT(updateKey(inputs::CONTROLKEYS,int)));
        connect(this,SIGNAL(setInternalKeyValue(inputs::CONTROLKEYS,int)),breaks,SLOT(updateKey(inputs::CONTROLKEYS,int)));
        connect(this,SIGNAL(setInternalKeyValue(inputs::CONTROLKEYS,int)),geartoogle,SLOT(updateKey(inputs::CONTROLKEYS,int)));
        connect(this,SIGNAL(setInternalKeyValue(inputs::CONTROLKEYS,int)),throttledown,SLOT(updateKey(inputs::CONTROLKEYS,int)));

        joystickgroup = new QGroupBox(tr("Joystick"));
        QGroupBox *keyboardgroup = new QGroupBox(tr("Keyboard"));
        QVBoxLayout *joyinnerlayout = new QVBoxLayout();
        joyinnerlayout->addWidget(numJoy);
        joyinnerlayout->addWidget(joystickSelectBox);
        joyinnerlayout->addLayout(controlSelectorBox);
        joyinnerlayout->addLayout(joyLayout);
        joystickgroup->setLayout(joyinnerlayout);
        keysMainLayout->addLayout(keysLeftLayout,1);
        keysMainLayout->addLayout(keysRightLayout,1);
        keyboardgroup->setLayout(keysMainLayout);
        verLayout->addWidget(enableJoyBox);
        verLayout->addWidget(joystickgroup);
        verLayout->addWidget(keyboardgroup);
        this->setLayout(verLayout);
        eventTimeout = 100;
        autoRepeatDelay = 500;
        if(haveJoystick && joystickNames.size()>0) {
            open(0);
            joyenabled = true;
        }else{
            joyenabled = false;
            joystickgroup->setEnabled(false);
        }
        if(joystickNames.size() <= 0)
        {
            joystickgroup->setEnabled(false);
            enableJoyBox->setChecked(false);
            enableJoyBox->setEnabled(false);
        }

        connect(joystickgroup,SIGNAL(toggled(bool)),this,SIGNAL(joystickEnabledChange(bool)));


}
settingsJoystick::~settingsJoystick()
{
  if ( isOpen() )
    close();
  SDL_Quit();
}
bool settingsJoystick::open(int stick)
{
    if ( isOpen() )
      close();
    joystick = SDL_JoystickOpen(stick);
    if ( joystick ) {
      this->activeName = joystickNames.at(stick);
      numAxes = SDL_JoystickNumAxes(joystick);
      //qDebug() << numAxes;
      numButtons = SDL_JoystickNumButtons(joystick);
      numHats = SDL_JoystickNumHats(joystick);
      numTrackballs = SDL_JoystickNumBalls(joystick);
      updateSelectors();
      return 1;
    } else {
      fprintf(stderr, "ERROR: couldn't open SDL joystick #%d", stick);
      return 0;
    }
}
void settingsJoystick::close()
{
  if ( joystick )
    SDL_JoystickClose(joystick);
  joystick = NULL;
  numAxes = numButtons = numHats = numTrackballs = 0;
}

void settingsJoystick::changeJoystick(int _joystick)
{
    close();
    open(_joystick);
    this->joystickIndex = _joystick;
    this->activeName = joystickNames[_joystick];
    this->requestJoystickSettings(this->activeName);
    qDebug() << "Chaged";
}
void settingsJoystick::printValue(inputs::CONTROLAXES axis, Sint16 value)
{
    if(axis == inputs::ELEVATORAXE) stick2->update_axe(1,(float)value*0.00003051757);
    else if(axis == inputs::AILERONAXE) stick2->update_axe(0,(float)value*0.00003051757);
    else if(axis == inputs::THROTTLEAXE) stick1->update_axe(1,(float)value*0.00003051757);
    else if(axis == inputs::RUDDERAXE) stick1->update_axe(0,(float)value*0.00003051757);
}

void settingsJoystick::updateSelectors()
{
    this->lock(true);

    aileronSelect->clear();
    rudderSelect->clear();
    throttleSelect->clear();
    elevatorSelect->clear();

    for(int i= 0; i< numAxes;i++)
    {
        aileronSelect->addItem(QString::number(i));
        rudderSelect->addItem(QString::number(i));
        elevatorSelect->addItem(QString::number(i));
        throttleSelect->addItem(QString::number(i));
    }
    this->lock(false);

}
void settingsJoystick::changeElevator(int channel)
{
    channels[0] = channel;
    if(!islocked) this->saveAxeValue(this->activeName,inputs::ELEVATORAXE,channels[0],inverted[0]);
}
void settingsJoystick::changeAilerons(int channel)
{
    channels[1] = channel;
    if(!islocked) this->saveAxeValue(this->activeName,inputs::AILERONAXE,channels[1],inverted[1]);
}
void settingsJoystick::changeThrottle(int channel)
{
    channels[2] = channel;
    if(!islocked) this->saveAxeValue(this->activeName,inputs::THROTTLEAXE,channels[2],inverted[2]);
}
void settingsJoystick::changeRudder(int channel)
{
    channels[3] = channel;
    if(!islocked) this->saveAxeValue(this->activeName,inputs::RUDDERAXE,channels[3],inverted[3]);
}
void settingsJoystick::invertElevator(bool _inverted)
{
    inverted[0] = _inverted;
    if(!islocked) this->saveAxeValue(this->activeName,inputs::ELEVATORAXE,channels[0],inverted[0]);
}
void settingsJoystick::invertAilerons(bool _inverted)
{
    inverted[1] = _inverted;
    if(!islocked) this->saveAxeValue(this->activeName,inputs::AILERONAXE,channels[1],inverted[1]);
}
void settingsJoystick::invertThrottle(bool _inverted)
{
    inverted[2] = _inverted;
    if(!islocked)  this->saveAxeValue(this->activeName,inputs::THROTTLEAXE,channels[2],inverted[2]);
}
void settingsJoystick::invertRudder(bool _inverted)
{
    inverted[3] = _inverted;
    if(!islocked) this->saveAxeValue(this->activeName,inputs::RUDDERAXE,channels[3],inverted[3]);
}
void settingsJoystick::restoreJoystick(int joy, bool enabled){
    if(joy <= joystickNum && haveJoystick)
    {
        this->changeJoystick(joy);
        this->joyenabled = true;
        this->joystickgroup->setEnabled(enabled);
        this->enableJoyBox->setChecked(enabled);
        this->joystickSelectBox->setCurrentIndex(joy);

    }else if(haveJoystick)
    {
        this->changeJoystick(0);
        this->joyenabled = true;
        this->joystickgroup->setEnabled(enabled);
        this->enableJoyBox->setChecked(enabled);
        this->joystickSelectBox->setCurrentIndex(0);
    }else{
        this->changeJoystick(0);
        this->joyenabled = false;
        this->joystickgroup->setEnabled(false);
        this->enableJoyBox->setChecked(false);
        this->joystickSelectBox->setCurrentIndex(0);

    }
    emit this->joystickEnabledChange(this->joyenabled);
}
void settingsJoystick::restoreAxis(inputs::CONTROLAXES axe, int axeval,bool inverted)
{
    bool oldstate = this->blockSignals(true);
    switch(axe)
    {
    case inputs::ELEVATORAXE:
        this->elevatorSelect->setCurrentIndex(axeval);
        this->elevatorICheck->setChecked(inverted);
        break;
    case inputs::AILERONAXE:
        this->aileronSelect->setCurrentIndex(axeval);
        this->aileronICheck->setChecked(inverted);
        break;
    case inputs::RUDDERAXE:
        this->rudderSelect->setCurrentIndex(axeval);
        this->rudderICheck->setChecked(inverted);
        break;
    case inputs::THROTTLEAXE:
        this->throttleSelect->setCurrentIndex(axeval);
        this->throttleICheck->setChecked(inverted);
        break;
    }
    this->blockSignals(oldstate);
}
void settingsJoystick::toggledJoystick(bool _value)
{
    this->joystickgroup->setEnabled(_value);
    this->joyenabled = _value;
    if(_value){
        this->saveJoystick(joystickNames[this->joystickSelectBox->currentIndex()],this->joyenabled);
    }
}
void settingsJoystick::restored(){
    this->loaded = true;
}
void settingsJoystick::receivedChangedSticks(Sint16 one, Sint16 two, Sint16 three, Sint16 four)
{
    printValue(inputs::ELEVATORAXE, one);
    printValue(inputs::AILERONAXE, two);
    printValue(inputs::THROTTLEAXE, three);
    printValue(inputs::RUDDERAXE, four);
}

void settingsJoystick::selectJoy(int _index)
{
    bool status = this->blockSignals(true);
    this->open(_index);
    this->blockSignals(status);
    this->activeName = this->joystickNames.at(_index);
    emit requestOpenJoy(_index);
    emit requestJoyInfo(this->joystickNames.at(_index));
}

void settingsJoystick::saveAxeValue(QString _joy,inputs::CONTROLAXES axe, int _axeint,bool inverted)
{
    qDebug() << _joy;
    settings->setValue("Input/Joystick/"+_joy+"/Axes/Select/"+QString::number(axe),_axeint);
    settings->setValue("Input/Joystick/"+_joy+"/Axes/Inverted/"+QString::number(axe),inverted);
    emit message2log(tr("Setting Axe ") + _joy + " to " + QString::number(_axeint) + QString::number(inverted),LOG::NORMAL);
    qDebug() << "Setting axe" << settings->value("Input/Joystick/"+_joy+"/Axes/Select/"+QString::number(axe));
    emit newAxeValue(axe,_axeint,inverted);
}

void settingsJoystick::saveJoystick(QString _name,bool _enabled)
{
    settings->setValue("Input/Joystick/Selected",_name);
    settings->setValue("Input/Joystick/"+_name+"/Enabled",_enabled);
    settings->setValue("Input/Joystick/"+_name+"/Name",_name);
    emit message2log(tr("Setting Joystick ") +_name + " to " + QString::number(_enabled),LOG::NORMAL);
    emit requestOpenJoy(this->joystickSelectBox->currentIndex());
}

void settingsJoystick::saveKeyValue(inputs::CONTROLKEYS _key, int _keyint)
{
    settings->setValue("Input/Keys/Select/"+QString::number(_key),_keyint);
    emit message2log(tr("Setting Key ") + QString::number(_key) + " to " + QString::number(_keyint),LOG::NORMAL);
    emit newKeyValue(_key,_keyint);
    qDebug() << "Saving Key";
}
void settingsJoystick::requestJoystickSettings(QString _name)
{
    qDebug() << "Info about" << _name << "Requested";
    bool exists = settings->value("Input/Joystick/"+_name+"/Enabled").isValid();
    settings->setValue("Input/Joystick/Selected",_name);
    if(exists)
    {
        this->lock(true);
        this->aileronSelect->setCurrentIndex(settings->value("Input/Joystick/"+_name+"/Axes/Select/"+QString::number(inputs::AILERONAXE)).toInt());
        this->elevatorSelect->setCurrentIndex(settings->value("Input/Joystick/"+_name+"/Axes/Select/"+QString::number(inputs::ELEVATORAXE)).toInt());
        this->throttleSelect->setCurrentIndex(settings->value("Input/Joystick/"+_name+"/Axes/Select/"+QString::number(inputs::THROTTLEAXE)).toInt());
        this->rudderSelect->setCurrentIndex(settings->value("Input/Joystick/"+_name+"/Axes/Select/"+QString::number(inputs::RUDDERAXE)).toInt());
        bool inverted0 = settings->value("Input/Joystick/"+_name+"/Axes/Inverted/"+QString::number(inputs::ELEVATORAXE)).toBool();
        bool inverted1 = settings->value("Input/Joystick/"+_name+"/Axes/Inverted/"+QString::number(inputs::AILERONAXE)).toBool();
        bool inverted2 = settings->value("Input/Joystick/"+_name+"/Axes/Inverted/"+QString::number(inputs::RUDDERAXE)).toBool();
        bool inverted3 = settings->value("Input/Joystick/"+_name+"/Axes/Inverted/"+QString::number(inputs::THROTTLEAXE)).toBool();
        this->elevatorICheck->setChecked(inverted0);
        this->aileronICheck->setChecked(inverted1);
        this->rudderICheck->setChecked(inverted2);
        this->throttleICheck->setChecked(inverted3);
        this->lock(false);
    }else{
        qDebug() << "Generating new Joy Info";
        settings->setValue("Input/Joystick/"+_name+"/Enabled",true);
        settings->setValue("Input/Joystick/"+_name+"/Name",_name);
        settings->setValue("Input/Joystick/"+_name+"/Axes/Select/"+QString::number(inputs::ELEVATORAXE),0);
        settings->setValue("Input/Joystick/"+_name+"/Axes/Select/"+QString::number(inputs::AILERONAXE),2);
        settings->setValue("Input/Joystick/"+_name+"/Axes/Select/"+QString::number(inputs::RUDDERAXE),3);
        settings->setValue("Input/Joystick/"+_name+"/Axes/Select/"+QString::number(inputs::THROTTLEAXE),4);
        settings->setValue("Input/Joystick/"+_name+"/Axes/Inverted/"+QString::number(inputs::ELEVATORAXE),false);
        settings->setValue("Input/Joystick/"+_name+"/Axes/Inverted/"+QString::number(inputs::AILERONAXE),false);
        settings->setValue("Input/Joystick/"+_name+"/Axes/Inverted/"+QString::number(inputs::RUDDERAXE),false);
        settings->setValue("Input/Joystick/"+_name+"/Axes/Inverted/"+QString::number(inputs::THROTTLEAXE),false);

        this->lock(true);
        qDebug() << "Exists" << settings->value("Input/Joystick/"+_name+"/Axes/Select/"+QString::number(inputs::AILERONAXE));
        this->aileronSelect->setCurrentIndex(settings->value("Input/Joystick/"+_name+"/Axes/Select/"+QString::number(inputs::AILERONAXE)).toInt());
        this->elevatorSelect->setCurrentIndex(settings->value("Input/Joystick/"+_name+"/Axes/Select/"+QString::number(inputs::ELEVATORAXE)).toInt());
        this->throttleSelect->setCurrentIndex(settings->value("Input/Joystick/"+_name+"/Axes/Select/"+QString::number(inputs::THROTTLEAXE)).toInt());
        this->rudderSelect->setCurrentIndex(settings->value("Input/Joystick/"+_name+"/Axes/Select/"+QString::number(inputs::RUDDERAXE)).toInt());
        bool inverted0 = settings->value("Input/Joystick/"+_name+"/Axes/Inverted/"+QString::number(inputs::ELEVATORAXE)).toBool();
        bool inverted1 = settings->value("Input/Joystick/"+_name+"/Axes/Inverted/"+QString::number(inputs::AILERONAXE)).toBool();
        bool inverted2 = settings->value("Input/Joystick/"+_name+"/Axes/Inverted/"+QString::number(inputs::RUDDERAXE)).toBool();
        bool inverted3 = settings->value("Input/Joystick/"+_name+"/Axes/Inverted/"+QString::number(inputs::THROTTLEAXE)).toBool();
        this->elevatorICheck->setChecked(inverted0);
        this->aileronICheck->setChecked(inverted1);
        this->rudderICheck->setChecked(inverted2);
        this->throttleICheck->setChecked(inverted3);
        this->lock(false);
    }
    emit newAxeValue(inputs::AILERONAXE,aileronSelect->currentIndex(),aileronICheck->isChecked());
    emit newAxeValue(inputs::ELEVATORAXE,elevatorSelect->currentIndex(),elevatorICheck->isChecked());
    emit newAxeValue(inputs::THROTTLEAXE,throttleSelect->currentIndex(),throttleICheck->isChecked());
    emit newAxeValue(inputs::RUDDERAXE,rudderSelect->currentIndex(),rudderICheck->isChecked());
}
void settingsJoystick::restoreKeysValues()
{
    qDebug() << "Restoring Keys";
    for ( int ctrloop = inputs::ELEVATORUP; ctrloop <= inputs::GEARTOGGLE; ctrloop++ )
    {
        inputs::CONTROLKEYS current = static_cast<inputs::CONTROLKEYS>(ctrloop);
        emit setInternalKeyValue(current,settings->value("Input/Keys/Select/"+QString::number(current)).toInt());
        emit newKeyValue(current,settings->value("Input/Keys/Select/"+QString::number(current)).toInt());
    }
}
void settingsJoystick::lock(bool enable)
{
    this->islocked = enable;
    this->blockSignals(enable);
}
void settingsJoystick::restoreAll()
{
    if(haveJoystick && joystickNames.size() > 0)
    {
        this->requestJoystickSettings(joystickNames.at(0));
    }
    this->restoreKeysValues();
}
void settingsJoystick::restoreSettings()
{
    emit joystickEnabledChange(this->joyenabled);
    qDebug() << this->joyenabled;
}
