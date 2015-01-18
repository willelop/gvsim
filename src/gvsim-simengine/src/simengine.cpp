#include "simengine.h"


simEngine::simEngine(QObject *parent) :
    QThread(parent)
{
    this->integration = RUNGEKUTTA4;
    this->sFactor = 1;
    this->simulationStep = DEFAULT_INTEGRATION_STEP;
    this->paused = true;
    this->stopped = true;
    this->modelLoaded = false;
    this->earth_radius = 6370000;
    this->current_iter =0 ;
    this->pendingfuelchange = false;
    this->fuelchange = 1.0f;
    this->exitonStall = true;
    this->exitonBetaMax = true;
    this->ISAdiff = 0;
    this->dof_lock[0] = false;
    this->dof_lock[1] = false;
    this->dof_lock[2] = false;
    this->startingAdamBasforth = true;
    this->adamStartCounter = 0;
    model = new planeModelEngine();
    atmosphere = new atmosphereEngine();
    autopilot = new autoPilot();

    this->resetSim();

}
/*!
 * \brief simEngine::run This is the main function. It starts the sim engine and provides a continous loop.
      Each loop it copies the command status and freezes it during the loop.
      Each loop a signal outputing the next calculated status is emited
 */
void simEngine::run()
{
    qDebug() << "Thread Started";

    QTime time;
    QTime freq_time;
    QElapsedTimer time2;

    int counter = 0;
    freq_time.start();
    emit logMessage("SimEngine Started",LOG::NORMAL);

    while(1)
    {
        /*
         * LOOP BEGINS
         */
        if(paused||stopped) {
            msleep(100);
            continue;
        }
        counter++;
        time.restart();
        time2.restart();
        /// 1: Update the inputs
        innerState.commanded =  this->commanddata;
        innerState.time.delta_t = this->simulationStep;
        if(this->pendingfuelchange) {
            pendingfuelchange = false;
            innerState.state.fuel_ratio = fuelchange;
        }
        autopilot->automaticOverride(&innerState,this->model);
        atmosphere->updateAtmosphere(&innerState);
        model->computeMassCenter(&innerState);
        model->computeInertias(&innerState);
        model->computeDerivatives(&innerState);
        model->computeEngineStatus(&innerState);
        model->computeGroundLevel(&innerState);

        if(abs(innerState.state.alpha_beta[0]) > model->data.alphamax && exitonStall)
        {
            this->finishSimulation(tr("Maximum Alpha Reached: ")+QString::number(model->data.alphamax*57.3f)+ tr(" degrees.\n If you want to disable this limitation, you can do it under <i>Settings -> Configuration -> Simulation -> Stop on Alpha Max</i>"));
            continue;
        }
        if(abs(innerState.state.alpha_beta[1]) > model->data.betamax && exitonBetaMax) {
            this->finishSimulation("Maximum Beta Reached = "+QString::number(model->data.betamax*57.3f)+ tr(" degrees.\n If you want to disable this limitation, you can do it under <i>Settings -> Configuration -> Simulation -> Stop on Beta Max</i>"));
            continue;
        }
        if(abs(innerState.state.position[0]) > 70/57.3){
            this->finishSimulation("Maximum Latitude Reached. Lat = 70 degrees.");
            continue;
        }

        if(this->integration == RUNGEKUTTA4){ //RUNGEKUTTA 4
            rungeKuttaStates[0] = innerState;
            rungeKuttaStates[1] = innerState;
            rungeKuttaStates[2] = innerState;
            rungeKuttaStates[3] = innerState;
            this->evaluateSituation(&rungeKuttaStates[0],&rungeKuttaStates[1],simulationStep/2.0); // 0 < an
            this->evaluateSituation(&rungeKuttaStates[1],&rungeKuttaStates[2],simulationStep/2.0); // 1 < bn
            this->evaluateSituation(&rungeKuttaStates[2],&rungeKuttaStates[3],simulationStep);   // 2 < cn
            this->evaluateSituation(&rungeKuttaStates[3],&rungeKuttaStates[3],simulationStep); // 3 < dn

            simUtils::RK4Integ3D(rungeKuttaStates[0].state.velocity_b_dot,rungeKuttaStates[1].state.velocity_b_dot,rungeKuttaStates[2].state.velocity_b_dot,rungeKuttaStates[3].state.velocity_b_dot,simulationStep,rungeKuttaStates[0].state.velocity_b);
            simUtils::RK4Integ3D(rungeKuttaStates[0].state.velocity_g,rungeKuttaStates[1].state.velocity_g,rungeKuttaStates[2].state.velocity_g,rungeKuttaStates[3].state.velocity_g,simulationStep,rungeKuttaStates[0].state.position);
            simUtils::RK4Integ3D(rungeKuttaStates[0].state.ang_velo_b_dot,rungeKuttaStates[1].state.ang_velo_b_dot,rungeKuttaStates[2].state.ang_velo_b_dot,rungeKuttaStates[3].state.ang_velo_b_dot,simulationStep,rungeKuttaStates[0].state.ang_velo_b);
            simUtils::RK4UpdateQuaternion(innerState,rungeKuttaStates[0].state.quat_der,rungeKuttaStates[1].state.quat_der,rungeKuttaStates[2].state.quat_der,rungeKuttaStates[3].state.quat_der,rungeKuttaStates[0].state.quaternion_b);
            simUtils::updateRotationMatrix(&rungeKuttaStates[0]);
            innerState = rungeKuttaStates[0];
        }else if(this->integration == ADAMBASH3){ // ADAM BASHFORTH 3
            // sol = sol_0 + delta(23/12 fn0 - 16/12 fn1 + 5/12 fn2)
            planeStatusData thrash;
            if(this->startingAdamBasforth == true)
            {
               this->evaluateSituation(&innerState,&thrash,simulationStep);
               adamBashforthStates[2-adamStartCounter] = innerState;
               if(adamStartCounter == 1)
               {
                   this->startingAdamBasforth = false;
                   adamStartCounter = 0;
               }
               adamStartCounter = 0;
            }else{
                adamBashforthStates[0] = innerState;
                this->evaluateSituation(&adamBashforthStates[0],&thrash,simulationStep);
                simUtils::ABInteg3D(adamBashforthStates[0].state.velocity_b_dot,adamBashforthStates[1].state.velocity_b_dot,adamBashforthStates[2].state.velocity_b_dot,innerState.time.delta_t,adamBashforthStates[0].state.velocity_b);
                simUtils::ABInteg3D(adamBashforthStates[0].state.velocity_g,adamBashforthStates[1].state.velocity_g,adamBashforthStates[2].state.velocity_g,innerState.time.delta_t,adamBashforthStates[0].state.position);
                simUtils::ABInteg3D(adamBashforthStates[0].state.ang_velo_b_dot,adamBashforthStates[1].state.ang_velo_b_dot,adamBashforthStates[2].state.ang_velo_b_dot,innerState.time.delta_t,adamBashforthStates[0].state.ang_velo_b);
                simUtils::ABUpdateQuaternion(innerState,adamBashforthStates[0].state.quat_der,adamBashforthStates[1].state.quat_der,adamBashforthStates[2].state.quat_der,adamBashforthStates[0].state.quaternion_b);
                simUtils::updateRotationMatrix(&adamBashforthStates[0]);
                adamBashforthStates[2] = adamBashforthStates[1];
                adamBashforthStates[1] = adamBashforthStates[0];
                innerState = adamBashforthStates[0];
            }
        }else{
            //EULER
            planeStatusData thrash;
            this->evaluateSituation(&innerState,&thrash,simulationStep);
        }
        // Check Latitude Longitude
        if(innerState.state.position[1] > PI){
            innerState.state.position[1] -= 2*PI;
            adamBashforthStates[0].state.position[1] -= 2*PI;
            adamBashforthStates[1].state.position[1] -= 2*PI;
            adamBashforthStates[2].state.position[1] -= 2*PI;
        }
        if(innerState.state.position[1] < -PI){
            innerState.state.position[1] += 2*PI;
            adamBashforthStates[0].state.position[1] += 2*PI;
            adamBashforthStates[1].state.position[1] += 2*PI;
            adamBashforthStates[2].state.position[1] += 2*PI;
        }

        //Store Current Time
        innerState.time.f_seconds += simulationStep;
        if(innerState.time.f_seconds >= 1.0f){
            innerState.time.f_seconds -=1.0f;
            innerState.time.seconds += 1;
        }
        //Timming thing

        if(freq_time.elapsed() > 1000){
            freq_time.restart();
            frequency = counter;
            counter = 0;
        }
        innerState.time.frequency = frequency;
        current_iter++;
        if(current_iter%RECORDING_FACTOR == 0){
            //emit the output status
            emit sendOutput(innerState);
        }
        // Case NO Max Speed
        if(sFactor > 0){
            int msecs_ela = time.elapsed();
            int diff_t = (int)((float)(simulationStep*1000 - msecs_ela)/(float)sFactor);
            if(diff_t > 0) {
                msleep(diff_t);
            }
        }
        innerState.state.reseting = false;
    }
}
/*!
 *  \brief this SLOT receives a new command status and saves it in the object
 */
