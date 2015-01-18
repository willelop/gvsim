#ifndef PLANESTATUSDATA_H
#define PLANESTATUSDATA_H
#include "src/commandstatus.h"
/*!
 * \brief The athmosphereData struct contains the athmospheric information
 */
struct athmosphereData {
    float p;            ///< Pressure
    float t;            ///< Temperature
    float d;            ///< Density
    float a;            ///< Sound Speed
    float ISADiff;
    float groundTemperature;
    float wind_sp[3];   ///< wind speed in m/s and local frame
    bool turb_enabled;
    bool turb_random;    ///< tries to make random turbulence
    float turb_intensity;///< raf intensity in m/s
    float turb_direction;///< raf direction in rads
    float turb_interval; ///< interval of the perturbation, 0 means single raf and occures one time
    float turb_duration;
    float delta_alpha;
    float delta_beta;
};
/*!
 * \brief The timeData structs contains senconds(int) and fraction of second(float)
 */
struct timeData {
    // All the coefficents
    int seconds;
    float f_seconds;
    int frequency;
    double delta_t;
};
/*!
 * \brief The derivativesData struct contains information about stability derivatives in the current situation
 */
struct derivativesData {
    // All the coefficents
    float CL, CLt,CD;
    float CLalphadot,CLde,CLq;
    float CDalphadot, CDde, CDq;
    float Cybeta,Cydr,Cybetadot,Cyp,Cyr,Cyda;
    float Cm0, Cmalpha, Cmde, Cmq,Cmalphadot;
    float Clbeta,Clbetadot, Cldr, Clda, Clp, Clr;
    float Cnbeta, Cnbetadot, Cndr, Cnda, Cnp, Cnr;
    float current_flaps;
    double mass;
    double mass_center[3];
    float inertias[3][3];
};
/*!
 * \brief The engineData struct contains information about engine status
 */
struct engineData {
    // All the coefficents
    float shaft_power;
    float thrust;
    float engine_rpm;
    float propeller_rpm;
    float beta;
    double fuel_consumption;
};
/*!
 * \brief The derivativesData contains information about stability derivatives in the current situation
 */
struct stateData {
    // All the coefficents
    double eulerangles[3];
    double quaternion_b[4];
    double quat_der[4];
    double rotationMatrix[3][3];
    double position[3]; /**< this is the position in latitude, longitude in radians and altitude (meters) */
    double velocity_b[3];  /**< this si the speed in the local horizon frame, in m/s  */
    double velocity_b_dot[3];
    double velocity_l[3];
    double velocity_e[3];
    double velocity_g[3];
    double ang_velo_b[3];
    double ang_velo_l[3];
    double ang_velo_b_dot[3];
    double alpha_beta[2];
    double alpha_beta_dot[2];
    double forces_air_sb[3]; ///< \brief Lift, Drag and Sideforce
    double forces_b[3];
    double engine_forces_b[3]; ///< \brief Engine forces in body frame
    double engine_moment_b[3];
    double gear_forces_b[3];
    double gear_moment_b[3];
    double ground_altitude;
    double moment_air_sb[3];
    double moment_b[3];
    double speed_air;
    double fuel_ratio;
    bool startOnAir;
    bool reseting;

};
/*!
 * \brief The planeStatusData struct contains the information of the plane status in a specific time.
 * It also contains the atmospheric information and commanded Status.
 */
struct planeStatusData {
    athmosphereData atmosphere;
    commandStatusData commanded;
    derivativesData derivatives;
    stateData state;
    timeData time;
    engineData engine;
} ;

#endif // PLANESTATUSDATA_H
