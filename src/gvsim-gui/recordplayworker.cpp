#include "recordplayworker.h"

recordPlayWorker::recordPlayWorker(QObject *parent) :
    QObject(parent)
{
    this->recordSubsampling = 1;
    this->freq = 100;
    this->lastNumber = 0;
    this->playSpeedFactor = 1;
    data2fg = false;
    udpsocket = new QUdpSocket();
    host = QHostAddress::LocalHost;
    port = 5500;
    planeStatusData planeStatusSample;
    debug = 0;
    statusSize = sizeof(planeStatusSample);
    this->playFileStr = "";
    this->recordFileStr = "";
    this->paused = false;
    this->playing = false;
    this->recording = false;
    this->simStep = DEFAULT_INTEGRATION_STEP;
    recordFile = new QFile();
    playFile = new QFile();

    playTimer = new QTimer();
    playTimer->setInterval(DEFAULT_INTEGRATION_STEP*1000*RECORDING_FACTOR);
    playTimer->start();
    viewTimer = new QElapsedTimer();
    viewTimer->start();
    connect(playTimer,SIGNAL(timeout()),this,SLOT(readEntitie()));
}
/*!
 * \brief This Slot chanes the file to write into
 * \param _file absolute or relative path to file
 */
void recordPlayWorker::changeRecordFile(QString _file)
{
    this->recordFileStr = _file;
}
/*!
 * \brief This Slot chanes the file to read from
 * \param _file absolute or relative path to file
 */
void recordPlayWorker::changePlayFile(QString _file)
{
    this->playFileStr = _file;
}
/*!
 * \brief Start Recording to the file
 */
void recordPlayWorker::record()
{
    recordFile->setFileName(recordFileStr);
    if(recordFile->open(QIODevice::WriteOnly))
    {
        entryno = 0;
        write.setDevice(recordFile);
        write << statusSize;
        recording = true;
        playing = false;
        emit message2log(tr("Recording started in file: ")+recordFileStr,LOG::NORMAL);
        emit statusChanged(3);
    }else {
        emit errorFile(tr("Recording"));
        emit message2log(tr("Cannot start recording: File I/O error."),LOG::ERRORMSG);
        emit statusChanged(0);
        return;
    }
}

/*!
 * \brief Start to play selected file or emits signal informating of the error.
 */
void recordPlayWorker::play()
{

    if(paused){
        //playTimer->start();
        paused = false;
    }else{
    if(playFile->isOpen()) playFile->close();
    playFile->setFileName(playFileStr);
    if(playFile->open(QIODevice::ReadOnly))
    {
        int fileStatusSize;
        read.setDevice(playFile);
        read >> fileStatusSize;
        if(fileStatusSize == statusSize)
        {
            tot_size = statusSize + sizeof(statusSize);
            playEntities= (int)((playFile->size()-sizeof(statusSize))/tot_size);
        }else{
            emit errorFile("right version detected");
            emit message2log(tr("Wrong file version. File descriptor is ") + QString::number(fileStatusSize) +  tr(" And software currently uses ") + QString::number(statusSize),LOG::ERRORMSG);
            read.unsetDevice();
            playFile->close();
        }
        //playTimer->start();
    }else {
        emit errorFile("Playing");
        emit message2log(tr("Cannon start playing due to file I/O error"),LOG::ERRORMSG);
        emit statusChanged(0);
        return;
    }
        emit message2log(tr("Playing file: ") + playFileStr,LOG::NORMAL);
    }
    playing = true;
    recording = false;
    emit statusChanged(1);
}
/*!
 * \brief Pauses the read of the file.
 */
void recordPlayWorker::pause()
{
    playing = false;
    paused = true;
    recording = false;
    emit statusChanged(2);


}
/*!
 * \brief Stop the reading or writing status
 */