void simEngine::updateCommand(commandStatusData _commandData){

    this->commanddata = _commandData;
    this->commanddata.elevator = _commandData.elevator;
}
/*!
 * \brief simEngine::pauseResumeSim pauses or resumes the simEngine
 */
void simEngine::pauseResumeSim()
{
    if(paused){
        resumeSim();
    }else{
        pauseSim();
    }

}
/*!
 * \brief simEngine::pauseSim pauses the simulation
 */
void simEngine::pauseSim()
{
    if(!stopped)
    {
        emit logMessage("Pausing Simulation",LOG::NORMAL);
        emit reportStatus(0);
        paused = true;
    }
}
/*!
 * \brief simEngine::startSim starts the simulation
 */
void simEngine::startSim()
{
    if(stopped && paused)
    {
        if(model->isLoaded())
        {
            if(this->innerState.state.position[2] < simUtils::getGroundAltitude(this->innerState.state.position[0],this->innerState.state.position[1]) +1)
            {
                this->innerState.state.position[2] = simUtils::getGroundAltitude(this->innerState.state.position[0],this->innerState.state.position[1]) +1;
            }
            if(stopped){
                this->startingAdamBasforth = true;
                this->adamStartCounter = 0;
            }
            emit logMessage("Starting Simulation",LOG::NORMAL);
            paused = false;
            stopped = false;
            emit reportStatus(1);
        }else{
            emit logMessage("No Model Data Loaded",LOG::ERRORMSG);
        }
    }

}
/*!
 * \brief simEngine::stopSim stops the simulation
 */
