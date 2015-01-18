#include "atmosphereengine.h"

atmosphereEngine::atmosphereEngine()
{
    this->ISADiff = 0.0f;
    temp11 = T_SEALEVEL + DELTA_T_H*11000;
    pres11 = P_SEALEVEL*exp(P_CTE*log(T_SEALEVEL/temp11));
    dens11 = (pres11/temp11)*(M_0_AIR/R_S_AIR);
    temp25 = temp11;
    pres25 = pres11*exp(-G_ACCEL*(25000-11000)*M_0_AIR/(R_S_AIR*temp25));
    dens25 = dens11*exp(-G_ACCEL*(25000-11000)*M_0_AIR/(R_S_AIR*temp25));

}
/*!
 * \brief atmosphereEngine::updateAtmosphere updates the amtosphere status based on the current (calculated in previous iteration)
 * status.
 */
void atmosphereEngine::updateAtmosphere(planeStatusData *state)
{
    /*!
     * We define the potentia altitde, using h = zR/(R+z),used for atmospheric calculations.
     * The temperature calculation is done for the first 11km with lineal constant, -0.0065 degrees/m
     * Pressure calculation is done via P = P0 · exp(CTE + LN(T0/T))
     * Density calculation is done via ro = (P/T)·(M0/RS)
     * Sound speed(a) calculation is done via a = sqrt(gamma·T·(RS/M0))
     */
    state->atmosphere.ISADiff = this->ISADiff;
    state->atmosphere.groundTemperature = T_SEALEVEL+state->atmosphere.ISADiff;
    float potential_altitude = EARTH_RADIUS_*state->state.position[2] / (EARTH_RADIUS_+state->state.position[2]);
    if(potential_altitude <= 11000){ //TROPO
        state->atmosphere.t = std::min(T_SEALEVEL+state->atmosphere.ISADiff,T_SEALEVEL+state->atmosphere.ISADiff + DELTA_T_H*potential_altitude);
        state->atmosphere.p = std::min(P_SEALEVEL*exp(P_CTE*log(T_SEALEVEL/state->atmosphere.t)),(double)P_SEALEVEL);
        state->atmosphere.d = (state->atmosphere.p / state->atmosphere.t)*(M_0_AIR/R_S_AIR);
    }else if(potential_altitude <=25000){ //LOW STRATO
        state->atmosphere.t = temp11;
        state->atmosphere.p = pres11*exp(-G_ACCEL*(potential_altitude-11000)*M_0_AIR/(R_S_AIR*temp11));
        state->atmosphere.d = dens11*exp(-G_ACCEL*(potential_altitude-11000)*M_0_AIR/(R_S_AIR*temp11));
    }else{ //HIGH STRATO
        state->atmosphere.t = temp25 + DELTA_T_H_2*(potential_altitude-25000);
        state->atmosphere.p = pres25*pow((state->atmosphere.t/temp25),(-P_CTE_2));
        state->atmosphere.d = dens25*pow((state->atmosphere.t/temp25),(-P_CTE_2-1));
    }

    state->atmosphere.a = sqrt(GAMMA*state->atmosphere.t*R_S_AIR / M_0_AIR);
}
//void atmosphereEngine::calculatePerturbations(planeStatusData *state)
//{
//    if(state->atmosphere.turb_random){
//        ///MAKE RANDOM RAF OVERWRITING PREVIOUS VALUES
//        state->atmosphere.turb_direction = 0.6;
//        state->atmosphere.turb_intensity = 4;
//        state->atmosphere.turb_interval = 2;
//    }
//    float vz, vy, vx;
//    vy = state->atmosphere.turb_intensity*cos(state->atmosphere.turb_direction);
//    vz = state->atmosphere.turb_intensity*sin(state->atmosphere.turb_direction);
//    vx = state->state.velocity_b[0];
//    state->atmosphere.delta_alpha = atan2(vz,vx);
//    state->atmosphere.delta_beta = atan2(vy,vx);
//}
void atmosphereEngine::updateISADiff(float isadiff)
{
    this->ISADiff = isadiff;
}
