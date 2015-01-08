#include "planemodelengine.h"

#define PI 3.14159
#define KMGRAD 0.001 / (111.11 * 57.3)

using namespace std;

planeModelEngine::planeModelEngine()
{
    this->modelLoaded = false;
    this->deltarpm = 0;
    this->gear_ground_alt.clear();
    this->gearState = 0.0;
}
/*!
 * \brief planeModelEngine::loadModel loads a plane model in memory
 * \param plane planemodel
 */
void planeModelEngine::loadModel(planeModelData plane)
{
    this->data = plane;
    this->modelLoaded = true;
}
/*!
 * \brief planeModelEngine::isLoaded returns the loaded status of the model
 * \return loaded status
 */
bool planeModelEngine::isLoaded()
{
    return this->modelLoaded;
}
/*!
 * \brief planeModelEngine::computeMassCenter calculates the mass center of the vehicle
 * \param state plane state
 */
void planeModelEngine::computeMassCenter(planeStatusData *state)
{

    float totalMass = this->data.dryplane.mass;
    if(state->commanded.missile[0]) totalMass += this->data.missile[0].mass;
    if(state->commanded.missile[1]) totalMass += this->data.missile[1].mass;
    totalMass += this->data.miscelaneous.mass;
    totalMass += this->data.gearIn.mass;
    totalMass += this->data.fuel_tank.max_mass*state->state.fuel_ratio;

    float fuelcg[3];
    fuelcg[0] = 0.0f;
    fuelcg[1] = 0.0f;
    fuelcg[2] = -0.5*this->data.fuel_tank.height*state->state.fuel_ratio;

    if(state->commanded.gearDown){
        gearState += (state->time.delta_t / GEAR_LPF_S)*(1.0-gearState);
    }else{
        gearState+= (state->time.delta_t / GEAR_LPF_S)*(0-gearState);
    }
    for(int i = 0; i < 3; i++){
        state->derivatives.mass_center[i] = this->data.dryplane.mass*this->data.dryplane.position[i];
        state->derivatives.mass_center[i]+= this->data.miscelaneous.mass*this->data.miscelaneous.position[i];
        float gearPos = this->data.gearOut.position[i]*gearState + (1.0-gearState)*this->data.gearIn.position[i];
        state->derivatives.mass_center[i]+= this->data.gearIn.mass*gearPos;
        if(state->commanded.missile[0]) state->derivatives.mass_center[i]+= this->data.missile[0].mass*this->data.missile[0].position[i];
        if(state->commanded.missile[1]) state->derivatives.mass_center[i]+= this->data.missile[1].mass*this->data.missile[1].position[i];
        state->derivatives.mass_center[i]+= this->data.fuel_tank.max_mass*state->state.fuel_ratio*(this->data.fuel_tank.basecenter[i]+fuelcg[i]);
        state->derivatives.mass_center[i] /= totalMass;
    }
    state->derivatives.mass = totalMass;
}
/*!
 * \brief planeModelEngine::computeDerivatives calculates the aerodynamic derivatives and coefficentes
 * \param state plane state
 */
