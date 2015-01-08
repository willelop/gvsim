#include "numericaldata.h"

numericalData::numericalData(QWidget *parent) :
    QWidget(parent)
{
    veldec = 3;
    velwidth = 9;
    raddec = 4;
    radwidth = 2;
    degdec = 4;
    degwidth = 2;
    forceswidth = 6;
    forcesdec = 0;
    scroll01 = new QScrollArea();
    QHBoxLayout *mainGrid = new QHBoxLayout();
    QVBoxLayout *column1 = new QVBoxLayout();
    QVBoxLayout *column2 = new QVBoxLayout();

    QGridLayout *gridGlobal = new QGridLayout();
    QGridLayout *gridSpeed = new QGridLayout();
    QGridLayout *gridAngles = new QGridLayout();
    QGridLayout *gridAtmosp = new QGridLayout();
    QGridLayout *gridOther = new QGridLayout();
    QGridLayout *gridCommand = new QGridLayout();
    QGridLayout *gridForces = new QGridLayout();
    QGridLayout *gridSimula = new QGridLayout();
    QGridLayout *gridAerodyn = new QGridLayout();
    QGridLayout *gridPayload = new QGridLayout();
    QGridLayout *gridEngine = new QGridLayout();

    QGridLayout *gridMass = new QGridLayout();

    //QString titleCss("QLabel {qproperty-alignment: AlignCenter;font-size: 14px; padding: 2px;font-weight:bold; border-bottom: 2px solid #000033; background-color: #F5F5F5;}");
    QString titleCss("QLabel {qproperty-alignment: AlignCenter;font-size: 14px; padding: 2px;font-weight:bold; border-bottom: 2px solid #00CC33; color:#00CC33 }");

    position[0] = new QLabel();
    position[1] = new QLabel();
    position[2] = new QLabel();
    lat_deg = new QLabel();
    lon_deg = new QLabel();
    ground_pos = new QLabel();
    velocity_e[0] = new QLabel();
    velocity_e[1] = new QLabel();
    velocity_e[2] = new QLabel();
    velocity_b[0] = new QLabel();
    velocity_b[1] = new QLabel();
    velocity_b[2] = new QLabel();
    speeds[0] = new QLabel();
    speeds[1] = new QLabel();
    alpha_beta[0] = new QLabel();
    alpha_beta[1] = new QLabel();
    alpha_beta_deg[0] = new QLabel();
    alpha_beta_deg[1] = new QLabel();
    eulerangles[0] = new QLabel();
    eulerangles[1] = new QLabel();
    eulerangles[2] = new QLabel();
    eulerangles_deg[0] = new QLabel();
    eulerangles_deg[1] = new QLabel();
    eulerangles_deg[2] = new QLabel();

    atm_pres = new QLabel();
    atm_dens = new QLabel();
    atm_temp = new QLabel();
    atm_sspd = new QLabel();
    atm_groundTemp = new QLabel();
//    atm_wind[0] = new QLabel();
//    atm_wind[1] = new QLabel();
//    atm_wind[2] = new QLabel();

    commandedSufaces[0] = new QLabel();
    commandedSufaces[1] = new QLabel();
    commandedSufaces[2] = new QLabel();
    commandedSufaces[3] = new QLabel();
    commandedFlaps = new QLabel();
    currentFlaps = new QLabel();
    commandedBrakes = new QLabel();
    commandedTO = new QLabel();
    commandedPropeller = new QLabel();

    sb_forces[0] = new QLabel();
    sb_forces[1] = new QLabel();
    sb_forces[2] = new QLabel();

    body_forces[0] = new QLabel();
    body_forces[1] = new QLabel();
    body_forces[2] = new QLabel();

    frequency = new QLabel();
    simtime = new QLabel();

    CD = new QLabel();
    CL = new QLabel();
    Effi = new QLabel();

    missile[0] = new QLabel();
    missile[1] = new QLabel();

    for(int i = 0; i< 3; i++)
    {
        mass_center[i] = new QLabel();
        for(int j = 0; j < 3; j++)
        {
            inertia[i][j] = new QLabel();
        }
    }
    totalMass = new QLabel();
    fuelMass= new QLabel();

    engineRPM = new QLabel();
    propellerRPM = new QLabel();
    thrust = new QLabel();
    shp = new QLabel();
    betaeng = new QLabel();
    fuel_consumption = new QLabel();


    xyzLabel[0] = new QLabel("<b>X</b>");
    xyzLabel[1] = new QLabel("<b>Y</b>");
    xyzLabel[2] = new QLabel("<b>Z</b>");
    rdeLabel[0] = new QLabel("<b>Radian</b>");
    rdeLabel[1] = new QLabel("<b>Degree</b>");

    // SIMULATION
    QLabel *simulationTitle = new QLabel(tr("Simulation Data"));
    simulationTitle->setStyleSheet(titleCss);
    gridSimula->addWidget(simulationTitle,0,0,1,2);
    gridSimula->addWidget(new QLabel(tr("<b>Frequency</b>")),1,0);
    gridSimula->addWidget(frequency,1,1,Qt::AlignRight);
    gridSimula->addWidget(new QLabel(tr("<b>Time</b>")),2,0);
    gridSimula->addWidget(simtime,2,1,Qt::AlignRight);
    gridSimula->setRowStretch(20,20);
    // FORCES
    QLabel *forcesTitle = new QLabel(tr("Forces Data"));
    forcesTitle->setStyleSheet(titleCss);
    gridForces->addWidget(forcesTitle,0,0,1,2);
    gridForces->addWidget(new QLabel(tr("<b>Lift</b>")),1,0);
    gridForces->addWidget(sb_forces[0],1,1,Qt::AlignRight);
    gridForces->addWidget(new QLabel(tr("<b>Drag</b>")),2,0);
    gridForces->addWidget(sb_forces[1],2,1,Qt::AlignRight);
    gridForces->addWidget(new QLabel(tr("<b>Lateral</b>")),3,0);
    gridForces->addWidget(sb_forces[2],3,1,Qt::AlignRight);
    gridForces->addWidget(new QLabel(tr("<b>Body X</b>")),4,0);
    gridForces->addWidget(body_forces[0],4,1,Qt::AlignRight);
    gridForces->addWidget(new QLabel(tr("<b>Body Y</b>")),5,0);
    gridForces->addWidget(body_forces[1],5,1,Qt::AlignRight);
    gridForces->addWidget(new QLabel(tr("<b>Body Z</b>")),6,0);
    gridForces->addWidget(body_forces[2],6,1,Qt::AlignRight);
    gridForces->setRowStretch(20,20);
    // POSITION
    QLabel *globalTitle = new QLabel(tr("Global Data"));
    QLabel *latitudeLabel = new QLabel(tr("<b>Latitude</b>"));
    QLabel *longitudeLabel = new QLabel(tr("<b>Longitude</b>"));
    QLabel *altitudeLabel = new QLabel(tr("<b>Altitude</b>"));
    QLabel *groundAltLabel = new QLabel(tr("<b>Ground Altitude</b>"));
    globalTitle->setStyleSheet(titleCss);
    gridGlobal->addWidget(globalTitle,0,0,1,3);
    gridGlobal->addWidget(latitudeLabel,2,0);
    gridGlobal->addWidget(longitudeLabel,3,0);
    gridGlobal->addWidget(altitudeLabel,4,0);
    gridGlobal->addWidget(groundAltLabel,5,0);
    gridGlobal->addWidget(position[0],2,1,Qt::AlignRight);
    gridGlobal->addWidget(position[1],3,1,Qt::AlignRight);
    gridGlobal->addWidget(lat_deg,2,2,Qt::AlignRight);
    gridGlobal->addWidget(lon_deg,3,2,Qt::AlignRight);
    gridGlobal->addWidget(position[2],4,2,Qt::AlignRight);
    gridGlobal->addWidget(ground_pos,5,2,Qt::AlignRight);

    // VELOCITIES
    QLabel *velocitiesTitle = new QLabel(tr("Velocities"));
    velocitiesTitle->setStyleSheet(titleCss);
    //velocitiesTitle->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    QLabel *velocityeLabel = new QLabel(tr("<b>Velocities Earth</b>"));
    QLabel *velocityblabel = new QLabel(tr("<b>Velocities Body</b>"));
    QLabel *airspeedlabel = new QLabel(tr("<b>Air Speed</b>"));
    QLabel *groundspeedlabel = new QLabel(tr("<b>Ground Speed</b>"));
    gridSpeed->addWidget(velocitiesTitle,0,0,1,4);
    gridSpeed->addWidget(xyzLabel[0],1,1,Qt::AlignCenter);
    gridSpeed->addWidget(xyzLabel[1],1,2,Qt::AlignCenter);
    gridSpeed->addWidget(xyzLabel[2],1,3,Qt::AlignCenter);
    gridSpeed->addWidget(velocityeLabel,2,0);
    gridSpeed->addWidget(velocity_e[0],2,1,Qt::AlignCenter);
    gridSpeed->addWidget(velocity_e[1],2,2,Qt::AlignCenter);
    gridSpeed->addWidget(velocity_e[2],2,3,Qt::AlignCenter);
    gridSpeed->addWidget(velocityblabel,3,0);
    gridSpeed->addWidget(velocity_b[0],3,1,Qt::AlignCenter);
    gridSpeed->addWidget(velocity_b[1],3,2,Qt::AlignCenter);
    gridSpeed->addWidget(velocity_b[2],3,3,Qt::AlignCenter);
//    gridSpeed->addWidget(new QLabel(tr("<b>Wind Velocity</b>")),4,0);
//    gridSpeed->addWidget(atm_wind[0],4,1,Qt::AlignCenter);
//    gridSpeed->addWidget(atm_wind[1],4,2,Qt::AlignCenter);
//    gridSpeed->addWidget(atm_wind[2],4,3,Qt::AlignCenter);
    gridSpeed->addWidget(airspeedlabel,5,0);
    gridSpeed->addWidget(speeds[0],5,1,Qt::AlignCenter);
    gridSpeed->addWidget(groundspeedlabel,6,0);
    gridSpeed->addWidget(speeds[1],6,1,Qt::AlignCenter);
    // ANGLES
    QLabel *anglesTitle = new QLabel(tr("Angles"));
    QLabel *alphaLabel = new QLabel(tr("<b>Alpha</b>"));
    QLabel *betaLabel = new QLabel(tr("<b>Beta</b>"));
    anglesTitle->setStyleSheet(titleCss);
    gridAngles->addWidget(anglesTitle,0,0,1,3);
    gridAngles->addWidget(rdeLabel[0],1,1,Qt::AlignCenter);
    gridAngles->addWidget(rdeLabel[1],1,2,Qt::AlignCenter);
    gridAngles->addWidget(new QLabel(tr("<b>Precesion</b>")),2,0,Qt::AlignRight);
    gridAngles->addWidget(new QLabel(tr("<b>Nutation</b>")),3,0,Qt::AlignRight);
    gridAngles->addWidget(new QLabel(tr("<b>Intr. Rotation</b>")),4,0,Qt::AlignRight);
    gridAngles->addWidget(eulerangles[0],2,1,Qt::AlignCenter);
    gridAngles->addWidget(eulerangles[1],3,1,Qt::AlignCenter);
    gridAngles->addWidget(eulerangles[2],4,1,Qt::AlignCenter);
    gridAngles->addWidget(eulerangles_deg[0],2,2,Qt::AlignCenter);
    gridAngles->addWidget(eulerangles_deg[1],3,2,Qt::AlignCenter);
    gridAngles->addWidget(eulerangles_deg[2],4,2,Qt::AlignCenter);

    gridAngles->addWidget(alphaLabel,5,0,Qt::AlignRight);
    gridAngles->addWidget(alpha_beta[0],5,1,Qt::AlignCenter);
    gridAngles->addWidget(alpha_beta_deg[0],5,2,Qt::AlignCenter);
    gridAngles->addWidget(betaLabel,6,0,Qt::AlignRight);
    gridAngles->addWidget(alpha_beta[1],6,1,Qt::AlignCenter);
    gridAngles->addWidget(alpha_beta_deg[1],6,2,Qt::AlignCenter);

    // ATMOSPHERE
    QLabel *atmosphereTitle = new QLabel(tr("Atmospherical Data"));
    atmosphereTitle->setStyleSheet(titleCss);
    gridAtmosp->addWidget(atmosphereTitle,0,0,1,2);
    gridAtmosp->addWidget(new QLabel(tr("<b>Density</b>")),1,0);
    gridAtmosp->addWidget(atm_dens,1,1,Qt::AlignRight);
    gridAtmosp->addWidget(new QLabel(tr("<b>Pressure</b>")),2,0);
    gridAtmosp->addWidget(atm_pres,2,1,Qt::AlignRight);
    gridAtmosp->addWidget(new QLabel(tr("<b>Temperature</b>")),3,0);
    gridAtmosp->addWidget(atm_temp,3,1,Qt::AlignRight);
    gridAtmosp->addWidget(new QLabel(tr("<b>Sound Speed</b>")),4,0);
    gridAtmosp->addWidget(atm_sspd,4,1,Qt::AlignRight);
    gridAtmosp->addWidget(new QLabel(tr("<b>Sea Level Temperature</b>")),5,0);
    gridAtmosp->addWidget(atm_groundTemp,5,1,Qt::AlignRight);

    //OTHER DATA
    QLabel *otherTitle = new QLabel(tr("Other Data"));
    otherTitle->setStyleSheet(titleCss);
    gridOther->addWidget(otherTitle,0,0,1,2);
    gridOther->addWidget(new QLabel(tr("<b>Flap Status</b>")),1,0);
    gridOther->addWidget(currentFlaps,1,1,Qt::AlignRight);


    // COMMAND
    QLabel *commandedTitle = new QLabel(tr("Commanded Data"));
    commandedTitle->setStyleSheet(titleCss);
    gridCommand->addWidget(commandedTitle,0,0,1,4);
    gridCommand->addWidget(new QLabel(tr("<b>Elevator</b>")),1,0);
    gridCommand->addWidget(commandedSufaces[0],1,1,Qt::AlignRight);
    gridCommand->addWidget(new QLabel(tr("<b>Ailerons</b>")),1,2);
    gridCommand->addWidget(commandedSufaces[1],1,3,Qt::AlignRight);
    gridCommand->addWidget(new QLabel(tr("<b>Rudder</b>")),2,0);
    gridCommand->addWidget(commandedSufaces[3],2,1,Qt::AlignRight);
    gridCommand->addWidget(new QLabel(tr("<b>Throttle</b>")),2,2);
    gridCommand->addWidget(commandedSufaces[2],2,3,Qt::AlignRight);
    gridCommand->addWidget(new QLabel(tr("<b>Take-Off M.</b>")),3,0);
    gridCommand->addWidget(commandedTO,3,1,Qt::AlignRight);
    gridCommand->addWidget(new QLabel(tr("<b>Propeller</b>")),3,2);
    gridCommand->addWidget(commandedPropeller,3,3,Qt::AlignRight);
    gridCommand->addWidget(new QLabel(tr("<b>Flaps</b>")),4,0);
    gridCommand->addWidget(commandedFlaps,4,1,Qt::AlignRight);
    gridCommand->addWidget(new QLabel(tr("<b>Brakes</b>")),4,2);
    gridCommand->addWidget(commandedBrakes,4,3,Qt::AlignRight);

    QLabel *aerodynamicTitle = new QLabel(tr("Aerodynamic Data"));
    aerodynamicTitle->setStyleSheet(titleCss);
    gridAerodyn->addWidget(aerodynamicTitle,0,0,1,4);
    gridAerodyn->addWidget(new QLabel(tr("<b>CL</b>")),1,0);
    gridAerodyn->addWidget(CL,1,1,Qt::AlignRight);
    gridAerodyn->addWidget(new QLabel(tr("<b>CD</b>")),1,2);
    gridAerodyn->addWidget(CD,1,3,Qt::AlignRight);
    gridAerodyn->addWidget(new QLabel(tr("<b>Efficiency</b>")),2,2);
    gridAerodyn->addWidget(Effi,2,3,Qt::AlignRight);

    QLabel *payloadTitle = new QLabel(tr("Payload Data"));
    payloadTitle->setStyleSheet(titleCss);
    gridPayload->addWidget(payloadTitle,0,0,1,4);
    gridPayload->addWidget(new QLabel(tr("<b>R Missile</b>")),1,0);
    gridPayload->addWidget(missile[0],1,1,Qt::AlignRight);
    gridPayload->addWidget(new QLabel(tr("<b>L Missile</b>")),1,2);
    gridPayload->addWidget(missile[1],1,3,Qt::AlignRight);
    QLabel *massTitle = new QLabel(tr("Mass Data"));
    massTitle->setStyleSheet(titleCss);
    gridMass->addWidget(massTitle,0,0,1,3);
    gridMass->addWidget(new QLabel(tr("<b>Total Mass</b>")),1,0,1,2,Qt::AlignLeft);
    gridMass->addWidget(totalMass,1,2,1,1,Qt::AlignRight);
    gridMass->addWidget(new QLabel(tr("<b>Fuel Remaining</b>")),2,0,1,2,Qt::AlignLeft);
    gridMass->addWidget(fuelMass,2,2,1,1,Qt::AlignRight);
    gridMass->addWidget(new QLabel(tr("<b>Mass Center</b>")),3,0,1,3,Qt::AlignCenter);
    gridMass->addWidget(mass_center[0],4,0,Qt::AlignRight);
    gridMass->addWidget(mass_center[1],4,1,Qt::AlignRight);
    gridMass->addWidget(mass_center[2],4,2,Qt::AlignRight);
    gridMass->addWidget(new QLabel(tr("<b>Inertia Matrix</b>")),5,0,1,3,Qt::AlignCenter);
    gridMass->addWidget(inertia[0][0],6,0,Qt::AlignRight);
    gridMass->addWidget(inertia[0][1],6,1,Qt::AlignRight);
    gridMass->addWidget(inertia[0][2],6,2,Qt::AlignRight);
    gridMass->addWidget(inertia[1][0],7,0,Qt::AlignRight);
    gridMass->addWidget(inertia[1][1],7,1,Qt::AlignRight);
    gridMass->addWidget(inertia[1][2],7,2,Qt::AlignRight);
    gridMass->addWidget(inertia[2][0],8,0,Qt::AlignRight);
    gridMass->addWidget(inertia[2][1],8,1,Qt::AlignRight);
    gridMass->addWidget(inertia[2][2],8,2,Qt::AlignRight);

    QLabel *engineTitle = new QLabel(tr("<b>Engine Data</b>"));
    engineTitle->setStyleSheet(titleCss);
    gridEngine->addWidget(engineTitle,0,0,1,2);
    gridEngine->addWidget(new QLabel(tr("<b>Engine RPM</b>")),1,0);
    gridEngine->addWidget(engineRPM,1,1,Qt::AlignRight);
    gridEngine->addWidget(new QLabel(tr("<b>Propeller RPM</b>")),2,0);
    gridEngine->addWidget(propellerRPM,2,1,Qt::AlignRight);
    gridEngine->addWidget(new QLabel(tr("<b>Beta 0.75</b>")),3,0);
    gridEngine->addWidget(betaeng,3,1,Qt::AlignRight);
    gridEngine->addWidget(new QLabel(tr("<b>Thrust</b>")),4,0);
    gridEngine->addWidget(thrust,4,1,Qt::AlignRight);
    gridEngine->addWidget(new QLabel(tr("<b>Shaft Power</b>")),5,0);
    gridEngine->addWidget(shp,5,1,Qt::AlignRight);
    gridEngine->addWidget(new QLabel(tr("<b>Fuel Consumption</b>")),6,0);
    gridEngine->addWidget(fuel_consumption,6,1,Qt::AlignRight);





    column1->addLayout(gridGlobal);
    column1->addLayout(gridSpeed);
    column1->addLayout(gridAngles);
    column1->addLayout(gridAtmosp);
    column1->addLayout(gridEngine);
    column1->addStretch(50);

    column2->addLayout(gridCommand);
    column2->addLayout(gridOther);
    column2->addLayout(gridForces);
    column2->addLayout(gridAerodyn);
    column2->addLayout(gridSimula);
    column2->addLayout(gridPayload);
    column2->addLayout(gridMass);
    column2->addStretch(50);


    column1->setContentsMargins(5,5,5,5);
    column2->setContentsMargins(5,5,5,5);

    mainGrid->addLayout(column1,1);
    mainGrid->addLayout(column2,1);
    scrollInner = new QWidget();
    scrollInner->setLayout(mainGrid);
    scrollInner->setStyleSheet("background-color:#FDFDFD");
    scrollInner->setStyleSheet("background-color:#222222; color: #EEEEEE;");
    scrollInner->setMinimumWidth(600);
    scrollInner->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    scroll01->setWidget(scrollInner);
    scroll01->setWidgetResizable(true);
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(scroll01);
    vLayout->setContentsMargins(0,0,0,0);
    this->setLayout(vLayout);
}
/*!
 * \brief numericalData::receivePackage updates the displayed values
 * \param _plane status to display
 */
