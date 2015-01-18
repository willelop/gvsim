#include "commandstatus.h"
#define key_inc 0.05
using namespace std;
commandStatus::commandStatus(): status(internalData)

{
    for(int i = 0; i < CTRLNUM; i++)
    {
        overRide[i] = false;
        overRideValue[i] = 0.0f;
    }
    for(int i = 0; i < 3; i++)
    {
        surfacesTimer[i] = new QTimer();
        surfacesTimer[i]->setInterval(40);
        surfacesTimer[i]->start();
    }
    joyEnabled = false;
    use_flightgear = false;
    this->mix_throttle = false;
    inputsocket = new QUdpSocket();
    host = QHostAddress("127.0.0.1");
    port = 5700;
    inputsocket->bind(host,port);
    internalData.ailerons = 0;
    internalData.rudder = 0;
    internalData.elevator = 0;
    internalData.throttle = 0;
    internalData.propeller = 0;
    internalData.aileronTrim =0;
    internalData.elevatorTrim = 0;
    internalData.rudderTrim = 0;
    internalData.flaps = 0;
    internalData.missile[0] = true;
    internalData.missile[1] = true;
    internalData.gearDown = true;
    internalData.takeoffmode = false;
    internalData.wheelBreak = false;
    overRide_surfaces = true;
    overRide_engine = true;
    connect(inputsocket,SIGNAL(readyRead()),this,SLOT(readfromflightgear()));
    connect(surfacesTimer[0],SIGNAL(timeout()),this,SLOT(clearAirleron()));
    connect(surfacesTimer[1],SIGNAL(timeout()),this,SLOT(clearElevator()));
    connect(surfacesTimer[2],SIGNAL(timeout()),this,SLOT(clearRudder()));

    qDebug() << "Input Socket ready";

}
void commandStatus::setSticks(Sint16 pitch, Sint16 ailerons, Sint16 throttle, Sint16 rudder)
{
    if(!use_flightgear)
    {
        if(!overRide_surfaces){
            internalData.ailerons = max(-1.0,min(1.0,ailerons*0.00003051757));
            internalData.elevator = max(-1.0,min(1.0,pitch*0.00003051757));
            internalData.rudder = max(-1.0,min(1.0,rudder*0.00003051757));
        }
        if(!overRide_engine){
            internalData.throttle = max(0.0,min(1.0,1 - throttle*0.00003051757*0.5 - 0.5));
            if(mix_throttle) internalData.propeller = internalData.throttle;
        }
        emit commandUpdated(internalData);
    }
}
void commandStatus::setKeys(inputs::CONTROLKEYS _axe)
{
    if(!use_flightgear)
    {
        switch(_axe)
        {
        case inputs::AILERONLEFT:
            if(internalData.ailerons < 1 && !overRide_surfaces) internalData.ailerons += key_inc;
            if(!joyEnabled) this->surfacesTimer[0]->start();
            break;
        case inputs::AILERONRIGHT:
            if(internalData.ailerons > -1 && !overRide_surfaces)internalData.ailerons -= key_inc;
            if(!joyEnabled) this->surfacesTimer[0]->start();
            break;
        case inputs::RUDDERLEFT:
            if(internalData.rudder < 1 && !overRide_surfaces) internalData.rudder += key_inc;
            if(!joyEnabled) this->surfacesTimer[2]->start();
            break;
        case inputs::RUDDERRIGHT:
            if(internalData.rudder > -1 && !overRide_surfaces)internalData.rudder -= key_inc;
            if(!joyEnabled) this->surfacesTimer[2]->start();
            break;
        case inputs::ELEVATORUP:
            if(internalData.elevator < 1 && !overRide_surfaces) internalData.elevator += key_inc;
            if(!joyEnabled) this->surfacesTimer[1]->start();
            break;
        case inputs::ELEVATORDOWN:
            if(internalData.elevator > -1 && !overRide_surfaces) internalData.elevator -= key_inc;
            if(!joyEnabled) this->surfacesTimer[1]->start();
            break;
        case inputs::THROTTLEUP:
            if(internalData.throttle < 1 && !overRide_engine) internalData.throttle += key_inc;
            break;
        case inputs::THROTTLEDOWN:
            if(internalData.throttle > 0 && !overRide_engine)internalData.throttle -= key_inc;
            break;
        case inputs::BREAKS:
            internalData.wheelBreak = !internalData.wheelBreak;
            break;
        case inputs::GEARTOGGLE:
            internalData.gearDown = !internalData.gearDown;
            break;
        default:
            break;
        }
        this->normalize(&internalData);
        if(mix_throttle) internalData.propeller = internalData.throttle;
        emit commandUpdated(internalData);
    }
}
void commandStatus::normalize(commandStatusData *command)
{
    if(command->throttle < 0) command->throttle = 0;
    if(command->throttle > 1) command->throttle = 1;
    if(command->ailerons < -1) command->ailerons = -1;
    if(command->ailerons > 1) command->ailerons = 1;
    if(command->rudder < -1) command->rudder = -1;
    if(command->rudder > 1) command->rudder = 1;
    if(command->elevator < -1) command->elevator = -1;
    if(command->elevator > 1) command->elevator = 1;
}