void planeModelEngine::computeDerivatives(planeStatusData *state)
{
    float alpha = 0;
    if(state->state.speed_air > 2.0)
        alpha = atan2(state->state.velocity_b[2],state->state.velocity_b[0]);
    bool ok, ok1;
    bool oktest;
    state->derivatives.current_flaps = (state->time.delta_t/FLAPS_LPF_S)*state->commanded.flaps*15.0f  +(1-(state->time.delta_t/FLAPS_LPF_S))*state->derivatives.current_flaps;

    state->derivatives.CL = this->data.cLalpha.getValue(alpha,state->derivatives.current_flaps,&oktest);
    state->derivatives.CLde = this->data.CLde;
    state->derivatives.CLalphadot = this->data.CLalphadot.getValue(state->derivatives.mass_center[0],&ok);
    state->derivatives.CLq = this->data.CLq.getValue(state->derivatives.mass_center[0],&ok);

    state->derivatives.CD = this->data.cDalpha.getValue(alpha,state->derivatives.current_flaps,&ok);
    //MISSILEDRAG
    if(state->commanded.missile[0]){
        state->derivatives.CD += data.cdMissile;
    }
    if(state->commanded.missile[1]){
        state->derivatives.CD += data.cdMissile;
    }
    //GEAR DRAG
    static float gearDrag = 0.0;
    if(state->commanded.gearDown){
        gearDrag += (state->time.delta_t / GEAR_LPF_S)*(this->data.cdGear-gearDrag);
    }else{
        gearDrag += (state->time.delta_t / GEAR_LPF_S)*(0-gearDrag);
    }
    state->derivatives.CD += gearDrag;
    state->derivatives.CDde = this->data.CDde;
    state->derivatives.CDalphadot = this->data.CDalphadot;
    state->derivatives.CDq = this->data.CDq;

    state->derivatives.CLt = 0.0;
    state->derivatives.Cybeta = this->data.Cybeta;
    state->derivatives.Cybetadot = this->data.Cybetadot.getValue(alpha,&ok1);
    state->derivatives.Cydr = this->data.Cydr;
    state->derivatives.Cyda = this->data.Cyda;
    state->derivatives.Cyp = this->data.Cyp.getValue(alpha,state->derivatives.mass_center[0],&ok1);
    state->derivatives.Cyr = this->data.Cyr.getValue(alpha,state->derivatives.mass_center[0],&ok1);
    state->derivatives.Cm0 = this->data.Cm0 + this->data.deltaCmFlaps.getValue(state->derivatives.current_flaps,&ok1);
    state->derivatives.Cmalpha = this->data.Cmalpha;
    state->derivatives.Cmde = this->data.Cmde.getValue(state->derivatives.mass_center[0],&ok);
    state->derivatives.Cmq = this->data.Cmq.getValue(state->derivatives.mass_center[0],&ok);
    state->derivatives.Cmalphadot =this->data.Cmalphadot.getValue(state->derivatives.mass_center[0],&ok);

    state->derivatives.Clbeta = this->data.Clbeta.getValue(alpha,state->derivatives.mass_center[0],&ok1);
    state->derivatives.Clbetadot = this->data.Clbetadot.getValue(alpha,&ok1);
    state->derivatives.Cldr = this->data.Cldr.getValue(alpha,state->derivatives.mass_center[0],&ok1);
    state->derivatives.Clda = this->data.Clda;
    state->derivatives.Clp = this->data.Clp;
    state->derivatives.Clr = this->data.Clr.getValue(alpha,state->derivatives.mass_center[0],&ok1);

    state->derivatives.Cnbeta = this->data.Cnbeta.getValue(alpha,state->derivatives.mass_center[0],&ok1);
    state->derivatives.Cnbetadot = this->data.Cnbetadot.getValue(alpha,&ok1);
    state->derivatives.Cndr = this->data.Cndr.getValue(alpha,state->derivatives.mass_center[0],&ok1);
    state->derivatives.Cnda = this->data.Cnda;
    state->derivatives.Cnp = this->data.Cnp.getValue(alpha,state->derivatives.mass_center[0],&ok);
    state->derivatives.Cnr = this->data.Cnr.getValue(alpha,state->derivatives.mass_center[0],&ok1);

    //GRound effect
    double ground_influence = data.groundEffectLimit - std::max(0.0,state->state.position[2] - state->state.ground_altitude);
    ground_influence = std::max(0.0,ground_influence);
    ground_influence = std::min(data.groundEffectLimit-data.groundEffectBase,(float)ground_influence);
    ground_influence /= (data.groundEffectLimit-data.groundEffectBase);

    state->derivatives.Cm0 += ground_influence * data.deltaCmGround.getValue(alpha,state->derivatives.current_flaps,&ok);
    state->derivatives.CL += ground_influence*(data.cLalphaGround.getValue(alpha,state->derivatives.current_flaps,&ok)-state->derivatives.CL);


}
/*!
 * \brief planeModelEngine::computeForcesMoments calculates all the forces and moments actin in the plane
 * \param state plane state
 */