void numericalData::receivePackage(planeStatusData _plane)
{
    bool oldstate;
    oldstate = this->blockSignals(true);


    this->position[0]->setText(QString("%1 rad").arg(_plane.state.position[0],11,'F',7));
    this->position[1]->setText(QString("%1 rad").arg(_plane.state.position[1],11,'F',7));
    this->lat_deg->setText(QString("%1 deg").arg(_plane.state.position[0]*RAD2DEG_,11,'F',7));
    this->lon_deg->setText(QString("%1 deg").arg(_plane.state.position[1]*RAD2DEG_,11,'F',7));
    this->position[2]->setText(QString("%1 m").arg(_plane.state.position[2],11,'F',3));
    this->ground_pos->setText(QString("%1 m").arg(_plane.state.ground_altitude,11,'F',3));
    this->velocity_e[0]->setText(QString("%1").arg(_plane.state.velocity_e[0],velwidth,'F',veldec));
    this->velocity_e[1]->setText(QString("%1").arg(_plane.state.velocity_e[1],velwidth,'F',veldec));
    this->velocity_e[2]->setText(QString("%1").arg(_plane.state.velocity_e[2],velwidth,'F',veldec));
    this->velocity_b[0]->setText(QString("%1").arg(_plane.state.velocity_b[0],velwidth,'F',veldec));
    this->velocity_b[1]->setText(QString("%1").arg(_plane.state.velocity_b[1],velwidth,'F',veldec));
    this->velocity_b[2]->setText(QString("%1").arg(_plane.state.velocity_b[2],velwidth,'F',veldec));
    this->eulerangles[0]->setText(QString("%1").arg(_plane.state.eulerangles[0],radwidth,'F',raddec));
    this->eulerangles[1]->setText(QString("%1").arg(_plane.state.eulerangles[1],radwidth,'F',raddec));
    this->eulerangles[2]->setText(QString("%1").arg(_plane.state.eulerangles[2],radwidth,'F',raddec));
    this->eulerangles_deg[0]->setText(QString("%1").arg(_plane.state.eulerangles[0]*RAD2DEG_,degwidth,'F',degdec));
    this->eulerangles_deg[1]->setText(QString("%1").arg(_plane.state.eulerangles[1]*RAD2DEG_,degwidth,'F',degdec));
    this->eulerangles_deg[2]->setText(QString("%1").arg(_plane.state.eulerangles[2]*RAD2DEG_,degwidth,'F',degdec));
    this->alpha_beta[0]->setText(QString("%1").arg(_plane.state.alpha_beta[0],radwidth,'F',raddec));
    this->alpha_beta[1]->setText(QString("%1").arg(_plane.state.alpha_beta[1],radwidth,'F',raddec));
    this->alpha_beta_deg[0]->setText(QString("%1").arg(_plane.state.alpha_beta[0]*RAD2DEG_,radwidth,'F',raddec));
    this->alpha_beta_deg[1]->setText(QString("%1").arg(_plane.state.alpha_beta[1]*RAD2DEG_,radwidth,'F',raddec));
    this->speeds[0]->setText(QString("%1").arg(_plane.state.speed_air,velwidth,'F',veldec));
    this->speeds[1]->setText(QString("%1").arg(sqrt(_plane.state.velocity_e[0]*_plane.state.velocity_e[0]+_plane.state.velocity_e[1]*_plane.state.velocity_e[1]+_plane.state.velocity_e[2]*_plane.state.velocity_e[2]),velwidth,'F',veldec));
    //Atmosphere
    this->atm_dens->setText(QString("%1 kg/m^3").arg(_plane.atmosphere.d,2,'F',5));
    this->atm_pres->setText(QString("%1 Pa").arg(_plane.atmosphere.p,7,'F',0));
    this->atm_temp->setText(QString("%1 K").arg(_plane.atmosphere.t,5,'F',2));
    this->atm_sspd->setText(QString("%1 m/s").arg(_plane.atmosphere.a,4,'F',2));
    this->atm_groundTemp->setText(QString("%1 m/s").arg(_plane.atmosphere.groundTemperature,4,'F',2));
//    this->atm_wind[0]->setText(QString("%1").arg(_plane.atmosphere.wind_sp[0],velwidth,'F',veldec));
//    this->atm_wind[1]->setText(QString("%1").arg(_plane.atmosphere.wind_sp[1],velwidth,'F',veldec));
//    this->atm_wind[2]->setText(QString("%1").arg(_plane.atmosphere.wind_sp[2],velwidth,'F',veldec));
    //COmmanded
    this->commandedSufaces[0]->setText(QString("%1").arg(_plane.commanded.elevator,velwidth,'F',veldec));
    this->commandedSufaces[1]->setText(QString("%1").arg(_plane.commanded.ailerons,velwidth,'F',veldec));
    this->commandedSufaces[2]->setText(QString("%1").arg(_plane.commanded.throttle,velwidth,'F',veldec));
    this->commandedSufaces[3]->setText(QString("%1").arg(_plane.commanded.rudder,velwidth,'F',veldec));
    this->commandedPropeller->setText(QString("%1").arg(_plane.commanded.propeller,velwidth,'F',veldec));
    this->commandedFlaps->setText(QString("%1").arg(_plane.commanded.flaps*15.0f,velwidth,'F',veldec));
    this->currentFlaps->setText(QString("%1").arg(_plane.derivatives.current_flaps,velwidth,'F',veldec));

    //FOrces
    this->sb_forces[0]->setText(QString("%1 N").arg(-_plane.state.forces_air_sb[2],forceswidth,'F',forcesdec));
    this->sb_forces[1]->setText(QString("%1 N").arg(-_plane.state.forces_air_sb[0],forceswidth,'F',forcesdec));
    this->sb_forces[2]->setText(QString("%1 N").arg(_plane.state.forces_air_sb[1],forceswidth,'F',forcesdec));
    this->body_forces[0]->setText(QString("%1 N").arg(_plane.state.forces_b[0],forceswidth,'F',forcesdec));
    this->body_forces[1]->setText(QString("%1 N").arg(_plane.state.forces_b[1],forceswidth,'F',forcesdec));
    this->body_forces[2]->setText(QString("%1 N").arg(_plane.state.forces_b[2],forceswidth,'F',forcesdec));
    //SImulation
    this->frequency->setText(QString("%1 Hz").arg((float)_plane.time.frequency*1.0f,forceswidth,'F',forcesdec));
    this->simtime->setText(QString("%1 seg").arg((float)_plane.time.seconds + _plane.time.f_seconds,11,'F',3));
    //Aerodynamic
    this->CD->setText(QString("%1").arg(_plane.derivatives.CD,radwidth,'F',raddec));
    this->CL->setText(QString("%1").arg(_plane.derivatives.CL,radwidth,'F',raddec));
    this->Effi->setText(QString("%1").arg(_plane.derivatives.CL/_plane.derivatives.CD,radwidth,'F',raddec));
    //Missile
    if(_plane.commanded.missile[0]) this->missile[0]->setText("Hold");
    else  this->missile[0]->setText("Launch");
    if(_plane.commanded.missile[1]) this->missile[1]->setText("Hold");
    else  this->missile[1]->setText("Launch");
    //MASS
    for(int i = 0; i< 3; i++)
    {
        this->mass_center[i]->setText(QString("%1").arg(_plane.derivatives.mass_center[i],radwidth,'F',raddec));
        for(int j = 0; j<3; j++)
        {
            this->inertia[i][j]->setText(QString("%1").arg(_plane.derivatives.inertias[i][j],velwidth,'F',veldec));
        }
    }
    this->totalMass->setText(QString("%1").arg(_plane.derivatives.mass,velwidth,'F',veldec));
    this->fuelMass->setText(QString("%1 %").arg(_plane.state.fuel_ratio*100,velwidth,'F',veldec));

    this->engineRPM->setText(QString("%1 RPM").arg(_plane.engine.engine_rpm,forceswidth,'F',forcesdec));
    this->propellerRPM->setText(QString("%1 RPM").arg(_plane.engine.propeller_rpm,forceswidth,'F',forcesdec));
    this->betaeng->setText(QString("%1 deg").arg(_plane.engine.beta,velwidth,'F',veldec));
    this->thrust->setText(QString("%1 N").arg(_plane.engine.thrust,velwidth,'F',veldec));
    this->shp->setText(QString("%1 kW").arg(_plane.engine.shaft_power*0.001,velwidth,'F',veldec));
    this->fuel_consumption->setText(QString("%1 g/seg").arg(_plane.engine.fuel_consumption*1000,velwidth,'F',veldec));

    if(_plane.commanded.wheelBreak){
        this->commandedBrakes->setText("On");
    }else{
        this->commandedBrakes->setText("Off");
    }
    if(_plane.commanded.takeoffmode){
        this->commandedTO->setText("On");
    }else{
        this->commandedTO->setText("Off");
    }

    this->blockSignals(oldstate);
}