void commandStatus::readfromflightgear()
{
    FGNetCtrls controls;
    if(use_flightgear)
    while(inputsocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(inputsocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        inputsocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        if(datagram.size() == sizeof(controls)){
            memmove(&controls,datagram.data(),datagram.size());
            if(!overRide_surfaces){
                internalData.ailerons = ntohd(controls.aileron);
                internalData.rudder = ntohd(controls.rudder);
                internalData.elevator = ntohd(controls.elevator);
            }
            if(!overRide_engine){
                internalData.throttle = ntohd(controls.throttle[0]);
                if(mix_throttle) internalData.propeller = internalData.throttle;
            }
            emit commandUpdated(internalData);
        }else{
            qDebug() << "Wrong Version";
        }
    }
}
void commandStatus::updatereadFG(bool _state)
{
    this->use_flightgear = _state;
}

double commandStatus::ntohd (double x)
{
        int    *Double_Overlay;
        int     Holding_Buffer;

        Double_Overlay = (int *) &x;
        Holding_Buffer = Double_Overlay [0];

        Double_Overlay [0] = ntohl(Double_Overlay [1]);
        Double_Overlay [1] = ntohl(Holding_Buffer);
        return x;
}


float commandStatus::ntohf (float x)
{

        int    *Float_Overlay;
        int     Holding_Buffer;

        Float_Overlay = (int *) &x;
        Holding_Buffer = Float_Overlay [0];

        Float_Overlay [0] = ntohl(Holding_Buffer);
        return x;
}

void commandStatus::getNewControlStatus(commandStatusData _instate,bool forcing_surfaces, bool forcing_engine,bool mix_throt)
{
    this->mix_throttle = mix_throt;
    this->overRide_engine = forcing_engine;
    this->overRide_surfaces = forcing_surfaces;
    this->controlData = _instate;
    if(!forcing_surfaces){
        controlData.ailerons = internalData.ailerons;
        controlData.elevator = internalData.elevator;
        controlData.rudder = internalData.rudder;
    }else{
        internalData.ailerons = controlData.ailerons;
        internalData.elevator = controlData.elevator;
        internalData.rudder = controlData.rudder;
    }
    if(!forcing_engine){
        controlData.throttle = internalData.throttle;
        if(mix_throt) controlData.propeller = internalData.throttle;
    }else{
        internalData.throttle = controlData.throttle;
        internalData.propeller = controlData.propeller;

    }
    this->internalData = controlData;
    emit commandUpdated(controlData);
}
void commandStatus::clearAirleron()
{
    if(!this->overRide_surfaces){
        this->internalData.ailerons += INCREMENT*(0-this->internalData.ailerons);
        emit commandUpdated(internalData);
    }
}
void commandStatus::clearElevator()
{
    if(!this->overRide_surfaces){
        this->internalData.elevator += INCREMENT*(0-this->internalData.elevator);
        emit commandUpdated(internalData);
    }
}
void commandStatus::clearRudder()
{
    if(!this->overRide_surfaces){
        this->internalData.rudder += INCREMENT*(0-this->internalData.rudder);
        emit commandUpdated(internalData);
    }
}
void commandStatus::enabledJoy(bool _en)
{
    this->joyEnabled = _en;
    if(_en){
        for(int i = 0; i < 3; i++)
        {
            surfacesTimer[i]->stop();
        }
    }else{
        for(int i = 0; i < 3; i++)
        {
            surfacesTimer[i]->start();
        }
    }
}