void planeModelEngine::computeForcesMoments(planeStatusData *state)
{
    //qDebug() << state->derivatives.Cyp << state->derivatives.Cyr;
    /// 5: Stability Frame Forces
    float qS = 0.5*state->atmosphere.d*state->state.speed_air*state->state.speed_air*data.Sw;
    float qS_V = 0.25f*state->atmosphere.d*state->state.speed_air*data.Sw;
    float sinalpha = sin(state->state.alpha_beta[0]);
    float cosalpha = cos(state->state.alpha_beta[0]);
    float p_stab = state->state.ang_velo_b[0]*cosalpha + state->state.ang_velo_b[2]*sinalpha;
    float r_stab = state->state.ang_velo_b[2]*cosalpha - state->state.ang_velo_b[0]*sinalpha;

    state->state.forces_air_sb[0]  = -qS*(state->derivatives.CD+state->derivatives.CDde*data.ele_def*(state->commanded.elevator+state->commanded.elevatorTrim));
    state->state.forces_air_sb[0] += -qS_V*data.cmg*(state->derivatives.CDq*state->state.ang_velo_b[1]+state->derivatives.CDalphadot*state->state.alpha_beta_dot[0]);

    state->state.forces_air_sb[1]  =  qS*(state->derivatives.Cydr*data.rud_def*(state->commanded.rudder+state->commanded.rudderTrim) + state->derivatives.Cyda*(state->commanded.ailerons+state->commanded.aileronTrim) + state->derivatives.Cybeta*state->state.alpha_beta[1]);
    state->state.forces_air_sb[1] +=  qS_V*data.b*(state->derivatives.Cybetadot*state->state.alpha_beta_dot[1]+state->derivatives.Cyp*state->state.ang_velo_b[1]+state->derivatives.Cyr*state->state.ang_velo_b[2]);

    state->state.forces_air_sb[2]  = -qS*(state->derivatives.CL+ state->derivatives.CLde*data.ele_def*(state->commanded.elevator+state->commanded.elevatorTrim));
    state->state.forces_air_sb[2] += -qS_V*data.cmg*(state->derivatives.CLq*state->state.ang_velo_b[1] + state->derivatives.CLalphadot*state->state.alpha_beta_dot[0]);

    //qDebug() <<  state->forces_air_sb[0]<< state->forces_air_sb[1];
    /// 6: Compute Engine and Gear
    /// ADD THIS
    state->state.engine_forces_b[0] = state->engine.thrust*data.engVector[0];
    state->state.engine_forces_b[1] = state->engine.thrust*data.engVector[1];
    state->state.engine_forces_b[2] = state->engine.thrust*data.engVector[2];
    state->state.engine_moment_b[0] = 0.0f;
    state->state.engine_moment_b[1] = 0.0f;
    state->state.engine_moment_b[2] = 0.0f;
    this->computeGearForces(state);
    /// 7: Body Frame Forces
    state->state.forces_b[0] = state->state.forces_air_sb[0]*cosalpha-state->state.forces_air_sb[2]*sinalpha - state->derivatives.mass*G_ACCEL*sin(state->state.eulerangles[1]) + state->state.engine_forces_b[0] + state->state.gear_forces_b[0];
    state->state.forces_b[1] = state->state.forces_air_sb[1] + state->derivatives.mass*G_ACCEL*sin(state->state.eulerangles[2])*cos(state->state.eulerangles[1]) + state->state.engine_forces_b[1] + state->state.gear_forces_b[1];
    state->state.forces_b[2] = state->state.forces_air_sb[0]*sinalpha + state->state.forces_air_sb[2]*cosalpha  + state->derivatives.mass*G_ACCEL*cos(state->state.eulerangles[1])*cos(state->state.eulerangles[2]) + state->state.engine_forces_b[2] + state->state.gear_forces_b[2];
    //qDebug() << state->state.gear_forces_b[0];
    /// 12: stability Axes momentum

    state->state.moment_air_sb[0]  = qS*data.b*(state->derivatives.Clbeta*state->state.alpha_beta[1] + state->derivatives.Clda*(state->commanded.ailerons+state->commanded.aileronTrim)*data.ail_def + state->derivatives.Cldr*(state->commanded.rudder+state->commanded.rudderTrim)*data.rud_def);
    state->state.moment_air_sb[0] += qS_V*data.b*data.b*(state->derivatives.Clp*p_stab + state->derivatives.Clr*r_stab + state->derivatives.Clbetadot*state->state.alpha_beta_dot[1]);

    state->state.moment_air_sb[1]  = qS*data.cbar*(state->derivatives.Cm0 + state->derivatives.Cmalpha*state->state.alpha_beta[0] + state->derivatives.Cmde*(state->commanded.elevator+state->commanded.elevatorTrim)*data.ele_def);
    state->state.moment_air_sb[1] += qS_V*data.cbar*data.cbar*(state->derivatives.Cmq*state->state.ang_velo_b[1]+state->derivatives.Cmalphadot*state->state.alpha_beta_dot[0]);

    state->state.moment_air_sb[2]  = qS*data.b*(state->derivatives.Cnbeta*state->state.alpha_beta[1] + state->derivatives.Cnda*(state->commanded.ailerons+state->commanded.aileronTrim)*data.ail_def + state->derivatives.Cndr*(state->commanded.rudder+state->commanded.rudderTrim)*data.rud_def);
    state->state.moment_air_sb[2] += qS_V*data.b*data.b*(state->derivatives.Cnp*p_stab + state->derivatives.Cnr*r_stab + state->derivatives.Cnbetadot*state->state.alpha_beta_dot[1]);
    //qDebug() << qS << state->moment_air_sb[0] << state->moment_air_sb[1] << state->moment_air_sb[2];
    /// 13: body frame moments
    state->state.moment_b[0] = state->state.moment_air_sb[0]*cosalpha - state->state.moment_air_sb[2]*sinalpha + state->state.forces_air_sb[2]*sinalpha*state->derivatives.mass_center[1] +state->state.engine_moment_b[0] + state->state.gear_moment_b[0];
    state->state.moment_b[1] = state->state.moment_air_sb[1] - state->state.forces_air_sb[2]*(-state->derivatives.mass_center[0] - 0.25*data.cbar)*cosalpha - state->state.forces_air_sb[0]*(-state->derivatives.mass_center[0] - 0.25*data.cbar)*sinalpha + state->state.engine_moment_b[1] + state->state.gear_moment_b[1];
    state->state.moment_b[2] = state->state.moment_air_sb[2]*cosalpha + state->state.moment_air_sb[0]*sinalpha + state->state.forces_air_sb[1]*(-state->derivatives.mass_center[0] -0.25*data.cbar) + state->state.engine_moment_b[2] + state->state.gear_moment_b[2];

}
/*!
 * \brief planeModelEngine::computeInertias calculates the inertia matrix of the plane
 * \param state plane state
 */
