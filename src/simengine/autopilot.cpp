#include "autopilot.h"

autoPilot::autoPilot()
{
    this->altEnabled = false;
    this->speedEnabled = false;
    this->commanded_altitude = 0.0;
    this->commanded_speed = 0.0;
    vel_error = 0;
    elev_error = 0;
    alt_error = 0;
    aThrottle = 0.5;
}
/*!
 * \brief autoPilot::doTrim stablishes the initial trim for a given condition
 * \param state plane status
 * \param model plane model
 * \param atmosphere athmospheric status
 */
void autoPilot::doTrim(planeStatusData *state, planeModelEngine *model,atmosphereEngine *atmosphere)
{
    atmosphere->updateAtmosphere(state);
    model->computeMassCenter(state);
    model->computeInertias(state);
    float qS = 0.5*state->atmosphere.d*state->state.speed_air*state->state.speed_air*model->data.Sw;
    //float qS_V = 0.25f*state->atmosphere.d*state->state.speed_air*modelData.Sw;
    bool ok;
    float weight = 0;
    float cL = 0;
    float alpha = 0;
    float thrust = 0;
    float rpm = 4500/2.273f;
    float J = 0;
    float cT = 0;
    float beta = 0;
    float cP = 0;
    float P = 0;
    float throttle = 0;
    float alpha_old = 0;
    float eps = 0.000001;
    for(int c = 0; c < 10000; c++)
    {
        weight = state->derivatives.mass*G_ACCEL-thrust*sin(alpha);
        cL = weight/qS;
        alpha_old= alpha;
        alpha = model->data.cLalpha.getVValue(0,cL,&ok);
        if(fabs(alpha-alpha_old) < eps){
            qDebug() << "Trim Resolvedin " << c << "iterations";
            break;
        }
        thrust = (1/cos(alpha))*0.5*state->atmosphere.d*model->data.Sw*state->state.speed_air*state->state.speed_air*model->data.cDalpha.getValue(alpha,0.0f,&ok);
    }
    state->state.alpha_beta[0] = alpha;
    state->state.alpha_beta[1] = 0;
    state->state.alpha_beta_dot[0] = 0;
    state->state.alpha_beta_dot[1] = 0;
    model->computeDerivatives(state);
    thrust = (1/cos(alpha))*0.5*state->atmosphere.d*model->data.Sw*state->state.speed_air*state->state.speed_air*model->data.cDalpha.getValue(alpha,0.0f,&ok);
    J = state->state.speed_air/((rpm/60)*model->data.propDiam);
    cT = thrust/(state->atmosphere.d*pow((rpm/60),2)*pow(model->data.propDiam,4));
    beta = model->data.cT.getHValue(J,cT,&ok);
    cP = model->data.cP.getValue(J,beta,&ok);
    P = cP*state->atmosphere.d*pow((rpm/60),3)*pow(model->data.propDiam,5);
    throttle = P / (model->data.rpmMaxP.getValue(4500.0f,&ok)*1000);

    float Mf = -qS*state->derivatives.CL*(-state->derivatives.mass_center[0] - 0.25*model->data.cbar)*cos(alpha) - state->derivatives.CD*qS*(-state->derivatives.mass_center[0] - 0.25*model->data.cbar)*sin(alpha) + 0;
    float elevator = ((Mf/(qS*model->data.cbar))-state->derivatives.Cm0 - state->derivatives.Cmalpha*alpha)/(state->derivatives.Cmde*model->data.ele_def);
    qDebug() << alpha << cL << elevator;


    state->state.velocity_b[0] = state->state.speed_air*cos(alpha);
    state->state.velocity_b[2] = -state->state.speed_air*sin(alpha);

    simUtils::initializeCuaternion(state,QVector3D(state->state.eulerangles[0],alpha,0));
    simUtils::updateRotationMatrix(state);
    state->commanded.elevatorTrim = elevator;
    state->commanded.throttle = throttle;
    state->engine.beta = beta;
    state->engine.thrust = thrust;
    state->engine.engine_rpm = 4500;
    state->engine.propeller_rpm = 4000/model->data.red_factor;
    state->commanded.propeller = (4500-model->data.min_rpm)/(model->data.max_rpm-model->data.min_rpm);
}
/*!
 * \brief autoPilot::doTrim stablishes the initial trim for a given condition
 * \param state plane status
 * \param model plane model
 * \param atmosphere athmospheric status
 * \param map map object
 */
