#ifndef COMANDSTATUSDATA_H
#define COMANDSTATUSDATA_H
/*!
 * \brief The commandStatusData struct contains the command information with float values between 0.0f and 1.0f
 */
struct commandStatusData {
    float ailerons;
    float elevator;
    float rudder;
    float throttle;
    float propeller;
    bool gearDown;///< \brief boolean to enable gear down
    bool wheelBreak; ///< \brief boolean to enable gear break
    bool takeoffmode; ///< \brief boolean to enable the engine takeoff mode
    int flaps; ///< \brief integer indicating the flaps status, 0 - 0 degrees, 1 - 15 degrees, 2 -  30 degrees
    float aileronTrim;
    float elevatorTrim;
    float rudderTrim;
    bool missile[2]; ///< \brief booleans to set the missile status (present or launched)
} ;

#endif // COMANDSTATUSDATA_H