void recordPlayWorker::stop()
{
    if(recording)
    {
        emit message2log(tr("Recording stopped"),LOG::NORMAL);
        recordFile->close();
    }
    if(playing || paused)
    {
        emit message2log(tr("Playing stopped"),LOG::NORMAL);
        playFile->close();
    }
    recording = false;
    playing = false;
    paused = false;
    emit statusChanged(0);
    emit progressChanged(0,0,10);

}
/*!
 * \brief This slot writes to the file a new plane status it the worker is in writing mode. If not, it does nothing.
 * \param _status Plane status to save
 */
void recordPlayWorker::receiveNewStatus(planeStatusData _status)
{
    static int counter = 0;
    counter++;
    if(recording && counter%this->recordSubsampling == 0)
    {
        write << entryno;
        write.writeRawData((char*)&_status,sizeof(_status));
        entryno++;
    }
    if(!playing){
        this->outputData(_status);
    }
}
/*!
 * \brief Read an entitie from the file and emits the signal with the information
 */
void recordPlayWorker::readEntitie()
{
    if(playing)
    {
        if(read.atEnd())
        {
            qDebug() << "EOF";
            emit message2log(tr("Play File reached its end. EOF."),LOG::NORMAL);
            this->stop();
            return;
        }else{
        planeStatusData guille;
        int number;
        for(int i = 0; i < playSpeedFactor; i++){
            read >> number;
            read.readRawData((char*)&guille,statusSize);
        }
        if(abs((number-lastNumber)) > 20){
            lastNumber = number;
            emit progressChanged(number,0,playEntities);
        }
        if(simStep != guille.time.delta_t){
            this->playTimer->stop();
            this->playTimer->setInterval(guille.time.delta_t*1000*RECORDING_FACTOR);
            this->playTimer->start();
            this->simStep = guille.time.delta_t;
        }
        this->outputData(guille);
        }
    }
}

/*!
 * \brief Handles the file actualizacion to a position.
 * \param _pos Slider position
 */
void recordPlayWorker::movedPlaySlider(int _pos)
{
    if(playing || paused){
        playFile->seek(sizeof(statusSize) +_pos*tot_size);
    }
}
/*!
 * \brief Handles the status receiving an integer
 * \param _state Status 0: None 1: Playing 2: Paused 3: Recording
 */
void recordPlayWorker::recordPlayStatus(int _state)
{
    switch(_state)
    {
    case 0: //none
        stop();
        break;
    case 1: //playing
        play();
        break;
    case 2: //playing paused
        pause();
        break;
    case 3: //Recording
        record();
        break;

    }
}

double recordPlayWorker::htond (double x)
{
        int    *Double_Overlay;
        int     Holding_Buffer;

        Double_Overlay = (int *) &x;
        Holding_Buffer = Double_Overlay [0];

        Double_Overlay [0] = htonl (Double_Overlay [1]);
        Double_Overlay [1] = htonl (Holding_Buffer);
        return x;
}


float recordPlayWorker::htonf (float x)
{

        int    *Float_Overlay;
        int     Holding_Buffer;

        Float_Overlay = (int *) &x;
        Holding_Buffer = Float_Overlay [0];

        Float_Overlay [0] = htonl (Holding_Buffer);
        return x;
}
void recordPlayWorker::setdata2fg(bool _enabled)
{
    qDebug() << "Seting output to FG" << _enabled;
    this->data2fg = _enabled;
    if(_enabled)
    {
        this->udpsocket->connectToHost(this->host,this->port,QUdpSocket::WriteOnly);
    }else{
        this->udpsocket->disconnectFromHost();
    }
}

