#ifndef AUTOPILOTTOOLS_H
#define AUTOPILOTTOOLS_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>
#include "simengine/planestatusdata.h"
#include <stdlib.h>
#include <cmath>
/*!
 * \brief The autopilotTools class is a widget to modify in real time the autopilot status
 */

class autopilotTools : public QWidget
{
    Q_OBJECT
public:
    explicit autopilotTools(QWidget *parent = 0);
private:
    QLabel *altitudeLabel;
    QLabel *speedLabel;
    QSpinBox *altitude;
    QSpinBox *speed;
    QCheckBox *enabled_alt;
    QCheckBox *enabled_vel;
    QPushButton *setCurrent_alt;
    QPushButton *setCurrent_spd;

    float lastAlt;
    float lastVel;

signals:
    void changedAPSettings(float,float,bool,bool); ///<  \brief emits a new autopilot status
    void lockLateralModes(bool);                    ///<  \brief forces de DOF Lock when AP is enabled
public slots:
    void updateLastValues(planeStatusData);
private slots:
    void changeState(bool);
    void changeState(int);
    void setCurrentAlt();
    void setCurrentVel();


};

#endif // AUTOPILOTTOOLS_H