void planeModelEngine::computeInertias(planeStatusData *state)
{
    float totalMatrix[3][3];
    float fuelMatrix[3][3];
    float globalFuelMatrix[3][3];
    float fuelPos[3];
    fuelMatrix[1][0] = 0;
    fuelMatrix[0][1] = 0;
    fuelMatrix[2][0] = 0;
    fuelMatrix[0][2] = 0;
    fuelMatrix[1][2] = 0;
    fuelMatrix[2][1] = 0;
    fuelMatrix[0][0] = (1.0f/12)*this->data.fuel_tank.max_mass*state->state.fuel_ratio*(this->data.fuel_tank.width*this->data.fuel_tank.width + this->data.fuel_tank.height*state->state.fuel_ratio*this->data.fuel_tank.height*state->state.fuel_ratio);
    fuelMatrix[1][1] = (1.0f/12)*this->data.fuel_tank.max_mass*state->state.fuel_ratio*(this->data.fuel_tank.length*this->data.fuel_tank.length + this->data.fuel_tank.height*state->state.fuel_ratio*this->data.fuel_tank.height*state->state.fuel_ratio);
    fuelMatrix[2][2] = (1.0f/12)*this->data.fuel_tank.max_mass*state->state.fuel_ratio*(this->data.fuel_tank.width*this->data.fuel_tank.width + this->data.fuel_tank.length*this->data.fuel_tank.length);

    fuelPos[0] = this->data.fuel_tank.basecenter[0];
    fuelPos[1] = this->data.fuel_tank.basecenter[1];
    fuelPos[2] = this->data.fuel_tank.basecenter[2]-0.5*state->state.fuel_ratio*this->data.fuel_tank.height;

    simUtils::makeSteiner(this->data.fuel_tank.max_mass*state->state.fuel_ratio,fuelPos,fuelPos,fuelMatrix,globalFuelMatrix);

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            totalMatrix[i][j] = 0;
            totalMatrix[i][j] += this->data.dryplane.inertia[i][j];
            if(state->commanded.missile[0]) totalMatrix[i][j] += this->data.missile[0].inertia[i][j];
            if(state->commanded.missile[1]) totalMatrix[i][j] += this->data.missile[1].inertia[i][j];
            float gearTerm = gearState*this->data.gearOut.inertia[i][j] + (1-gearState)*this->data.gearIn.inertia[i][j];
            totalMatrix[i][j] += gearTerm;
            totalMatrix[i][j] += globalFuelMatrix[i][j];
            totalMatrix[i][j] += globalFuelMatrix[i][j];
        }
    }
    float invCG[3];
    invCG[0] = -state->derivatives.mass_center[0];
    invCG[1] = -state->derivatives.mass_center[1];
    invCG[2] = -state->derivatives.mass_center[2];
    simUtils::makeSteiner(state->derivatives.mass,invCG,invCG,totalMatrix,state->derivatives.inertias);
}
/*!
 * \brief planeModelEngine::computeGroundLevel calculates the groun level, subsampling when far from ground. For now is always 0.0f
 * \param state plane state
 * \param map map object
 */
