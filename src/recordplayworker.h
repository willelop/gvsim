#ifndef RECORDPLAYWORKER_H
#define RECORDPLAYWORKER_H

#include <QObject>
#include <QDebug>
#include <QDialog>
#include <QHBoxLayout>
#include <QBuffer>
#include <QFile>
#include <QDateTime>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QTimer>
#include <QFile>
#include <QDataStream>
#include <QDateTime>
#include <QSlider>
#include <QMessageBox>
#include <QUdpSocket>
#include <QHostAddress>
#include <QElapsedTimer>
#include "utils/net_fdm.h"
#include "stdint.h"
#include "simengine/planestatusdata.h"
#ifdef Q_OS_WIN32
#include <Winsock2.h>
#else
#include <netinet/in.h>
#include <arpa/inet.h>
#include <endian.h>
#endif
#include <QtEndian>




#define METER2FEET 3.2808399
/*!
 * \brief This class handles writing and reading the flights from files and to files
 *
 */
class recordPlayWorker : public QObject
{
    Q_OBJECT
public:
    explicit recordPlayWorker(QObject *parent = 0);
private:
    int freq;
    int playSpeedFactor;
    int lastNumber;
    int recordSubsampling;
    QDateTime time;
    int debug;
    QHostAddress host;
    int port;
    QString recordFileStr;
    QString playFileStr;
    QFile *recordFile;
    QFile *playFile;
    QDataStream write;
    QDataStream read;
    int entryno;
    int playFileSize;
    int statusSize;
    int tot_size;
    int playEntities;
    int currentPlayEntitie;

    bool recording;
    bool playing;
    bool paused;
    bool data2fg;
    QTimer *playTimer;
    QElapsedTimer *viewTimer;
    double simStep;

    QUdpSocket *udpsocket;
    double htond(double x);
    float htonf(float x);
    void sendData2Fg(planeStatusData);
    void outputData(planeStatusData _status);

signals:
    void sendNewStatus(planeStatusData);///< \brief Emits new information when readed
    void statusChanged(int);            ///< \brief Emits the worker status when changed
    void progressChanged(int,int,int);  ///< \brief Emits new progreess status over the file
    void errorFile(QString);            ///< \brief Emits and error string when needed
    void message2log(QString,LOG::TYPE);///< \brife Emits a message to log

public slots:
    void setRecordSubsampling(int);
    void notifyDataSize(int freq);
    void setdata2fg(bool);
    void receiveNewStatus(planeStatusData);
    void movedPlaySlider(int _pos);
    void changeRecordFile(QString);
    void changePlayFile(QString);
    void record();
    void stop();
    void play();
    void pause();
    void changePlaySpeed(int);
private slots:
    void readEntitie();
    void recordPlayStatus(int);
};

#endif // RECORDPLAYWORKER_H