void simEngine::stopSim()
{
    this->resetSim();
    emit logMessage("Stopping Simulation",LOG::NORMAL);
    emit reportStatus(-1);
    paused = true;
    stopped = true;
}
/*!
 * \brief simEngine::resumeSim resumes the simulation if paused
 */
void simEngine::resumeSim()
{
    if(!stopped)
    {
        emit logMessage("Resuming Simulation",LOG::NORMAL);
        paused = false;
        emit reportStatus(1);
    }
}
/*!
 * \brief simEngine::reset resets the sim engine using the information in the planeStatusData param
 * \param _plane plane status data
 */
void simEngine::reset(planeStatusData _plane)
{
    this->stopSim();
    this->resetSim();
    this->innerState = _plane;
    this->innerState.state.reseting = true;
    this->resetSurfaces(&innerState);
    this->innerState.state.ground_altitude = simUtils::getGroundAltitude(innerState.state.position[0],innerState.state.position[1]);
    if(this->innerState.state.startOnAir && innerState.state.position[2] >= this->innerState.state.ground_altitude + 1){
        innerState.commanded.gearDown = false;
        autopilot->doTrim2(&innerState,model,atmosphere);
    }else{
        innerState.commanded.gearDown = true;
        innerState.state.position[2] = this->innerState.state.ground_altitude + 1;
    }
    emit newTrimmedCommand(innerState.commanded);
    this->startSim();
}
/*!
 * \brief simEngine::reset resets the simulation to the default position and start condition
 */
void simEngine::reset()
{
    this->stopSim();
    this->resetSim();
    this->resetSurfaces(&innerState);
    this->innerState.state.ground_altitude = simUtils::getGroundAltitude(innerState.state.position[0],innerState.state.position[1]) ;
    innerState.state.position[2] = this->innerState.state.ground_altitude + 1;
    emit newTrimmedCommand(innerState.commanded);
    this->startSim();
}

/*!
 * \brief simEngine::setSpeedFactor sets a new simulation speed
 * \param _sFactor speed -1: maximum, else the number indicates the x factor
 */
void simEngine::setSpeedFactor(int _sFactor)
{
    this->sFactor = _sFactor;
}
/*!
 * \brief simEngine::evaluateSituation evaluates acelerations and propagates using euler method
 * \param stateIn in data
 * \param stateOut out data (for RK4 method)
 * \param step integration step
 */
