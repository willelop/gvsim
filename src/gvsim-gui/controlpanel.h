#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QDebug>
#include <QDockWidget>
#include "commandstatusdata.h"
#include "simengine/planestatusdata.h"
/*!
 * \brief The controlPanel class is a widget allowing the user to control and modify de inputs using the mouse and keyboard.
 * It also allows the user to manage the flaps, gear and missiles systems.
 */
class controlPanel : public QDialog
{
    Q_OBJECT
public:
    explicit controlPanel(QWidget *parent = 0);
private:
    QGridLayout *mainLayout;
    QGroupBox *surfacesGroup;
    QGroupBox *engineGroup;
    QGroupBox *flapsGroup;
    QGroupBox *payloadGroup;
    QGroupBox *gearGroup;
    QPushButton *wheelBreak;
    QSlider *surfaces[3];
    QDoubleSpinBox *surfaces_trim[3];
    QDoubleSpinBox *surfaces_values[3];
    QCheckBox *missile[2];
    QCheckBox *gear;
    QCheckBox *enableSurfaces;

    QLabel *aileronLabel;
    QLabel *elevatorLabel;
    QLabel *rudderLabel;
    QLabel *throttleLabel;
    QLabel *rpmLabel;


    QSlider *engine[2];
    QDoubleSpinBox *engine_values[2];
    QCheckBox *mix_throttle_check;
    QCheckBox *throttle_enabled;
    QCheckBox *takeoffmode;
    QRadioButton *flaps[3];

    int surf_sign[3];
protected:
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *);


    
signals:
    void newControlPacket(commandStatusData,bool force_surfaces,bool force_engine,bool propthrot);
    void widgetShow();
    void widgetClose();
public slots:
    void receivePackage(planeStatusData _status);
private slots:
    void updatedThrottle(int);
    void updateControls(int i = 0);
    void spinboxchanged();
    void sliderschanged();
    void getNewTrimmedCommand(commandStatusData);
    void getStatusFromJoy(commandStatusData);
    
};

#endif // CONTROLPANEL_H
