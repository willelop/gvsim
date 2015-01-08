#ifndef PLANEMODELENGINE_H
#define PLANEMODELENGINE_H

#define FLAPS_LPF_S 5.0f
#define GEAR_LPF_S 7.0f
#define G_ACCEL 9.81f
#define _RAD2GRAD 57.2957795


#include "simengine/planemodeldata.h"
#include "simengine/planestatusdata.h"
#include "simutils.h"
#include <cmath>
#include <QVector3D>
#include <QDebug>

#include "../typedefs.h"

/*!
 * \brief The planeModelEngine class compute the plane states, such as aerodynamic forces and moments, gear forces and moments
 * mass center, inertias, etc...
 */

class planeModelEngine
{

public:
    planeModelEngine();
    planeModelData data;

    void loadModel(planeModelData plane);
    bool isLoaded();

    void computeMassCenter(planeStatusData *state);
    void computeDerivatives(planeStatusData *state);
    void computeForcesMoments(planeStatusData *state);
    void computeInertias(planeStatusData *state);
    void computeGearForces(planeStatusData *state);
    void computeEngineStatus(planeStatusData *state);
    void computeGroundLevel(planeStatusData *state);



private:
    void makeSteiner(float mass,float t_position[3], float cg[3],float inertia[3][3], float matrix[3][3]);
    bool modelLoaded;
    float gearState;
    vector<double> gear_ground_alt;
    //ENGINE
    float deltarpm;

};

#endif // PLANEMODELENGINE_H