void simEngine::evaluateSituation(planeStatusData *stateIn, planeStatusData *stateOut, float step)
{
    this->computeDots(stateIn);
    stateOut->state.speed_air = stateIn->state.speed_air;
    stateOut->state.alpha_beta[0] = stateIn->state.alpha_beta[0];


    /// 9: integrate the body frame accelerations
    simUtils::integrateEuler(&stateIn->state.velocity_b[0],stateIn->state.velocity_b_dot[0],step);
    simUtils::integrateEuler(&stateIn->state.velocity_b[1],stateIn->state.velocity_b_dot[1],step);
    simUtils::integrateEuler(&stateIn->state.velocity_b[2],stateIn->state.velocity_b_dot[2],step);
    //STATE OUT
    simUtils::integrateEuler(&stateOut->state.velocity_b[0],stateIn->state.velocity_b_dot[0],step);
    simUtils::integrateEuler(&stateOut->state.velocity_b[1],stateIn->state.velocity_b_dot[1],step);
    simUtils::integrateEuler(&stateOut->state.velocity_b[2],stateIn->state.velocity_b_dot[2],step);
    //qDebug() << stateIn->velocity_b[0] << stateIn->velocity_b[1] << stateIn->velocity_b[2];
    //qDebug() << stateIn->velocity_b[2] << stateIn->alpha_beta[0] << stateIn->eulerangles[1] << stateIn->velocity_l[2];

    /// 10: compute Earth Speeds
    /// At this points there is something weird in the book, something about plus and minus winds.
    /// Im just changin this velocities to the local frame an then add windspeed.
    QVector3D earthVel = simUtils::body2local(QVector3D(stateIn->state.velocity_b[0],stateIn->state.velocity_b[1],stateIn->state.velocity_b[2]),stateIn);
    stateIn->state.velocity_l[0] = earthVel.x();
    stateIn->state.velocity_l[1] = earthVel.y();
    stateIn->state.velocity_l[2] = earthVel.z();

    simUtils::body2local(stateIn->state.velocity_b,stateIn,stateIn->state.velocity_l);


    //qDebug() << stateIn->velocity_l[0] << stateIn->velocity_l[1] << stateIn->velocity_l[2];
    /// 11: now, latitude and longitude

    stateIn->state.velocity_e[0] = (stateIn->state.velocity_l[0] + stateIn->atmosphere.wind_sp[0]);
    stateIn->state.velocity_e[1] = (stateIn->state.velocity_l[1] + stateIn->atmosphere.wind_sp[1]);
    stateIn->state.velocity_e[2] = (stateIn->state.velocity_l[2] + stateIn->atmosphere.wind_sp[2]);
//        qDebug() << stateIn->velocity_e[0] << stateIn->velocity_e[1] << stateIn->velocity_e[2];

    stateIn->state.velocity_g[0] = (stateIn->state.velocity_e[0])/(earth_radius + fabs(stateIn->state.position[2]));
    stateIn->state.velocity_g[1] = (stateIn->state.velocity_e[1])/((earth_radius + fabs(stateIn->state.position[2]))*cos(stateIn->state.position[0]));
    stateIn->state.velocity_g[2] = -(stateIn->state.velocity_e[2]);

    simUtils::integrateEuler(&stateIn->state.position[0],stateIn->state.velocity_g[0],step);
    simUtils::integrateEuler(&stateIn->state.position[1],stateIn->state.velocity_g[1],step);
    simUtils::integrateEuler(&stateIn->state.position[2],stateIn->state.velocity_g[2],step);

//    if(stateIn->state.position[2]< 0 ) stateIn->state.position[2] = 0;
    //STATE OUT
    simUtils::integrateEuler(&stateOut->state.position[0],stateIn->state.velocity_g[0],step);
    simUtils::integrateEuler(&stateOut->state.position[1],stateIn->state.velocity_g[1],step);
    simUtils::integrateEuler(&stateOut->state.position[2],stateIn->state.velocity_g[2],step);
//    if(stateOut->state.position[2]< 0 ) stateOut->state.position[2] = 0;

    //qDebug() << stateIn->ang_velo_b_dot[0] << stateIn->ang_velo_b_dot[1] << stateIn->ang_velo_b_dot[2];
    /// 15: integrate angular accelss
    simUtils::integrateEuler(&stateIn->state.ang_velo_b[0],stateIn->state.ang_velo_b_dot[0],step);
    simUtils::integrateEuler(&stateIn->state.ang_velo_b[1],stateIn->state.ang_velo_b_dot[1],step);
    simUtils::integrateEuler(&stateIn->state.ang_velo_b[2],stateIn->state.ang_velo_b_dot[2],step);
    //STATE OUT
    simUtils::integrateEuler(&stateOut->state.ang_velo_b[0],stateIn->state.ang_velo_b_dot[0],step);
    simUtils::integrateEuler(&stateOut->state.ang_velo_b[1],stateIn->state.ang_velo_b_dot[1],step);
    simUtils::integrateEuler(&stateOut->state.ang_velo_b[2],stateIn->state.ang_velo_b_dot[2],step);
    //DOF LOCK
    if(dof_lock[0]) stateIn->state.ang_velo_b[0] = 0.0;
    if(dof_lock[1]) stateIn->state.ang_velo_b[1] = 0.0;
    if(dof_lock[2]) stateIn->state.ang_velo_b[2] = 0.0;
    if(dof_lock[0]) stateOut->state.ang_velo_b[0] = 0.0;
    if(dof_lock[1]) stateOut->state.ang_velo_b[1] = 0.0;
    if(dof_lock[2]) stateOut->state.ang_velo_b[2] = 0.0;

    /*
     *
     * OPTIONAL CALCULATION
     *
    QVector3D angular_vel(stateIn->state.ang_velo_b[0],stateIn->state.ang_velo_b[1],stateIn->state.ang_velo_b[2]);
    // THis calculation is not necesary. Can be removed
    QVector3D local_ang_vel = this->body2local(angular_vel,stateIn);
    stateIn->state.ang_velo_l[0] = local_ang_vel.x();
    stateIn->state.ang_velo_l[1] = local_ang_vel.y();
    stateIn->state.ang_velo_l[2] = local_ang_vel.z();

    */
    //Establish inner values to the last values.
    /*!
     * In order to establish the orientation, we update the quaternion
     * with the angular velociy in the body frame
     */

    QVector3D angular_vel(stateIn->state.ang_velo_b[0],stateIn->state.ang_velo_b[1],stateIn->state.ang_velo_b[2]);

    float lambda = 1.0f - (stateIn->state.quaternion_b[0]*stateIn->state.quaternion_b[0] + stateIn->state.quaternion_b[1]*stateIn->state.quaternion_b[1] + stateIn->state.quaternion_b[2]*stateIn->state.quaternion_b[2] + stateIn->state.quaternion_b[3]*stateIn->state.quaternion_b[3]);
    float k = 1.0f / step; // Last member prevents numerical errors.
    stateIn->state.quat_der[0] = -0.5f*(stateIn->state.quaternion_b[1]*angular_vel.x() + stateIn->state.quaternion_b[2]*angular_vel.y() + stateIn->state.quaternion_b[3]*angular_vel.z());
    stateIn->state.quat_der[1] =  0.5f*(stateIn->state.quaternion_b[0]*angular_vel.x() + stateIn->state.quaternion_b[2]*angular_vel.z() - stateIn->state.quaternion_b[3]*angular_vel.y());
    stateIn->state.quat_der[2] =  0.5f*(stateIn->state.quaternion_b[0]*angular_vel.y() + stateIn->state.quaternion_b[3]*angular_vel.x() - stateIn->state.quaternion_b[1]*angular_vel.z());
    stateIn->state.quat_der[3] =  0.5f*(stateIn->state.quaternion_b[0]*angular_vel.z() + stateIn->state.quaternion_b[1]*angular_vel.y() - stateIn->state.quaternion_b[2]*angular_vel.x());
    stateIn->state.quat_der[0] += k*lambda*stateIn->state.quaternion_b[0];
    stateIn->state.quat_der[1] += k*lambda*stateIn->state.quaternion_b[1];
    stateIn->state.quat_der[2] += k*lambda*stateIn->state.quaternion_b[2];
    stateIn->state.quat_der[3] += k*lambda*stateIn->state.quaternion_b[3];
    simUtils::integrateEuler(&stateIn->state.quaternion_b[0],stateIn->state.quat_der[0],step);
    simUtils::integrateEuler(&stateIn->state.quaternion_b[1],stateIn->state.quat_der[1],step);
    simUtils::integrateEuler(&stateIn->state.quaternion_b[2],stateIn->state.quat_der[2],step);
    simUtils::integrateEuler(&stateIn->state.quaternion_b[3],stateIn->state.quat_der[3],step);
    //STATE OUT
    simUtils::integrateEuler(&stateOut->state.quaternion_b[0],stateIn->state.quat_der[0],step);
    simUtils::integrateEuler(&stateOut->state.quaternion_b[1],stateIn->state.quat_der[1],step);
    simUtils::integrateEuler(&stateOut->state.quaternion_b[2],stateIn->state.quat_der[2],step);
    simUtils::integrateEuler(&stateOut->state.quaternion_b[3],stateIn->state.quat_der[3],step);

    //qDebug() << "Quaternion" <<  (stateIn->quaternion_b[0]*stateIn->quaternion_b[0] + stateIn->quaternion_b[1]*stateIn->quaternion_b[1] + stateIn->quaternion_b[2]*stateIn->quaternion_b[2] + stateIn->quaternion_b[3]*stateIn->quaternion_b[3]);
    simUtils::updateRotationMatrix(stateIn);
    simUtils::updateRotationMatrix(stateOut);

}
/*!
 * \brief simEngine::updatePlaneModel stablish a new plane model
 * \param _model plane model
 */

