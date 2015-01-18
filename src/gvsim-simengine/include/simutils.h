#ifndef SIMUTILS_H
#define SIMUTILS_H
#include <stdlib.h>
#include <math.h>
//#include <osgEarth/Map>
//#include <osgEarth/ElevationQuery>
#include "planestatusdata.h"
#include "planemodeldata.h"
#include <QVector3D>

#define _RAD2GRAD 57.2957795
/*!
 * \brief The simUtils class provide some static methods used by other members of the sim engine.
 */
class simUtils
{
public:
    simUtils();
    static QVector3D body2local(QVector3D in,planeStatusData *status);
    static QVector3D local2body(QVector3D in,planeStatusData *status);
    static void  body2local(double in[3],planeStatusData *status,double out[3]);
    static void  body2local(float in[3],planeStatusData *status,double out[3]);
    static void local2body(double in[3],planeStatusData *status,double out[3]);
    static void local2body(float in[3],planeStatusData *status,double out[3]);

    static void makeSteiner(float mass,float t_position[3], float cg[3],float inertia[3][3], float matrix[3][3]);
    static void crossproduct(double v1[3], double v2[3],double  out[3]);
    static void crossproduct(float v1[3], double v2[3],double  out[3]);
    static void crossproduct(double v1[3], float v2[3],double  out[3]);

    static void integrateEuler(double *x,double dx, double h);
    static void integrateEuler(double xin, double *xout,double dx, double h);
    static void RK4Integ3D(double vec1[3],double vec2[3], double vec3[3],double vec4[3],double step, double result[3]);
    static void RK4Integ4D(double vec1[4],double vec2[4], double vec3[4],double vec4[4],double step, double result[4]);
    static void RK4SumVec3D(double vec1[3],double vec2[3], double vec3[3],double vec4[3], double result[3]);
    static void RK4SumVec4D(double vec1[4],double vec2[4], double vec3[4],double vec4[4], double result[4]);
    static void RK4UpdateQuaternion(planeStatusData state,double vec1[4],double vec2[4], double vec3[4],double vec4[4], double result[4]);
    static void ABInteg3D(double f1[3], double f2[3],double f3[3],double step, double result[3]);
    static void ABSum4D(double f1[4], double f2[4],double f3[4], double result[4]);
    static void ABUpdateQuaternion(planeStatusData state,double vec1[4],double vec2[4], double vec3[4], double result[4]);



    static void initializeCuaternion(planeStatusData *state,QVector3D eulerangles);
    static void updateRotationMatrix(planeStatusData *state);
    static double getGroundAltitude(double lat, double lon);

};

#endif // SIMUTILS_H
