#include "simutils.h"

simUtils::simUtils()
{
}

QVector3D simUtils::body2local(QVector3D in, planeStatusData *status)
{
    QVector3D out;
    out.setX(in.x()*status->state.rotationMatrix[0][0] + in.y()*status->state.rotationMatrix[0][1] + in.z()*status->state.rotationMatrix[0][2]);
    out.setY(in.x()*status->state.rotationMatrix[1][0] + in.y()*status->state.rotationMatrix[1][1] + in.z()*status->state.rotationMatrix[1][2]);
    out.setZ(in.x()*status->state.rotationMatrix[2][0] + in.y()*status->state.rotationMatrix[2][1] + in.z()*status->state.rotationMatrix[2][2]);
    return out;
}

void simUtils::crossproduct(double v1[3], double v2[3],double out[3])
{
    out[0] = v1[1]*v2[2] - v1[2]*v2[1];
    out[1] = v1[2]*v2[0] - v1[0]*v2[2];
    out[2] = v1[0]*v2[1] - v1[1]*v2[0];
}
void simUtils::crossproduct(double v1[3], float v2[3],double out[3])
{
    out[0] = v1[1]*v2[2] - v1[2]*v2[1];
    out[1] = v1[2]*v2[0] - v1[0]*v2[2];
    out[2] = v1[0]*v2[1] - v1[1]*v2[0];
}
void simUtils::crossproduct(float v1[3], double v2[3],double out[3])
{
    out[0] = v1[1]*v2[2] - v1[2]*v2[1];
    out[1] = v1[2]*v2[0] - v1[0]*v2[2];
    out[2] = v1[0]*v2[1] - v1[1]*v2[0];
}


void simUtils::body2local(double  in[3], planeStatusData *status,double  out[3])
{
    out[0] = in[0]*status->state.rotationMatrix[0][0] + in[1]*status->state.rotationMatrix[0][1] + in[2]*status->state.rotationMatrix[0][2];
    out[1] = in[0]*status->state.rotationMatrix[1][0] + in[1]*status->state.rotationMatrix[1][1] + in[2]*status->state.rotationMatrix[1][2];
    out[2] = in[0]*status->state.rotationMatrix[2][0] + in[1]*status->state.rotationMatrix[2][1] + in[2]*status->state.rotationMatrix[2][2];
}
void simUtils::body2local(float  in[3], planeStatusData *status,double  out[3])
{
    out[0] = in[0]*status->state.rotationMatrix[0][0] + in[1]*status->state.rotationMatrix[0][1] + in[2]*status->state.rotationMatrix[0][2];
    out[1] = in[0]*status->state.rotationMatrix[1][0] + in[1]*status->state.rotationMatrix[1][1] + in[2]*status->state.rotationMatrix[1][2];
    out[2] = in[0]*status->state.rotationMatrix[2][0] + in[1]*status->state.rotationMatrix[2][1] + in[2]*status->state.rotationMatrix[2][2];
}
void simUtils::local2body(double  in[3], planeStatusData *status,double  out[3])
{
    out[0] = in[0]*status->state.rotationMatrix[0][0] + in[1]*status->state.rotationMatrix[1][0] + in[2]*status->state.rotationMatrix[2][0];
    out[1] = in[0]*status->state.rotationMatrix[0][1] + in[1]*status->state.rotationMatrix[1][1] + in[2]*status->state.rotationMatrix[2][1];
    out[2] = in[0]*status->state.rotationMatrix[0][2] + in[1]*status->state.rotationMatrix[1][2] + in[2]*status->state.rotationMatrix[2][2];
}
void simUtils::local2body(float  in[3], planeStatusData *status,double  out[3])
{
    out[0] = in[0]*status->state.rotationMatrix[0][0] + in[1]*status->state.rotationMatrix[1][0] + in[2]*status->state.rotationMatrix[2][0];
    out[1] = in[0]*status->state.rotationMatrix[0][1] + in[1]*status->state.rotationMatrix[1][1] + in[2]*status->state.rotationMatrix[2][1];
    out[2] = in[0]*status->state.rotationMatrix[0][2] + in[1]*status->state.rotationMatrix[1][2] + in[2]*status->state.rotationMatrix[2][2];
}

void simUtils::makeSteiner(float mass,float t_position[3], float cg[3],float inertia[3][3], float matrix[3][3])
{
    float t_pos[3];
    float mod2_cg = 0;
    float mod2_t = 0;
    for(int i = 0; i< 3; i++)
    {
        t_pos[i] = t_position[i]-cg[i];
        mod2_cg += cg[i]*cg[i];
        mod2_t += t_pos[i]*t_pos[i];
    }

    for(int i = 0; i< 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            matrix[i][j] = inertia[i][j];
            if(i == j) matrix[i][j] += mass*(mod2_cg-mod2_t);
            matrix[i][j] += -mass*(cg[i]*cg[j]-t_pos[i]*t_pos[j]);
        }
    }

}