void simEngine::updatePlaneModel(planeModelData _model)
{
    this->model->loadModel(_model);
    this->modelLoaded = true;
}
/*!
 * \brief simEngine::setDOFLock locks moments in body frame
 * \param l locks l moment
 * \param m locks m moment
 * \param n locks n moment
 */
void simEngine::setDOFLock(bool l, bool m, bool n)
{
    qDebug() << l << m <<n;
    this->dof_lock[0] = l;
    this->dof_lock[1] = m;
    this->dof_lock[2] = n;
//    emit logMessage("DOF Lock "+QString::number(l)+QString::number(m)+QString::number(n),LOG::NORMAL);
}
/*!
 * \brief simEngine::resetState resets the given state
 * \param state state to reset
 */

void simEngine::resetState(planeStatusData *state)
{

    state->commanded.ailerons = 0.0f;
    state->commanded.elevator = 0.0f;
    state->commanded.rudder = 0.0f;
    state->commanded.aileronTrim = 0.0f;
    state->commanded.elevatorTrim = 0.0f;
    state->commanded.rudderTrim = 0.0f;
    state->commanded.flaps = 0;
    state->commanded.throttle = 0.0f;
    state->commanded.propeller = 0.0f;
    state->commanded.missile[0] = true;
    state->commanded.missile[1] = true;
    state->commanded.gearDown = true;

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j<3; j++)
        {
            if(i==j)    state->state.rotationMatrix[i][j] = 1.0f;
            else        state->state.rotationMatrix[i][j] = 0.0f;
        }
    }
    state->atmosphere.p = 101325.0f;
    state->atmosphere.t = 288.0f;
    state->atmosphere.d = 1.225f;
    state->atmosphere.ISADiff = 0.0f;
    state->atmosphere.groundTemperature = 288.0f;
    state->atmosphere.wind_sp[0] = 0.0f;
    state->atmosphere.wind_sp[1] = 0.0f;
    state->atmosphere.wind_sp[2] = 0.0f;
    state->state.quaternion_b[0] = 1.0f*cos(45.0f/(RAD2DEG_*2));
    state->state.quaternion_b[1] = 0.0f;
    state->state.quaternion_b[2] = 0.0f;
    state->state.quaternion_b[3] = 1.0f*sin(45.0f/(RAD2DEG_*2));
    state->state.quat_der[0] = 0.0f;
    state->state.quat_der[1] = 0.0f;
    state->state.quat_der[2] = 0.0f;
    state->state.quat_der[3] = 0.0f;
    state->state.eulerangles[0] = 45.0f/RAD2DEG_;
    state->state.eulerangles[1] = 0.0f;
    state->state.eulerangles[2] = 0.0f;
    state->state.position[0] = 41.697290f/RAD2DEG_;
    state->state.position[1] = -4.864407/RAD2DEG_;
    state->state.position[2] = 2.0f;
    state->state.ground_altitude = 0.0f;
    state->state.speed_air = 0.0f;
    state->state.velocity_b[0] = 0.0f;
    state->state.velocity_b[1] = 0.0f;
    state->state.velocity_b[2] = 0.0f;
    state->state.velocity_b_dot[0] = 0.0f;
    state->state.velocity_b_dot[1] = 0.0f;
    state->state.velocity_b_dot[2] = 0.0f;
    state->state.velocity_e[0] = 0.0f;
    state->state.velocity_e[1] = 0.0f;
    state->state.velocity_e[2] = 0.0f;
    state->state.velocity_l[0] = 0.0f;
    state->state.velocity_l[1] = 0.0f;
    state->state.velocity_l[2] = 0.0f;
    state->state.ang_velo_b[0] = 0.0f;
    state->state.ang_velo_b[1] = 0.0f;
    state->state.ang_velo_b[2] = 0.0f;
    state->state.ang_velo_l[0] = 0.0f;
    state->state.ang_velo_l[1] = 0.0f;
    state->state.ang_velo_l[2] = 0.0f;

    state->state.ang_velo_b_dot[0] = 0.0f;
    state->state.ang_velo_b_dot[1] = 0.0f;
    state->state.ang_velo_b_dot[2] = 0.0f;
    state->state.alpha_beta[0] = 0.0f;
    state->state.alpha_beta[1] = 0.0f;
    state->state.alpha_beta_dot[0] = 0.0f;
    state->state.alpha_beta_dot[1] = 0.0f;
    state->state.forces_air_sb[0] = 0.0f;
    state->state.forces_air_sb[1] = 0.0f;
    state->state.forces_air_sb[2] = 0.0f;
    state->state.forces_b[0] = 0.0f;
    state->state.forces_b[1] = 0.0f;
    state->state.forces_b[2] = 0.0f;

    state->state.gear_forces_b[0] = 0.0f;
    state->state.gear_forces_b[1] = 0.0f;
    state->state.gear_forces_b[2] = 0.0f;
    state->state.ground_altitude = 0.0;

    state->state.engine_forces_b[0] = 0.0f;
    state->state.engine_forces_b[1] = 0.0f;
    state->state.engine_forces_b[2] = 0.0f;

    state->state.gear_moment_b[0] = 0.0f;
    state->state.gear_moment_b[1] = 0.0f;
    state->state.gear_moment_b[2] = 0.0f;
    state->state.engine_moment_b[0] = 0.0f;
    state->state.engine_moment_b[1] = 0.0f;
    state->state.engine_moment_b[2] = 0.0f;

    state->state.moment_air_sb[0] = 0.0f;
    state->state.moment_air_sb[1] = 0.0f;
    state->state.moment_air_sb[2] = 0.0f;
    state->state.moment_b[0] = 0.0f;
    state->state.moment_b[1] = 0.0f;
    state->state.moment_b[2] = 0.0f;
    state->time.seconds = 0;
    state->time.f_seconds = 0.0f;
    state->time.delta_t = DEFAULT_INTEGRATION_STEP;
    state->time.frequency = 1.0 / DEFAULT_INTEGRATION_STEP;


    state->derivatives.current_flaps = 0.0f;
    state->state.fuel_ratio = 1.0;
    state->derivatives.mass =  0;
    state->engine.engine_rpm = 0;
    state->engine.propeller_rpm = 0;
    state->engine.beta = 0;
    state->engine.thrust = 0;
    state->engine.shaft_power = 0;
    state->state.startOnAir = false;
    state->state.reseting = true;
}
/*!
 * \brief simEngine::resetSim resetes the whole simEngine
 */