void planeModelEngine::computeGroundLevel(planeStatusData *state)
{

    state->state.ground_altitude = 0.0;
}
/*!
 * \brief planeModelEngine::computeGearForces calculates the forces of the ground interaction
 * \param state plane state
 */
void planeModelEngine::computeGearForces(planeStatusData *state)
{
    double vel[3][3];
    double deltas[3];
    double globalpos[3][3];
    double velocities[3][3];
    double globalvel[3][3];
    double forces[3][3];
    double moments[3][3];
    double moment_res[3];
    if((state->state.position[2] - gear_ground_alt[0]) > 5.0){
        for(int i= 0; i < 3; i++){
            state->state.gear_moment_b[i] = 0.0f;
            state->state.gear_forces_b[i] = 0.0f;
        }
        return;
    }

    simUtils::body2local(data.landingWheels[0].position,state,globalpos[0]);
    simUtils::body2local(data.landingWheels[1].position,state,globalpos[1]);
    simUtils::body2local(data.landingWheels[2].position,state,globalpos[2]);
    for(int i= 0; i < 3; i++)
    {
        // PLUS SIGN BECAUSE OF THE SIGNS
        deltas[i] = state->state.position[2] - gear_ground_alt[i+1] - globalpos[i][2];
    }
    simUtils::crossproduct(state->state.ang_velo_b,data.landingWheels[0].position,vel[0]);
    simUtils::crossproduct(state->state.ang_velo_b,data.landingWheels[1].position,vel[1]);
    simUtils::crossproduct(state->state.ang_velo_b,data.landingWheels[2].position,vel[2]);

    for(int i= 0; i < 3; i++)
    {
        for(int j = 0; j<3; j++)
        {
            // PLUS SIGN BECAUSE OF THE SIGNS
            velocities[i][j] = state->state.velocity_b[j] + vel[i][j];
        }
        simUtils::body2local(velocities[i],state,globalvel[i]);
    }
    for(int i= 0; i < 3; i++)
    {
        double friction = 0;
        double sideforce = 0;
        double normal = -data.landingWheels[i].kspring*deltas[i] +data.landingWheels[i].kdamper*globalvel[i][2];
        //DEBUG
        double inVelocities[2];
        float giro = 0;
        if(i == 0) giro = (PI/6)*state->commanded.rudder;
            inVelocities[0] = velocities[i][0]*cos(giro) - velocities[i][1]*sin(giro);
            inVelocities[1] = velocities[i][0]*sin(giro) + velocities[i][1]*cos(giro);
        if(abs(inVelocities[0]) > 0.0000001){
            friction = ((data.cBreakout*(1-min((double)data.speedLimit,abs(inVelocities[0]))/data.speedLimit)) + data.cRolling*normal)*(inVelocities[0]/abs(inVelocities[0]));
            if(state->commanded.wheelBreak) friction += data.cBreak*normal*(inVelocities[0]/abs(inVelocities[0]));
        }
        if(abs(inVelocities[1]) > 0.0000001){
            sideforce = -normal*data.cSide*(inVelocities[1]/abs(inVelocities[1]))*100*min(0.01,abs(inVelocities[i]));
        }

        if(deltas[i] < 0){
            //CHAPUCILLA
            // FIXME USE GROUNDSPEED
            forces[i][0] = (-friction*cos(giro)+sideforce*sin(giro))*cos(state->state.eulerangles[1]) + normal*sin(state->state.eulerangles[1]);
            forces[i][1] = sideforce*cos(giro) - friction*sin(giro) - normal*sin(state->state.eulerangles[2]);
            forces[i][2] = -friction*sin(state->state.eulerangles[1]) + sideforce*sin(state->state.eulerangles[2]) -normal*cos(state->state.eulerangles[1]);
        }else{
            forces[i][0] = 0;
            forces[i][1] = 0;
            forces[i][2] = 0;
        }
    }
    for(int i = 0; i< 3; i++)
    {
        simUtils::crossproduct(data.landingWheels[i].position,forces[i],moments[i]);
    }

    for(int i = 0; i<3;i++)
    {
        state->state.gear_forces_b[i] = 0;
        for(int j = 0; j < 3; j++)
        {
            state->state.gear_forces_b[i] += forces[j][i];
        }
    }
    simUtils::crossproduct(state->derivatives.mass_center,state->state.gear_forces_b,moment_res);
    for(int i = 0; i<3;i++)
    {
        state->state.gear_moment_b[i] = 0;
        for(int j = 0; j < 3; j++)
        {
            state->state.gear_moment_b[i] += moments[j][i];
        }
        state->state.gear_moment_b[i] -= moment_res[i];
    }
    //qDebug() << state->derivatives.mass_center[0] << data.landingWheels[1].position[0];
}
/*!
 * \brief planeModelEngine::computeEngineStatus determines and propagates the engine status
 * \param state plane status
 */