/*!
 *  \brief Integrates values using euler method.
 */
void simUtils::integrateEuler(double *x, double dx, double h)
{

    *x = *x + dx*h;
}
void simUtils::integrateEuler(double xin,double *xout, double dx, double h)
{

    *xout = xin + dx*h;
}
void simUtils::RK4SumVec3D(double vec1[], double vec2[], double vec3[], double vec4[], double result[])
{
    for(int i = 0; i < 3; i++)
    {
        result[i] = (vec1[i]+2*vec2[i]+ 2*vec3[i]+vec4[i])/6;
    }
}
void simUtils::RK4SumVec4D(double vec1[], double vec2[], double vec3[], double vec4[], double result[])
{
    for(int i = 0; i < 4; i++)
    {
        result[i] = (vec1[i]+2*vec2[i]+ 2*vec3[i]+vec4[i])/6;
    }
}
void simUtils::RK4Integ3D(double vec1[], double vec2[], double vec3[], double vec4[],double step, double result[])
{
    double deltaV[3];
    simUtils::RK4SumVec3D(vec1,vec2,vec3,vec4,deltaV);
    for(int i = 0; i < 3; i++)
    {
        result[i] += step*deltaV[i];
    }
}
void simUtils::RK4Integ4D(double vec1[], double vec2[], double vec3[], double vec4[],double step, double result[])
{
    double deltaV[4];
    simUtils::RK4SumVec4D(vec1,vec2,vec3,vec4,deltaV);
    for(int i = 0; i < 4; i++)
    {
        result[i] += step*deltaV[i];
    }
}