void simEngine::resetSim()
{
    this->startingAdamBasforth = true;
    commanddata.ailerons = 0.0f;
    commanddata.elevator = 0.0f;
    commanddata.rudder = 0.0f;
    commanddata.aileronTrim = 0.0f;
    commanddata.elevatorTrim = 0.0f;
    commanddata.rudderTrim = 0.0f;
    commanddata.flaps = 0;
    commanddata.throttle = 0.0f;
    commanddata.propeller = 0.0f;
    commanddata.missile[0] = true;
    commanddata.missile[1] = true;
    commanddata.takeoffmode = false;
    commanddata.wheelBreak = false;
    commanddata.gearDown = true;
    this->resetState(&this->innerState);

    for(int i = 0; i < 3; i++)
    {
        this->resetState(&this->adamBashforthStates[i]);
    }

    innerState.state.fuel_ratio = this->fuelchange;
    this->autopilot->resetAP();

}
/*!
 * \brief simEngine::setNewFuelRatio updates the plane fuel ratio
 * \param ratio fuel ratio 0.0 - 1.0
 */
void simEngine::setNewFuelRatio(float ratio)
{
    qDebug() << "Received";
    this->pendingfuelchange = true;
    this->fuelchange = ratio;
}
/*!
 * \brief simEngine::finishSimulation ends the simulation logging a message
 * \param message message to log
 */