void planeModelEngine::computeEngineStatus(planeStatusData *state)
{
    double deltabeta;
    double rpm_commanded = (data.min_rpm + state->commanded.propeller*(data.max_rpm-data.min_rpm))/data.red_factor;
    double rpm_current = state->engine.propeller_rpm;
    double rpm_motor = rpm_current*data.red_factor;
    double omega = rpm_current*2*PI/(36.0*60.0);
    double cPowerFactor;
    double cThrustFactor;
    double cT;
    float delta_rpm;
    float cpowerRequired;
    float cpowerAvailable;
    float powerexcess ;
    float torqueexcess;
    float maxpower;
    bool ok;
    float J;
    if(rpm_current >0 ){
        cPowerFactor = state->atmosphere.d*pow((rpm_current/60),3)*pow(this->data.propDiam,5);
        cThrustFactor = state->atmosphere.d*pow((rpm_current/60),2)*pow(this->data.propDiam,4);
        J = state->state.velocity_b[0]/((rpm_current/60)*this->data.propDiam);
    }else{
        cPowerFactor = state->atmosphere.d*pow(this->data.propDiam,5);
        cThrustFactor = state->atmosphere.d*pow(this->data.propDiam,4);
        J = state->state.velocity_b[0]/(this->data.propDiam);
    }
    if(state->state.fuel_ratio > 0){
        maxpower = this->data.rpmMaxP.getValue(rpm_motor,&ok)*this->data.AltIsaCorrection.getValue(state->state.position[2]/0.305,state->atmosphere.ISADiff,&ok)-this->data.powerExtraction;
    }else{
        maxpower = 0.0f;
    }
    if(state->commanded.takeoffmode){
        maxpower *= data.takeOffPerformance;
    }
    state->engine.shaft_power = (maxpower*state->commanded.throttle)*1000;
    cpowerAvailable = max(state->engine.shaft_power/cPowerFactor,0.0);
    cpowerRequired = data.cP.getValue(J,state->engine.beta,&ok);
    powerexcess = (cpowerAvailable-cpowerRequired)*cPowerFactor;
    if(omega > 0.001){
         torqueexcess = powerexcess/omega;
    }else{
         torqueexcess = powerexcess;
    }

    delta_rpm = (60/(2*PI))*torqueexcess/data.propellerIxx;
    state->engine.propeller_rpm += delta_rpm*state->time.delta_t;
    if(state->engine.propeller_rpm < 0) state->engine.propeller_rpm = 0;
    state->engine.engine_rpm = state->engine.propeller_rpm*data.red_factor;
    deltabeta = -data.controlKp*(rpm_commanded-rpm_current) + data.controlKd*delta_rpm;
    state->engine.beta += deltabeta*state->time.delta_t;
    if(state->engine.beta > data.max_pitch) state->engine.beta = data.max_pitch;
    if(state->engine.beta < data.min_pitch) state->engine.beta = data.min_pitch;
    cT = data.cT.getValue(J,state->engine.beta,&ok);
    state->engine.thrust = cT*cThrustFactor;

    //Consumption and Fuel
    state->engine.fuel_consumption = (state->engine.shaft_power + this->data.powerExtraction*1000)*0.001*this->data.ce*0.001;
    if(state->state.fuel_ratio > 0){
        state->state.fuel_ratio -= state->engine.fuel_consumption*state->time.delta_t/this->data.fuel_tank.max_mass;
    }
}
