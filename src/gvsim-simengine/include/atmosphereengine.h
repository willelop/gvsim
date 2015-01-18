#ifndef ATMOSPHEREENGINE_H
#define ATMOSPHEREENGINE_H


#include <math.h>
#include <stdlib.h>
#include <QElapsedTimer>
#include "planestatusdata.h"


#define G_ACCEL 9.81f

#define EARTH_RADIUS_ 6356766.0f
#define GAMMA 1.4f
#define T_SEALEVEL 288.0f
#define P_SEALEVEL 101325.0f
#define D_SEALEVEL 1.225f
#define DELTA_T_H -0.0065f
#define DELTA_T_H_2 0.0035f
#define R_S_AIR 8314.32f
#define M_0_AIR 28.9644f
#define P_CTE G_ACCEL*M_0_AIR/(R_S_AIR*DELTA_T_H)
#define P_CTE_2 G_ACCEL*M_0_AIR/(R_S_AIR*DELTA_T_H_2)

//RAF
#define MEAN_TURB 5.0f
#define MEAN_TURB_DEV 2.0f
#define TURB_LPF 0.01

/*!
 * \brief The atmosphereEngine class processess the athmospheric state and calculates the
 * alpha and beta increments due to instantaneous wind changes.
 */

class atmosphereEngine
{
public:
    atmosphereEngine();
    void updateAtmosphere(planeStatusData *state);
    void updateISADiff(float isadiff);
private:
    float temp11, dens11, pres11;
    float temp25, dens25, pres25;
//    void calculatePerturbations(planeStatusData *state);
//    QElapsedTimer perturbDuration;
//    QElapsedTimer perturbInterval;
    float ISADiff;


};

#endif // ATMOSPHEREENGINE_H
