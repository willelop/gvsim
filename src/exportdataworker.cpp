#include "exportdataworker.h"

exportDataWorker::exportDataWorker(QObject *parent)  :
    QObject(parent)
{

}
/*!
 * \brief exportDataWorker::getValue retrieves a value from a planeStatusData struct in a QString format
 * \param chain value string
 * \param _data status data
 * \return string containing the value
 */
QString exportDataWorker::getValue(QString chain, planeStatusData _data)
{
    if(chain == "Euler0")           return QString::number(_data.state.eulerangles[0]);
    else if(chain == "Euler1")      return QString::number(_data.state.eulerangles[1]);
    else if(chain == "Euler2")      return QString::number(_data.state.eulerangles[2]);
    //
    else if(chain == "Latitude")      return QString::number(_data.state.position[0]);
    else if(chain == "Longitude")      return QString::number(_data.state.position[1]);
    else if(chain == "Altitude")      return QString::number(_data.state.position[2]);
    else if(chain == "AirSpeed")      return QString::number(_data.state.speed_air);
    else if(chain == "Velocity U")      return QString::number(_data.state.velocity_b[0]);
    else if(chain == "Velocity V")      return QString::number(_data.state.velocity_b[1]);
    else if(chain == "Velocity W")      return QString::number(_data.state.velocity_b[2]);
    else if(chain == "Velocity N")      return QString::number(_data.state.velocity_e[0]);
    else if(chain == "Velocity E")      return QString::number(_data.state.velocity_e[1]);
    else if(chain == "Velocity D")      return QString::number(_data.state.velocity_e[2]);
    else if(chain == "Ang Vel p")      return QString::number(_data.state.ang_velo_b[0]);
    else if(chain == "Ang Vel q")      return QString::number(_data.state.ang_velo_b[1]);
    else if(chain == "Ang Vel r")      return QString::number(_data.state.ang_velo_b[2]);
    else if(chain == "Alpha")      return QString::number(_data.state.alpha_beta[0]);
    else if(chain == "Beta")      return QString::number(_data.state.alpha_beta[1]);
    else if(chain == "Alpha_Dot")      return QString::number(_data.state.alpha_beta_dot[0]);
    else if(chain == "Beta_Dot")      return QString::number(_data.state.alpha_beta_dot[1]);
    else if(chain == "Force Xb")      return QString::number(_data.state.forces_b[0]);
    else if(chain == "Force Yb")      return QString::number(_data.state.forces_b[1]);
    else if(chain == "Force Zb")      return QString::number(_data.state.forces_b[2]);
    else if(chain == "Engine Force Xb")      return QString::number(_data.state.engine_forces_b[0]);
    else if(chain == "Engine Force Yb")      return QString::number(_data.state.engine_forces_b[1]);
    else if(chain == "Engine Force Zb")      return QString::number(_data.state.engine_forces_b[2]);
    else if(chain == "Gear Force Xb")      return QString::number(_data.state.gear_forces_b[0]);
    else if(chain == "Gear Force Yb")      return QString::number(_data.state.gear_forces_b[1]);
    else if(chain == "Gear Force Zb")      return QString::number(_data.state.gear_forces_b[2]);
    else if(chain == "Moment L")      return QString::number(_data.state.moment_b[0]);
    else if(chain == "Moment M")      return QString::number(_data.state.moment_b[1]);
    else if(chain == "Moment N")      return QString::number(_data.state.moment_b[2]);
    else if(chain == "Engine Moment L")      return QString::number(_data.state.engine_moment_b[0]);
    else if(chain == "Engine Moment M")      return QString::number(_data.state.engine_moment_b[1]);
    else if(chain == "Engine Moment N")      return QString::number(_data.state.engine_moment_b[2]);
    else if(chain == "Gear Moment L")      return QString::number(_data.state.gear_moment_b[0]);
    else if(chain == "Gear Moment M")      return QString::number(_data.state.gear_moment_b[1]);
    else if(chain == "Gear Moment N")      return QString::number(_data.state.gear_moment_b[2]);
    else if(chain == "Mass")      return QString::number(_data.derivatives.mass);
    else if(chain == "Simulation Time")      return QString::number(_data.time.seconds + _data.time.f_seconds);
    else if(chain == "Ground Altitude")      return QString::number(_data.state.ground_altitude);

    else if(chain == "Ang Accel p_dot")      return QString::number(_data.state.ang_velo_b_dot[0]);
    else if(chain == "Ang Accel q_dot")      return QString::number(_data.state.ang_velo_b_dot[1]);
    else if(chain == "Ang Accel r_dot")      return QString::number(_data.state.ang_velo_b_dot[2]);

    else if(chain == "Shaft Power")      return QString::number(_data.engine.shaft_power);
    else if(chain == "Thrust")      return QString::number(_data.engine.thrust);
    else if(chain == "Engine RPM")      return QString::number(_data.engine.engine_rpm);
    else if(chain == "Propeller RPM")      return QString::number(_data.engine.propeller_rpm);
    else if(chain == "Beta 0.75")      return QString::number(_data.engine.beta);
    else if(chain == "Fuel Consumption")      return QString::number(_data.engine.fuel_consumption);

    else if(chain == "Mass Center X")      return QString::number(_data.derivatives.mass_center[0]);
    else if(chain == "Mass Center Y")      return QString::number(_data.derivatives.mass_center[1]);
    else if(chain == "Mass Center Z")      return QString::number(_data.derivatives.mass_center[2]);
    else if(chain == "Inertia Matrix 11")      return QString::number(_data.derivatives.inertias[0][0]);
    else if(chain == "Inertia Matrix 12")      return QString::number(_data.derivatives.inertias[0][1]);
    else if(chain == "Inertia Matrix 13")      return QString::number(_data.derivatives.inertias[0][2]);
    else if(chain == "Inertia Matrix 21")      return QString::number(_data.derivatives.inertias[1][0]);
    else if(chain == "Inertia Matrix 22")      return QString::number(_data.derivatives.inertias[1][1]);
    else if(chain == "Inertia Matrix 23")      return QString::number(_data.derivatives.inertias[1][2]);
    else if(chain == "Inertia Matrix 31")      return QString::number(_data.derivatives.inertias[2][0]);
    else if(chain == "Inertia Matrix 32")      return QString::number(_data.derivatives.inertias[2][2]);
    else if(chain == "Inertia Matrix 33")      return QString::number(_data.derivatives.inertias[2][2]);

    else if(chain == "CL")                  return QString::number(_data.derivatives.CL);
    else if(chain == "CLalphadot")                  return QString::number(_data.derivatives.CLalphadot);
    else if(chain == "CLde")                  return QString::number(_data.derivatives.CLde);
    else if(chain == "CLq")                  return QString::number(_data.derivatives.CLq);

    else if(chain == "CD")                  return QString::number(_data.derivatives.CD);
    else if(chain == "CDalphadot")                  return QString::number(_data.derivatives.CDalphadot);
    else if(chain == "CDde")                  return QString::number(_data.derivatives.CDde);
    else if(chain == "CDq")                  return QString::number(_data.derivatives.CDq);

    else if(chain == "Cybeta")                  return QString::number(_data.derivatives.Cybeta);
    else if(chain == "Cybetadot")                  return QString::number(_data.derivatives.Cybetadot);
    else if(chain == "Cydr")                  return QString::number(_data.derivatives.Cydr);
    else if(chain == "Cyda")                  return QString::number(_data.derivatives.Cyda);
    else if(chain == "Cyp")                  return QString::number(_data.derivatives.Cyp);
    else if(chain == "Cyr")                  return QString::number(_data.derivatives.Cyr);

    else if(chain == "Cm0")                  return QString::number(_data.derivatives.Cm0);
    else if(chain == "Cmalpha")                  return QString::number(_data.derivatives.Cmalpha);
    else if(chain == "Cmalphadot")                  return QString::number(_data.derivatives.Cmalphadot);
    else if(chain == "Cmq")                  return QString::number(_data.derivatives.Cmq);
    else if(chain == "Cmde")                  return QString::number(_data.derivatives.Cmde);

    else if(chain == "Clbeta")                  return QString::number(_data.derivatives.Clbeta);
    else if(chain == "Clbetadot")                  return QString::number(_data.derivatives.Clbetadot);
    else if(chain == "Cldr")                  return QString::number(_data.derivatives.Cldr);
    else if(chain == "Clda")                  return QString::number(_data.derivatives.Clda);
    else if(chain == "Clp")                  return QString::number(_data.derivatives.Clp);
    else if(chain == "Clr")                  return QString::number(_data.derivatives.Clr);

    else if(chain == "Cnbeta")                  return QString::number(_data.derivatives.Cnbeta);
    else if(chain == "Cnbetadot")                  return QString::number(_data.derivatives.Cnbetadot);
    else if(chain == "Cndr")                  return QString::number(_data.derivatives.Cndr);
    else if(chain == "Cnda")                  return QString::number(_data.derivatives.Cnda);
    else if(chain == "Cnp")                  return QString::number(_data.derivatives.Cnp);
    else if(chain == "Cnr")                  return QString::number(_data.derivatives.Cnr);

    else if(chain == "Com. Elevator")                  return QString::number(_data.commanded.elevator);
    else if(chain == "Com. Ailerons")                  return QString::number(_data.commanded.ailerons);
    else if(chain == "Com. Rudder")                  return QString::number(_data.commanded.rudder);
    else if(chain == "Com. Throttle")                  return QString::number(_data.commanded.throttle);
    else if(chain == "Com. Propeller")                  return QString::number(_data.commanded.propeller);

    else if(chain == "Trim Elevator")                  return QString::number(_data.commanded.elevatorTrim);
    else if(chain == "Trim Ailerons")                  return QString::number(_data.commanded.aileronTrim);
    else if(chain == "Trim Rudder")                  return QString::number(_data.commanded.rudderTrim);

    else if(chain == "Gear Down")                  return QString::number(_data.commanded.gearDown);
    else if(chain == "Gear Brakes")                  return QString::number(_data.commanded.wheelBreak);
    else if(chain == "Flaps")                  return QString::number(_data.commanded.flaps);
    else if(chain == "TakeOff Mode")                  return QString::number(_data.commanded.takeoffmode);
    else if(chain == "Missile 1")                  return QString::number(_data.commanded.missile[0]);
    else if(chain == "Missile 2")                  return QString::number(_data.commanded.missile[1]);

    else if(chain == "Atm. Temp")                  return QString::number(_data.atmosphere.t);
    else if(chain == "Atm. Pres")                  return QString::number(_data.atmosphere.p);
    else if(chain == "Atm. Dens")                  return QString::number(_data.atmosphere.d);
    else if(chain == "Sound Speed")                  return QString::number(_data.atmosphere.a);
    else if(chain == "Wind Speed N")                  return QString::number(_data.atmosphere.wind_sp[0]);
    else if(chain == "Wind Speed E")                  return QString::number(_data.atmosphere.wind_sp[1]);
    else if(chain == "Wind Speed D")                  return QString::number(_data.atmosphere.wind_sp[2]);

    else if(chain == "Air Force Xsb")                  return QString::number(_data.state.forces_air_sb[0]);
    else if(chain == "Air Force Ysb")                  return QString::number(_data.state.forces_air_sb[1]);
    else if(chain == "Air Force Zsb")                  return QString::number(_data.state.forces_air_sb[2]);
    else if(chain == "Air Moment Xsb")                  return QString::number(_data.state.moment_air_sb[0]);
    else if(chain == "Air Moment Ysb")                  return QString::number(_data.state.moment_air_sb[1]);
    else if(chain == "Air Moment Zsb")                  return QString::number(_data.state.moment_air_sb[2]);


    else return "NULL";
}
/*!
 * \brief exportDataWorker::startExport starts the exporting process
 * \param infile file to binary read
 * \param outfile text file to export data to
 * \param colocation column data distribution
 */
void exportDataWorker::startExport(QString infile, QString outfile, QStringList colocation)
{
    planeStatusData datatest;
    QFile *in = new QFile(infile);
    QFile *out = new QFile(outfile);
    int fileStatusSize;
    QDataStream inStream;
    QTextStream outStream;

     if(in->open(QIODevice::ReadOnly))
     {
        inStream.setDevice(in);
        inStream >> fileStatusSize;
        if(fileStatusSize == sizeof(datatest))
        {
            tot_size = fileStatusSize + sizeof(fileStatusSize);
            playEntities= (int)((in->size()-sizeof(fileStatusSize))/tot_size);
        }
     }
     if(out->open((QIODevice::WriteOnly| QIODevice::Text)))
     {
         outStream.setDevice(out);
         for(int i = 0; i < playEntities; i++)
         {
             QString line;
             planeStatusData planeStatus;
             int number;
             inStream >> number;
             inStream.readRawData((char*)&planeStatus,fileStatusSize);
             for(int j = 0; j < colocation.size();j++)
             {
                 line.append(this->getValue(colocation.at(j),planeStatus));
                 line.append(";");
             }
             line.append("\n");
             outStream << line;
             emit updateStatus(i+1,playEntities);
         }
     }

     emit finished(true);


}
