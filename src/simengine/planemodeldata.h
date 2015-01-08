#ifndef PLANEMODELDATA_H
#define PLANEMODELDATA_H


#include "curvehandler.h"
#include "curve3dhandler.h"

/*!
 * \brief The massStruct struct contains information of a single mass element, containing mass, position and inertias
 */
struct massStruct
{
    float mass;
    float position[3];
    float inertia[3][3];
};
/*!
 * \brief The wheelStruct struct contains the information about a single wheel
 */
struct wheelStruct
{
    float position[3];
    float kspring;
    float kdamper;
};
/*!
 * \brief The fuelStruct struct contains the information about a single fuel tank
 */
struct fuelStruct
{
    float max_mass;
    float basecenter[3];
    float width;
    float height;
    float length;
};
/*!
 * \brief The planeModelData struct contains all the information about the plane.
 * , like aerodynamic derivatives, mass, dimensiones, engines, etc...
 */
struct planeModelData
{
    float Sw;
    float b;
    float cbar;
    float cmg;
    float cma;
    float stallalpha;
    float stallbeta;
    float alphamax;
    float betamax;
    curve3DHandler cLalpha;
    curve3DHandler cDalpha;
    curveHandler deltaCmFlaps;

    curve3DHandler cLalphaGround;
    curve3DHandler deltaCmGround;
    float groundEffectLimit;
    float groundEffectBase;

    float ail_def;
    float ele_def;
    float rud_def;

    float cdMissile;
    //LandingGear
    float cdGear;
    wheelStruct landingWheels[3];
    float cSide;
    float cRolling;
    float cBreak;
    float cBreakout;
    float speedLimit;
    float noseGearDef;

    //CONTROL
    float ki_alt;
    float kp_alt;
    float kd_alt;

    float ki_speed;
    float kd_speed;
    float kp_speed;

    float ki_pitch;
    float kp_pitch;
    float kd_pitch;
    float minmax_pitch;

    //Mass
    float mass_initial;
    float mass_fuel;

    //MOTOR
    float ce;
    float min_rpm;
    float max_rpm;
    float min_pitch;
    float max_pitch;
    float red_factor;
    float propDiam;
    float propellerIxx;
    float powerExtraction;
    float engPosition[3];
    float engVector[3];
    float controlKp;
    float controlKd;
    float takeOffPerformance;
    curveHandler rpmMaxP;
    curve3DHandler AltIsaCorrection;
    curve3DHandler cT;
    curve3DHandler cP;

    //DERIVATIVES
    float CLde;
    curveHandler CLalphadot;
    curveHandler CLq;
    float CDde;
    float CDq;
    float CDalphadot;
    float Cybeta;
    float Cydr;
    float Cyda;
    curve3DHandler Cyp;
    curve3DHandler Cyr;
    curveHandler Cybetadot;

    float Cm0;
    float Cmalpha;
    curveHandler Cmde;
    curveHandler Cmq;
    curveHandler Cmalphadot;
    curve3DHandler Clbeta;
    curveHandler Clbetadot;
    curve3DHandler Cldr;
    float Clda;
    float Clp;
    curve3DHandler Clr;

    curve3DHandler Cnbeta;
    curveHandler Cnbetadot;
    curve3DHandler Cndr;
    float Cnda;
    curve3DHandler Cnp;
    curve3DHandler Cnr;

    massStruct dryplane;
    massStruct miscelaneous;
    fuelStruct fuel_tank;
    massStruct gearIn;
    massStruct gearOut;
    massStruct missile[2];


};
#endif // PLANEMODELDATA_H