void recordPlayWorker::sendData2Fg(planeStatusData _status)
{
    FGNetFDM fgfdm;



       fgfdm.version = qToBigEndian(FG_NET_FDM_VERSION);
       fgfdm.visibility = htonf(0.0);
       fgfdm.warp = htonf(0.0);
       fgfdm.slip_deg = htonf(0.0);
       fgfdm.padding = htonf(1.0);
       fgfdm.stall_warning = htonf(0.0);
       // FIX ME
       fgfdm.vcas = htonf(_status.state.speed_air*METER2FEET);
       fgfdm.v_down = htonf(_status.state.velocity_e[2]*METER2FEET);
       fgfdm.v_east = htonf(_status.state.velocity_e[1]*METER2FEET);
       fgfdm.v_north = htonf(_status.state.velocity_e[0]*METER2FEET);

       fgfdm.latitude = htond(_status.state.position[0]);
       fgfdm.longitude = htond(_status.state.position[1]);
       fgfdm.altitude = htond(_status.state.position[2]);
       fgfdm.agl = htonf((float)(_status.state.position[2]-_status.state.ground_altitude));

       fgfdm.v_body_u = htonf(_status.state.velocity_b[0]*METER2FEET);
       fgfdm.v_body_v = htonf(_status.state.velocity_b[1]*METER2FEET);
       fgfdm.v_body_w = htonf(_status.state.velocity_b[2]*METER2FEET);

       fgfdm.alpha = htonf(_status.state.alpha_beta[0]);
       fgfdm.beta = htonf(_status.state.alpha_beta[1]);

       fgfdm.psi = htonf(_status.state.eulerangles[0]);
       fgfdm.theta = htonf(_status.state.eulerangles[1]);
       fgfdm.phi = htonf(_status.state.eulerangles[2]);
       fgfdm.phidot =  htonf(_status.state.ang_velo_b[2]);
       fgfdm.psidot =  htonf(_status.state.ang_velo_b[0]);
       fgfdm.thetadot =  htonf(_status.state.ang_velo_b[1]);

       fgfdm.A_X_pilot = htonf(_status.state.velocity_b_dot[0]*METER2FEET);
       fgfdm.A_Y_pilot = htonf(_status.state.velocity_b_dot[1]*METER2FEET);
       fgfdm.A_Z_pilot = htonf(_status.state.velocity_b_dot[2]*METER2FEET);
       fgfdm.climb_rate = htonf(-_status.state.velocity_e[2]*METER2FEET);
       fgfdm.cur_time = time.toTime_t();

       // SURFACES
       fgfdm.elevator = htonf(_status.commanded.elevator);
       fgfdm.elevator_trim_tab = htonf(_status.commanded.elevatorTrim);
       fgfdm.right_aileron = htonf(_status.commanded.ailerons);
       fgfdm.right_flap = htonf(_status.commanded.flaps/2.0);
       fgfdm.left_aileron = htonf(_status.commanded.ailerons);
       fgfdm.left_flap = htonf(_status.commanded.flaps/2.0);
       fgfdm.rudder = htonf(-_status.commanded.rudder);
       fgfdm.speedbrake = htonf(0.0);
       fgfdm.spoilers = htonf(0.0);

       fgfdm.nose_wheel = htonf(-_status.commanded.rudder);
       fgfdm.num_engines = htonf(1.0);
       fgfdm.num_tanks = htonf(1.0);;
       fgfdm.num_wheels = htonf(3.0);;

       //ENGINE
       fgfdm.egt[0] = htonf(1.0);
       fgfdm.egt[1] = htonf(0.0);
       fgfdm.egt[2] = htonf(0.0);
       fgfdm.egt[3] = htonf(0.0);
       fgfdm.rpm[0] = htonf(_status.engine.engine_rpm);
       fgfdm.rpm[1] = htonf(0.0);
       fgfdm.rpm[2] = htonf(0.0);
       fgfdm.rpm[3] = htonf(0.0);
       fgfdm.cht[0] = htonf(0.0);
       fgfdm.cht[1] = htonf(0.0);
       fgfdm.cht[2] = htonf(0.0);
       fgfdm.cht[3] = htonf(0.0);
       fgfdm.eng_state[0] = htonl(2);
       fgfdm.eng_state[1] = htonf(0.0);
       fgfdm.eng_state[2] = htonf(0.0);
       fgfdm.eng_state[3] = htonf(0.0);
       fgfdm.oil_px[0] = htonf(0.0);
       fgfdm.oil_px[1] = htonf(0.0);
       fgfdm.oil_px[2] = htonf(0.0);
       fgfdm.oil_px[3] = htonf(0.0);
       fgfdm.oil_temp[1] = htonf(0.0);
       fgfdm.oil_temp[0] = htonf(0.0);
       fgfdm.oil_temp[2] = htonf(0.0);
       fgfdm.oil_temp[3] = htonf(0.0);
       fgfdm.mp_osi[0] = htonf(0.0);
       fgfdm.mp_osi[1] = htonf(0.0);
       fgfdm.mp_osi[2] = htonf(0.0);
       fgfdm.mp_osi[3] = htonf(0.0);
       fgfdm.tit[0] = htonf(0.0);
       fgfdm.tit[1] = htonf(0.0);
       fgfdm.tit[2] = htonf(0.0);
       fgfdm.tit[3] = htonf(0.0);
       // FUEL
       fgfdm.fuel_flow[0] = htonf(0.0);
       fgfdm.fuel_flow[1] = htonf(0.0);
       fgfdm.fuel_flow[2] = htonf(0.0);
       fgfdm.fuel_flow[3] = htonf(0.0);
       fgfdm.fuel_px[0] = htonf(0.0);
       fgfdm.fuel_px[1] = htonf(0.0);
       fgfdm.fuel_px[2] = htonf(0.0);
       fgfdm.fuel_px[3] = htonf(0.0);
       fgfdm.fuel_quantity[0] = htonf(0.0);
       fgfdm.fuel_quantity[1] = htonf(0.0);
       fgfdm.fuel_quantity[2] = htonf(0.0);
       fgfdm.fuel_quantity[3] = htonf(0.0);
       //GEAR
       fgfdm.gear_compression[0] = htonf(0.0);
       fgfdm.gear_compression[1] = htonf(0.0);
       fgfdm.gear_compression[2] = htonf(0.0);
       fgfdm.gear_pos[0] = htonf(0.0);
       fgfdm.gear_pos[1] = htonf(0.0);
       fgfdm.gear_pos[2] = htonf(0.0);
       fgfdm.gear_steer[0] = htonf(0.0);
       fgfdm.gear_steer[1] = htonf(0.0);
       fgfdm.gear_steer[2] = htonf(0.0);
       fgfdm.wow[0] = htonf(0.0);
       fgfdm.wow[1] = htonf(0.0);
       fgfdm.wow[2] = htonf(0.0);

       this->udpsocket->writeDatagram((char*)&fgfdm,sizeof(fgfdm),host,port);
}
void recordPlayWorker::changePlaySpeed(int speed)
{
    this->playSpeedFactor = speed;
    qDebug() << "Changed speed" << speed;
}
void recordPlayWorker::notifyDataSize(int freq)
{
    planeStatusData sample;
    this->freq = freq;
    int size = sizeof(sample);
    float kbps = (float)size*0.001f*freq;
    kbps /= this->recordSubsampling;
    emit message2log("Data will grow at "+QString::number(kbps)+" kBytes/s in real time mode "+QString::number(freq)+"Hz",LOG::INFO);
}
void recordPlayWorker::outputData(planeStatusData _status)
{
    if(viewTimer->elapsed() >= VIEWING_STEP*1000)
    {
        viewTimer->restart();
        emit sendNewStatus(_status);
        if(data2fg)
        {
            this->sendData2Fg(_status);
        }
    }else{
        //qDebug() << "Not Viewing";
    }
}

void recordPlayWorker::setRecordSubsampling(int _sub)
{
    this->recordSubsampling = _sub;
    this->notifyDataSize(freq);
}