void simUtils::initializeCuaternion(planeStatusData *state, QVector3D eulerangles)
{
    state->state.quaternion_b[0] = cos(0.5f*eulerangles.x())*cos(0.5f*eulerangles.y())*cos(0.5f*eulerangles.z())+sin(0.5f*eulerangles.x())*sin(0.5f*eulerangles.y())*sin(0.5f*eulerangles.z());
    state->state.quaternion_b[1] = cos(0.5f*eulerangles.x())*cos(0.5f*eulerangles.y())*sin(0.5f*eulerangles.z())-sin(0.5f*eulerangles.x())*sin(0.5f*eulerangles.y())*cos(0.5f*eulerangles.z());
    state->state.quaternion_b[2] = cos(0.5f*eulerangles.x())*sin(0.5f*eulerangles.y())*cos(0.5f*eulerangles.z())+sin(0.5f*eulerangles.x())*cos(0.5f*eulerangles.y())*sin(0.5f*eulerangles.z());
    state->state.quaternion_b[3] = sin(0.5f*eulerangles.x())*cos(0.5f*eulerangles.y())*cos(0.5f*eulerangles.z())-cos(0.5f*eulerangles.x())*sin(0.5f*eulerangles.y())*sin(0.5f*eulerangles.z());
}
void simUtils::updateRotationMatrix(planeStatusData *state)
{
    state->state.rotationMatrix[0][0] = state->state.quaternion_b[0]*state->state.quaternion_b[0] + state->state.quaternion_b[1]*state->state.quaternion_b[1] - state->state.quaternion_b[2]*state->state.quaternion_b[2] - state->state.quaternion_b[3]*state->state.quaternion_b[3];
    state->state.rotationMatrix[1][1] = state->state.quaternion_b[0]*state->state.quaternion_b[0] - state->state.quaternion_b[1]*state->state.quaternion_b[1] + state->state.quaternion_b[2]*state->state.quaternion_b[2] - state->state.quaternion_b[3]*state->state.quaternion_b[3];
    state->state.rotationMatrix[2][2] = state->state.quaternion_b[0]*state->state.quaternion_b[0] - state->state.quaternion_b[1]*state->state.quaternion_b[1] - state->state.quaternion_b[2]*state->state.quaternion_b[2] + state->state.quaternion_b[3]*state->state.quaternion_b[3];
    state->state.rotationMatrix[0][1] = 2.0f*(state->state.quaternion_b[1]*state->state.quaternion_b[2] - state->state.quaternion_b[0]*state->state.quaternion_b[3]);
    state->state.rotationMatrix[0][2] = 2.0f*(state->state.quaternion_b[0]*state->state.quaternion_b[2] - state->state.quaternion_b[1]*state->state.quaternion_b[3]);
    state->state.rotationMatrix[1][0] = 2.0f*(state->state.quaternion_b[1]*state->state.quaternion_b[2] + state->state.quaternion_b[0]*state->state.quaternion_b[3]);
    state->state.rotationMatrix[1][2] = 2.0f*(state->state.quaternion_b[2]*state->state.quaternion_b[3] - state->state.quaternion_b[0]*state->state.quaternion_b[1]);
    state->state.rotationMatrix[2][0] = 2.0f*(state->state.quaternion_b[1]*state->state.quaternion_b[3] - state->state.quaternion_b[0]*state->state.quaternion_b[2]);
    state->state.rotationMatrix[2][1] = 2.0f*(state->state.quaternion_b[2]*state->state.quaternion_b[3] + state->state.quaternion_b[0]*state->state.quaternion_b[1]);
    //Nex
    // NUmerical Problems
    state->state.eulerangles[0] = atan2(state->state.rotationMatrix[1][0],state->state.rotationMatrix[0][0]);
    state->state.eulerangles[1] = asin(max(-1.0f,min(1.0f,(float)(-state->state.rotationMatrix[2][0])))); //Prevent numerical errors.
    state->state.eulerangles[2] = atan2(state->state.rotationMatrix[2][1],state->state.rotationMatrix[2][2]);
}
void simUtils::RK4UpdateQuaternion(planeStatusData state, double vec1[], double vec2[], double vec3[], double vec4[], double result[])
{
    double   k = 1.0f / state.time.delta_t;
    double lambda = 1.0f - (state.state.quaternion_b[0]*state.state.quaternion_b[0] + state.state.quaternion_b[1]*state.state.quaternion_b[1] + state.state.quaternion_b[2]*state.state.quaternion_b[2] + state.state.quaternion_b[3]*state.state.quaternion_b[3]);
    double quat_der[4];
    simUtils::RK4SumVec4D(vec1,vec2,vec3,vec4,quat_der);
    quat_der[0] += k*lambda*state.state.quaternion_b[0];
    quat_der[1] += k*lambda*state.state.quaternion_b[1];
    quat_der[2] += k*lambda*state.state.quaternion_b[2];
    quat_der[3] += k*lambda*state.state.quaternion_b[3];

    result[0] = state.state.quaternion_b[0] + state.time.delta_t*quat_der[0];
    result[1] = state.state.quaternion_b[1] + state.time.delta_t*quat_der[1];
    result[2] = state.state.quaternion_b[2] + state.time.delta_t*quat_der[2];
    result[3] = state.state.quaternion_b[3] + state.time.delta_t*quat_der[3];
}
void simUtils::ABInteg3D(double f1[], double f2[], double f3[], double step, double result[])
{
    for(int i = 0; i<3; i++)
    {
        result[i] += (step/12.0f)*(23*f1[i]-16*f2[i]+5*f3[i]);
    }
}
void simUtils::ABSum4D(double f1[], double f2[], double f3[], double result[])
{
    for(int i = 0; i<4; i++)
    {
        result[i] = (1/12)*(23*f1[i]-16*f2[i]+5*f3[i]);
    }
}
void simUtils::ABUpdateQuaternion(planeStatusData state, double vec1[], double vec2[], double vec3[], double result[])
{
    double   k = 1.0f / state.time.delta_t;
    double lambda = 1.0f - (state.state.quaternion_b[0]*state.state.quaternion_b[0] + state.state.quaternion_b[1]*state.state.quaternion_b[1] + state.state.quaternion_b[2]*state.state.quaternion_b[2] + state.state.quaternion_b[3]*state.state.quaternion_b[3]);
    double quat_der[4];
    simUtils::ABSum4D(vec1,vec2,vec3,quat_der);
    quat_der[0] += k*lambda*state.state.quaternion_b[0];
    quat_der[1] += k*lambda*state.state.quaternion_b[1];
    quat_der[2] += k*lambda*state.state.quaternion_b[2];
    quat_der[3] += k*lambda*state.state.quaternion_b[3];

    result[0] = state.state.quaternion_b[0] + state.time.delta_t*quat_der[0];
    result[1] = state.state.quaternion_b[1] + state.time.delta_t*quat_der[1];
    result[2] = state.state.quaternion_b[2] + state.time.delta_t*quat_der[2];
    result[3] = state.state.quaternion_b[3] + state.time.delta_t*quat_der[3];
}

double simUtils::getGroundAltitude(double lat, double lon)
{
    return 0.0;
}


