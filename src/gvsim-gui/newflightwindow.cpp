#include "newflightwindow.h"

newFlightWindow::newFlightWindow(QWidget *parent):QDialog(parent)
{
    this->setWindowTitle(tr("New Flight"));
    this->setWindowIcon(QIcon(QPixmap(":/icons/flight.png")));
    this->setWindowFlags(Qt::Dialog);
    
    fuel = new QSlider(Qt::Horizontal);
    fuelq = new QLabel("100%");
    latitude = new QDoubleSpinBox();
    longitude = new QDoubleSpinBox();
    altitude = new QDoubleSpinBox();
    airspeed = new QDoubleSpinBox();
    course = new QDoubleSpinBox();

    latitude->setRange(-90.0,90.0);
    longitude->setRange(-180.0,180.0);
    course->setRange(0.0,360.0);
    latitude->setDecimals(6);
    longitude->setDecimals(6);
    altitude->setRange(0.0,20000);
    airspeed->setRange(0.0,200);

    locations[0] = new QPushButton(tr("Valladolid"));
    locations[1] = new QPushButton(tr("Himalaya"));
    locations[2] = new QPushButton(tr("Teide"));
    locations[3] = new QPushButton(tr("Barajas"));
    locations[4] = new QPushButton(tr("Great Canyon"));
    locations[5] = new QPushButton(tr("Barajas Final"));
    sMapper = new QSignalMapper();



    startOnAirGroup = new QGroupBox(tr("Start on Air"));
    startOnAirGroup->setCheckable(true);
    startOnAirGroup->setChecked(false);

    startflight = new QPushButton(tr("Flight!"));
    closeButton = new QPushButton(tr("Cancel"));


    fuel->setRange(0,100);
    fuel->setValue(100);
    fuel->setSingleStep(1);

    longitude->setRange(-180,+180);
    latitude->setSingleStep(0.05);
    longitude->setSingleStep(0.05);
    altitude->setSingleStep(1.0);
    altitude->setMaximum(40000);

    airspeed->setValue(50.0);
    course->setValue(45);


    altitude->setValue(1000);

    latitude->setMinimumWidth(120);
    longitude->setMinimumWidth(120);
    altitude->setMinimumWidth(120);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(closeButton);
    buttonLayout->addStretch(100);
    buttonLayout->addWidget(startflight);

    QVBoxLayout  *mainLayout = new QVBoxLayout();
    QGroupBox *atmosGroup = new QGroupBox(tr("Atmosphere"));
    QGridLayout *atmosLayout = new QGridLayout();
    atmosGroup->setLayout(atmosLayout);

    QGroupBox *fuelGroup = new QGroupBox(tr("Fuel"));
    QGridLayout *fuelLayout = new QGridLayout();
    fuelLayout->addWidget(fuel,0,0,1,2);
    fuelGroup->setLayout(fuelLayout);

    QGridLayout *startairlayout = new QGridLayout();

    startairlayout->addWidget(new QLabel("Altitude (m)"),0,0);
    startairlayout->addWidget(altitude,0,1);
    startairlayout->addWidget(new QLabel(tr("Airspeed (m/s)")),1,0);
    startairlayout->addWidget(airspeed,1,1);
    startOnAirGroup->setLayout(startairlayout);

    QGroupBox *favoritesGroup = new QGroupBox(tr("Favorites"));
    QHBoxLayout *favoritesLayout = new QHBoxLayout(favoritesGroup);
    for(unsigned i = 0; i < 6; i++)
    {
        sMapper->setMapping(locations[i],i);
        favoritesLayout->addWidget(locations[i]);
        connect(locations[i],SIGNAL(clicked()),sMapper,SLOT(map()));
    }
    QGroupBox *positionGroup = new QGroupBox(tr("Position"));
    QGridLayout *positionLayout = new QGridLayout();
    positionLayout->addWidget(new QLabel("Latitude"),1,0);
    positionLayout->addWidget(latitude,1,1);
    positionLayout->addWidget(new QLabel("Longitude"),2,0);
    positionLayout->addWidget(longitude,2,1);
    positionLayout->addWidget(new QLabel(tr("Course")),3,0);
    positionLayout->addWidget(course,3,1);
    positionLayout->addWidget(startOnAirGroup,4,0,1,2);

    positionGroup->setLayout(positionLayout);

    mainLayout->addWidget(favoritesGroup);
    mainLayout->addWidget(positionGroup);
    mainLayout->addWidget(fuelGroup);
//    mainLayout->addWidget(atmosGroup);
    mainLayout->addLayout(buttonLayout);

    this->setLayout(mainLayout);
    this->setFavorite(0);
    this->setFixedSize(this->sizeHint());
    connect(startflight,SIGNAL(clicked()),this,SLOT(flightButtonPressed()));
    connect(fuel,SIGNAL(valueChanged(int)),this,SLOT(updateFuel(int)));
    connect(sMapper,SIGNAL(mapped(int)),this,SLOT(setFavorite(int)));
    connect(closeButton,SIGNAL(clicked()),this,SLOT(close()));
}
void newFlightWindow::configureFlight()
{
    planeStatusData startState;

    simEngine::resetState(&startState);
    startState.state.position[0] = this->latitude->value()/RAD2DEG_;
    startState.state.position[1] = this->longitude->value()/RAD2DEG_;
    if(startOnAirGroup->isChecked()){
        startState.state.position[2] = this->altitude->value();
        startState.state.speed_air = this->airspeed->value();
        startState.state.startOnAir = true;
    }else{
        startState.state.position[2] = 0.0;
        startState.state.speed_air = 0.0;
        startState.state.startOnAir = false;
    }
    startState.state.fuel_ratio = (float)this->fuel->value()/(float)this->fuel->maximum();
    simUtils::initializeCuaternion(&startState,QVector3D(course->value()/RAD2DEG_,0,0));
    simUtils::updateRotationMatrix(&startState);

    emit logMessage("New Flight Started",LOG::NORMAL);
    emit startFlight(startState);
    this->close();
}
void newFlightWindow::flightButtonPressed()
{
    emit pauseFlight();
    QTimer::singleShot(200,this,SLOT(configureFlight()));
}

