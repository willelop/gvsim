#ifndef COMMANDSTATUS_H
#define COMMANDSTATUS_H

#include <QDebug>
#include <QDialog>
#include <stdlib.h>
#include <math.h>
#include <QUdpSocket>
#include <QTimer>
#include <QHostAddress>
#include <QVariantList>
#include "typedefs.h"
#include "commandstatusdata.h"
#include "utils/net_ctrls.h"
#ifdef Q_OS_WIN32
#include <Winsock2.h>
#else
#include <asm/byteorder.h>
#include <netinet/in.h>
#endif

#define CTRLNUM 5
#define INCREMENT 0.1
using namespace std;
/*!
 * \brief The commandStatus class handles the command information interfacing between the inputs and the simulation engine.
 */
class commandStatus : public QObject
{
    Q_OBJECT
public:
    commandStatus();
    const commandStatusData &status;
private:
    commandStatusData internalData;
    QTimer *surfacesTimer[3];
    commandStatusData controlData;
    bool use_flightgear;
    QUdpSocket *inputsocket;
    QHostAddress host;
    int port;
    bool overRide[CTRLNUM];
    float overRideValue[CTRLNUM];
    bool overRide_surfaces;
    bool overRide_engine;
    bool joyEnabled;
    bool mix_throttle;
    void normalize(commandStatusData*);

    double ntohd( double);
    float  ntohf(float);
public slots:
    void setSticks(Sint16,Sint16,Sint16,Sint16);
    void setKeys(inputs::CONTROLKEYS);
    void readfromflightgear();
    void enabledJoy(bool);
    void updatereadFG(bool);
    void getNewControlStatus(commandStatusData,bool forcing_surfaces,bool forcing_engine,bool mix_throt);
private slots:
    void clearAirleron();
    void clearElevator();
    void clearRudder();
signals:
    void commandUpdated(commandStatusData);
};

#endif // COMMANDDATA_H
