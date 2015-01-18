#ifndef SIMENGINE_H
#define SIMENGINE_H

#include <QApplication>

#include <QThread>
#include <QDebug>
#include <QString>
#include <QDateTime>
#include <QVector3D>
#include <QVector4D>
#include <QElapsedTimer>
#include "../commandstatusdata.h"
#include "simengine/planestatusdata.h"
#include "simengine/planemodeldata.h"
#include "../typedefs.h"
#include "math.h"
#include "stdlib.h"
#include "time.h"
#include "planemodelengine.h"
#include "atmosphereengine.h"
#include "simutils.h"
#include "autopilot.h"
#include "../typedefs.h"


#define EARTH_RADIUS_ 6356766.0f
#define G_ACCEL 9.81f
#define EARTH_OMEGA 0.00007292
#define PI 3.14159265359
#define FLAPS_LPF 0.004f




/*!
 *  \brief Class for the simulation engine, this class is the main wrapper for the simulation, and inside it other elements,
 * such as atmosphericEngine or planemodelEngine, are instantiated.
 *  \author Guillermo López Matos
 */
class simEngine : public QThread
{
    Q_OBJECT

public:
    enum INT_METHOD {
        RUNGEKUTTA4,
        ADAMBASH3,
        EULER
    };
    explicit simEngine(QObject *parent = 0);
    bool isStopped();
    void run();
private:
    INT_METHOD integration;
    bool pendingfuelchange;
    bool exitonStall;
    bool exitonBetaMax;
    float fuelchange;
    int current_iter;
    int ISAdiff;
    bool dof_lock[3];
    bool paused;
    bool stopped;
    bool startingAdamBasforth;
    int adamStartCounter;
    bool modelLoaded;
    int sFactor;
    double simulationStep;
    float frequency;
    planeStatusData innerState;
    planeStatusData adamBashforthStates[3];
    planeStatusData rungeKuttaStates[4];
    commandStatusData commanddata;
    planeModelEngine *model;
    atmosphereEngine *atmosphere;
    autoPilot *autopilot;
    float earth_radius;

    void evaluateSituation(planeStatusData *stateIn, planeStatusData *stateOut, float step);
    void computeDerivatives(planeStatusData *state);
    void updateAtmosphere();
    void computeCoefficients();

    void computeMassCenter(planeStatusData *state,planeModelData *model);
    void computeForcesMoments(planeStatusData *state,planeModelData *model);
    void computeInertias(planeStatusData *state,planeModelData *model);
    void resetSurfaces(planeStatusData *state);
    void computeDots(planeStatusData *state);

signals:
    void sendOutput(planeStatusData); ///< \brief This signal emits the calculated state
    void logMessage(QString,LOG::TYPE);         ///< \brief Signal to the logging system
    void alertMessage(QString);         ///< \brief Signal to the alert system
    void newTrimmedCommand(commandStatusData);
    void reportStatus(int status);
public slots:
    void updateCommand(commandStatusData _commandData);
    void pauseResumeSim();
    void pauseSim();
    void stopSim();
    void startSim();
    void resumeSim();
    void resetSim();
    void setISAtempDif(int);
    void updateExitConditions(bool onstall,bool onbeta);
    static void resetState(planeStatusData *state);
    void finishSimulation(QString message);
    void updatePlaneModel(planeModelData _model);
    void reset(planeStatusData _plane);
    void reset();
    void setIntegrationMethod(simEngine::INT_METHOD integ);
    void setSpeedFactor(int _sFactor);
    void setSimulationFreq(int _simFreq);
    void setDOFLock(bool l,bool m,bool n);
    void setNewFuelRatio(float ratio);
    void setAutopilot(float altitude, float speed, bool enabled_alt, bool enabled_speed);
    
};

#endif // SIMENGINE_H