void simEngine::finishSimulation(QString message)
{
    this->resetSim();
    emit logMessage("Stopping Simulation:"+message,LOG::NORMAL);
    emit alertMessage(message);
    emit reportStatus(-1);
    paused = true;
    stopped = true;
}
/*!
 * \brief simEngine::resetSurfaces reset the surfaces deflections and trims
 * \param state state to reset the surfaces
 */
void simEngine::resetSurfaces(planeStatusData *state)
{
    state->commanded.ailerons = 0;
    state->commanded.elevator = 0;
    state->commanded.rudder = 0;
    state->commanded.aileronTrim = 0;
    state->commanded.elevatorTrim = 0;
    state->commanded.rudderTrim = 0;
    state->commanded.propeller = 0.0;
    state->commanded.throttle = 0.0;
    state->commanded.takeoffmode = false;
}
/*!
 * \brief simEngine::updateExitConditions enables or disables the stop conditions
 * \param onstall stop on maximum alpha condition
 * \param onbeta stop on maximum beta condition
 */
void simEngine::updateExitConditions(bool onstall,bool onbeta)
{
    this->exitonStall = onstall;
    this->exitonBetaMax = onbeta;
}
/*!
 * \brief simEngine::setISAtempDif update the atmosphere ISA differntial
 * \param dif ISA differential in Celsius
 */
void simEngine::setISAtempDif(int dif)
{
    this->atmosphere->updateISADiff(dif);
}

/*!
 * \brief simEngine::isStopped checks the stopped condition
 * \return  stopped condition
 */
bool simEngine::isStopped()
{
    return this->stopped;
}
/*!
 * \brief simEngine::setSimulationFreq set the integration step
 * \param _simFreq simulation frequecy (50-100Hz)
 */
void simEngine::setSimulationFreq(int _simFreq)
{
    this->simulationStep = 1.0f / (double)_simFreq;
    qDebug() << "Setting Simulation Step" << 1.0f / (double)_simFreq;
}
/*!
 * \brief simEngine::computeDots computes the angular and linea accelerations
 * \param state plane status
 */