void newFlightWindow::updateFuel(int value)
{
    this->fuelq->setText(QString::number(value)+"%");
}
void newFlightWindow::setFavorite(int fav)
{
    double lat, lon,cou, alt,speed;
    speed = 50;
    bool enabled_fly;
    switch(fav)
    {
    case 0:
        lat = 41.697290;
        lon = -4.864407;
        cou = 45;
        enabled_fly = false;
        alt = 0;
        break;
    case 1:
        lat = 28;
        lon = 87;
        cou = 45;
        alt = 6000;
        enabled_fly = true;
        break;
    case 2:
        lat = 28.08;
        lon = -16.75;
        cou = 45;
        alt = 2500;
        enabled_fly = true;
        break;
    case 3:
        lat = 40.5012798;
        lon = -3.5592014;
        cou = 0;
        alt = 0;
        enabled_fly = false;
        break;
    case 4:
        lat = 36.231191;
        lon = -113.030883;
        cou = 45;
        alt = 2000;
        enabled_fly = true;
        break;
    case 5:
        lat = 40.455777;
        lon = -3.518117;
        cou = 322;
        alt = 710;
        speed = 41;
        enabled_fly = true;
        break;
    default:
        lat = 0;
        lon = 0;
        alt = 0;
        enabled_fly = false;
    }
    this->latitude->setValue(lat);
    this->longitude->setValue(lon);
    this->course->setValue(cou);
    this->airspeed->setValue(speed);
    this->startOnAirGroup->setChecked(enabled_fly);
    this->altitude->setValue(alt);
}
