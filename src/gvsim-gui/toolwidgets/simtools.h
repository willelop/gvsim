#ifndef SIMTOOLS_H
#define SIMTOOLS_H

#include <QWidget>
#include <QHBoxLayout>
#include <QDebug>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QGroupBox>
/*!
 * \brief The simTools class is the toolWidget that modifies parameters related to the simulation such
 * as simulation speed, DOF lock, etc..
 */

class simTools : public QWidget
{
    Q_OBJECT
public:
    explicit simTools(QWidget *parent = 0);
private:
    QGridLayout *simModLayout;
    QGroupBox *simSpeedGroup;
    QRadioButton *normalspeed;
    QRadioButton *speed2x;
    QRadioButton *speed4x;
    QRadioButton *speed8x;
    QRadioButton *speed16x;
    QVBoxLayout *simSpeedLayout;
    QGroupBox *simAxeLock;
    QGroupBox *stallBox;

    QCheckBox *lockPitch;
    QCheckBox *lockRoll;
    QCheckBox *lockYaw;
signals:
    void updatedSpeed(int);
    void updatedDofLock(bool,bool,bool);
private slots:
    void speedChanged();
    void dofLockChanged(bool = true);
public slots:
    void setLockLateral(bool);
    
};

#endif // SIMTOOLS_H