void simEngine::computeDots(planeStatusData *state)
{
    /// 2: Angles of atack & sideslip
    state->state.speed_air = sqrt(state->state.velocity_b[0]*state->state.velocity_b[0]+
                                state->state.velocity_b[1]*state->state.velocity_b[1]+
                                state->state.velocity_b[2]*state->state.velocity_b[2]);
    float uwsqrt = sqrt(state->state.velocity_b[0]*state->state.velocity_b[0]+state->state.velocity_b[2]*state->state.velocity_b[2]);
    /// Check speed != 0 before calculating alpha & beta
    if(uwsqrt > 0.0f && state->state.speed_air > 2.0){
        state->state.alpha_beta[0] = atan2(state->state.velocity_b[2],state->state.velocity_b[0]);
        /// The book formula for alpha_dot and Beta_dot seem to crash, so i've changed it by linear aproximations.
        state->state.alpha_beta[1] = atan2(state->state.velocity_b[1],uwsqrt);
        state->state.alpha_beta_dot[0] = (state->state.velocity_b[0]*state->state.velocity_b_dot[2] - state->state.velocity_b[2]*state->state.velocity_b_dot[0]) / uwsqrt ;
        state->state.alpha_beta_dot[1] = (state->state.velocity_b_dot[1]*uwsqrt - state->state.velocity_b[1]*(state->state.velocity_b[0]*state->state.velocity_b_dot[0]+state->state.velocity_b[2]*state->state.velocity_b_dot[2]) )/ (uwsqrt*state->state.speed_air*state->state.speed_air);
    }else{
        state->state.alpha_beta[0] = 0.0f;
        state->state.alpha_beta_dot[0] = 0.0f;
        state->state.alpha_beta[1] = 0.0f;
        state->state.alpha_beta_dot[1] = 0.0f;
        state->state.alpha_beta_dot[0] = 0.0f;
    }

    /// 5: Stability Frame Forces
    model->computeForcesMoments(state);

    /// 8: Compute body frame accelerations
    state->state.velocity_b_dot[0] = state->state.forces_b[0]/state->derivatives.mass - state->state.ang_velo_b[1]*state->state.velocity_b[2] + state->state.ang_velo_b[2]*state->state.velocity_b[1];
    state->state.velocity_b_dot[1] = state->state.forces_b[1]/state->derivatives.mass - state->state.ang_velo_b[2]*state->state.velocity_b[0] + state->state.ang_velo_b[0]*state->state.velocity_b[2];
    state->state.velocity_b_dot[2] = state->state.forces_b[2]/state->derivatives.mass - state->state.ang_velo_b[0]*state->state.velocity_b[1] + state->state.ang_velo_b[1]*state->state.velocity_b[0];
    /// 14: body frame angular accels
    state->state.ang_velo_b_dot[0] = (1/state->derivatives.inertias[0][0])*(state->state.moment_b[0] + (state->derivatives.inertias[1][1] - state->derivatives.inertias[2][2])*state->state.ang_velo_b[1]*state->state.ang_velo_b[2] + state->derivatives.inertias[0][2]*(state->state.ang_velo_b_dot[2]*state->state.ang_velo_b[0] + state->state.ang_velo_b[0]*state->state.ang_velo_b[1]));
    state->state.ang_velo_b_dot[1] = (1/state->derivatives.inertias[1][1])*(state->state.moment_b[1] + (state->derivatives.inertias[2][2] - state->derivatives.inertias[0][0])*state->state.ang_velo_b[2]*state->state.ang_velo_b[0] + state->derivatives.inertias[0][2]*(state->state.ang_velo_b[2]*state->state.ang_velo_b[2] - state->state.ang_velo_b[0]*state->state.ang_velo_b[0]));
    state->state.ang_velo_b_dot[2] = (1/state->derivatives.inertias[2][2])*(state->state.moment_b[2] + (state->derivatives.inertias[0][0] - state->derivatives.inertias[1][1])*state->state.ang_velo_b[0]*state->state.ang_velo_b[1] + state->derivatives.inertias[0][2]*(state->state.ang_velo_b_dot[0] - state->state.ang_velo_b[1]*state->state.ang_velo_b[2]));

}
/*!
 * \brief simEngine::setIntegrationMethod sets the integration method
 * \param integ integration method
 */
void simEngine::setIntegrationMethod(INT_METHOD integ)
{
    qDebug() << "Setting Method" << integ;
    this->integration = integ;
    if(this->integration == ADAMBASH3){
        this->startingAdamBasforth = true;
        this->adamStartCounter = 0;
    }
}
/*!
 * \brief simEngine::setAutopilot sets the autopilot configuration
 * \param altitude commanded altitude
 * \param speed commanded airspeed
 * \param enabled_alt enables the altitude control
 * \param enabled_speed enables de speed control
 */
void simEngine::setAutopilot(float altitude, float speed, bool enabled_alt,bool enabled_speed)
{
    this->autopilot->setAutopilot(altitude,speed,enabled_alt,enabled_speed);
}
