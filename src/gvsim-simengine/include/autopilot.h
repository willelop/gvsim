#ifndef AUTOPILOT_H
#define AUTOPILOT_H

#include "simengine/planestatusdata.h"
#include "simengine/planemodeldata.h"
#include "atmosphereengine.h"
#include "planemodelengine.h"
#include "simutils.h"
#define EPSILON 0.001
/*!
 * \brief The autoPilot class handles some calculations needed to start and maintain
 *a controlled flight to be studied
 */
class autoPilot
{

public:
    autoPilot();
    void doTrim(planeStatusData *state,planeModelEngine *model,atmosphereEngine *atmosphere);
    void doTrim2(planeStatusData *state,planeModelEngine *model,atmosphereEngine *atmosphere);
    void automaticOverride(planeStatusData *state,planeModelEngine *model);
    void setAutopilot(float altitude, float speed, bool enabled_alt,bool enabled_speed);
    void resetAP();
private:
    float commanded_altitude;
    float commanded_speed;
    bool speedEnabled;
    bool altEnabled;
    float vel_error;
    float elev_error;
    float alt_error;
    float aThrottle;
};

#endif // AUTOPILOT_H