void autoPilot::doTrim2(planeStatusData *state, planeModelEngine *model, atmosphereEngine *atmosphere)
{
    float alpha = 0;
    float qS = 0.5*state->atmosphere.d*state->state.speed_air*state->state.speed_air*model->data.Sw;
    bool ok;
    float thrust = 0;
    float rpm = 4500/2.273f;
    float J = 0;
    float cT = 0;
    float beta = 0;
    float cP = 0;
    float P = 0;
    state->state.velocity_b[0] = state->state.speed_air*cos(alpha);
    state->state.velocity_b[2] = -state->state.speed_air*sin(alpha);
    state->state.alpha_beta[0] = alpha;
    state->engine.engine_rpm = 4500;
    state->engine.propeller_rpm = 4500/model->data.red_factor;
    state->commanded.propeller = (4500-model->data.min_rpm)/(model->data.max_rpm-model->data.min_rpm);
    atmosphere->updateAtmosphere(state);
    model->computeMassCenter(state);
    model->computeInertias(state);
    model->computeDerivatives(state);
    model->computeEngineStatus(state);
    model->computeGroundLevel(state);
    model->computeGearForces(state);
    model->computeForcesMoments(state);
    double vertForce = state->state.forces_b[2]*cos(alpha) - state->state.forces_b[0]*sin(alpha);
    int count = 0;
    while(count < 5000){
        count++;
        state->state.alpha_beta[0] = alpha;
        state->state.velocity_b[0] = state->state.speed_air*cos(alpha);
        state->state.velocity_b[2] = state->state.speed_air*sin(alpha);
        model->computeDerivatives(state);
        thrust = (1/cos(alpha))*qS*model->data.cDalpha.getValue(alpha,0.0f,&ok);
        J = state->state.speed_air/((rpm/60)*model->data.propDiam);
        cT = thrust/(state->atmosphere.d*pow((rpm/60),2)*pow(model->data.propDiam,4));
        beta = model->data.cT.getHValue(J,cT,&ok);
        cP = model->data.cP.getValue(J,beta,&ok);
        P = cP*state->atmosphere.d*pow((rpm/60),3)*pow(model->data.propDiam,5);
        state->commanded.throttle = P / (model->data.rpmMaxP.getValue(4500.0f,&ok)*1000);
        model->computeEngineStatus(state);
        model->computeForcesMoments(state);
        vertForce = state->state.forces_b[2]*cos(alpha) - state->state.forces_b[0]*sin(alpha);
        if(fabs(vertForce) > EPSILON) alpha += min(max(0.00001*vertForce,-0.01),0.01);
        if(fabs(state->state.moment_b[1]) > EPSILON) state->commanded.elevatorTrim += min(max(0.00001*state->state.moment_b[1],-0.01),0.01);
        if(fabs(state->state.moment_b[1]) < EPSILON && fabs(vertForce) < EPSILON){
            break;
        }
     }
    state->state.alpha_beta[0] = alpha;
    state->state.alpha_beta_dot[0] = 0;
    state->state.eulerangles[1] = alpha;
    state->state.velocity_b[0] = state->state.speed_air*cos(alpha);
    state->state.velocity_b[2] = state->state.speed_air*sin(alpha);
    state->state.ang_velo_b[0] = 0.0;
    state->state.ang_velo_b[1] = 0.0;
    state->state.ang_velo_b[2] = 0.0;
    simUtils::initializeCuaternion(state,QVector3D(state->state.eulerangles[0],state->state.eulerangles[1],state->state.eulerangles[2]));
    simUtils::updateRotationMatrix(state);
    simUtils::body2local(state->state.velocity_b,state,state->state.velocity_l);

    qDebug() << count <<state->state.moment_b[1];
}
/*!
 * \brief autoPilot::automaticOverride determines the overriding commands when using autopilot
 * \param state plane state data
 * \param model plane model data
 */
void autoPilot::automaticOverride(planeStatusData *state,planeModelEngine *model)
{
    if(this->altEnabled)
    {
    float max_pitch = model->data.minmax_pitch;
    float com_pitch = 0;

    alt_error += model->data.ki_alt*(commanded_altitude - state->state.position[2]);
    if(alt_error >= 1.0) alt_error = 1.0;
    if(alt_error <= -1.0) alt_error = -1.0;
    com_pitch = alt_error + model->data.kp_alt*(commanded_altitude - state->state.position[2]) + model->data.kd_alt*(state->state.velocity_e[2]);

    if(com_pitch > max_pitch) com_pitch = max_pitch;
    if(com_pitch < -max_pitch) com_pitch = -max_pitch;

    elev_error += model->data.ki_pitch*(com_pitch-state->state.eulerangles[1]);
    if(elev_error >= 1.0) elev_error = 1.0;
    if(elev_error <= -1.0) elev_error = -1.0;

    float elev = elev_error + model->data.kp_pitch*(com_pitch-state->state.eulerangles[1]) +model->data.kd_pitch*(state->state.ang_velo_b[1]);
    if(elev > 1) elev = 1;
    if(elev < -1) elev = -1;


    state->commanded.elevator = elev;

    }
    if(this->speedEnabled)
    {
        vel_error += model->data.ki_speed*(commanded_speed - state->state.speed_air);
        if(vel_error > 1) vel_error = 1;
        if(vel_error < -1) vel_error = -1;

        float deltaThrottle = vel_error + model->data.kp_speed*(commanded_speed - state->state.speed_air) + model->data.kd_speed*(state->state.velocity_b_dot[0]);
        aThrottle = deltaThrottle;
        if(aThrottle > 1.0) aThrottle = 1.0;
        if(aThrottle < 0.01) aThrottle = 0.01;
        state->commanded.throttle = aThrottle;
    }
}

void autoPilot::setAutopilot(float altitude, float speed, bool enabled_alt, bool enabled_speed)
{
    this->commanded_altitude = altitude;
    this->commanded_speed = speed;
    this->altEnabled = enabled_alt;
    this->speedEnabled = enabled_speed;
}

void autoPilot::resetAP()
{
    vel_error = 0;
    elev_error = 0;
    alt_error = 0;
    aThrottle = 0.5;
}
