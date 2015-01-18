#ifndef NUMERICALDATA_H
#define NUMERICALDATA_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include "src/simengine/planestatusdata.h"
#include "math.h"
#include "stdlib.h"

/*!
 * \brief The numericalData class is a widget containing all the displayed numerical information
 */
class numericalData : public QWidget
{
    Q_OBJECT
public:
    explicit numericalData(QWidget *parent = 0);
private:
    int veldec;
    int velwidth;
    int raddec;
    int radwidth;
    int degdec;
    int degwidth;
    int forceswidth;
    int forcesdec;

    QScrollArea *scroll01;
    QWidget *scrollInner ;
    QLabel *position[3];
    QLabel *lat_deg;
    QLabel *lon_deg;
    QLabel *ground_pos;
    QLabel *velocity_e[3];
    QLabel *velocity_b[3];
    QLabel *eulerangles[3];
    QLabel *eulerangles_deg[3];
    QLabel *velang_euler[3];
    QLabel *alpha_beta[2];
    QLabel *alpha_beta_deg[2];
    QLabel *speeds[3];
    QLabel *xyzLabel[3];
    QLabel *rdeLabel[2];
    QLabel *atm_pres;
    QLabel *atm_dens;
    QLabel *atm_temp;
    QLabel *atm_sspd;
    QLabel *atm_groundTemp;
//    QLabel *atm_wind[3];

    QLabel *commandedSufaces[4];
    QLabel *commandedFlaps;
    QLabel *currentFlaps;
    QLabel *commandedBrakes;
    QLabel *commandedTO;
    QLabel *commandedPropeller;
    QLabel *sb_forces[3];
    QLabel *body_forces[3];

    QLabel *missile[2];

    QLabel *CL;
    QLabel *CD;
    QLabel *Effi;

    QLabel *frequency;
    QLabel *simtime;
    //MAss
    QLabel *inertia[3][3];
    QLabel *mass_center[3];
    QLabel *totalMass;
    QLabel *fuelMass;

    //ENGINE
    QLabel *engineRPM;
    QLabel *propellerRPM;
    QLabel *thrust;
    QLabel *shp;
    QLabel *betaeng;
    QLabel *fuel_consumption;

public slots:
    void receivePackage(planeStatusData);
    
};

#endif // NUMERICALDATA_H
