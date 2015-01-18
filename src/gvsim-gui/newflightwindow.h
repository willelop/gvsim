#ifndef NEWFLIGHTWINDOW_H
#define NEWFLIGHTWINDOW_H

#include <QDialog>
#include <QIcon>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSignalMapper>
#include <QLabel>
#include <QSlider>
#include <QTimer>
#include "planestatusdata.h"
#include "planemodeldata.h"
#include "typedefs.h"
#include "simengine.h"


/*!
 * \brief The newFlightWindow class is the "New Flight" widget to start a new flight providing position, speed, altitude, etc...
 */
class newFlightWindow : public QDialog
{
    Q_OBJECT
private:
    QPushButton *startflight;
    QPushButton *closeButton;
    QDoubleSpinBox *latitude;
    QDoubleSpinBox *longitude;
    QDoubleSpinBox *altitude;
    QDoubleSpinBox *airspeed;
    QDoubleSpinBox *course;
    QPushButton *locations[6];
    QSignalMapper *sMapper;
    QGroupBox *startOnAirGroup;
    QSlider *fuel;
    QLabel *fuelq;
public:
    newFlightWindow(QWidget *parent = 0);
private slots:
    void configureFlight();
    void flightButtonPressed();
    void updateFuel(int value);
    void setFavorite(int fav);
signals:
    void pauseFlight();
    void startFlight(planeStatusData startState); ///< \brief starts a new flight with the provided planeStatusData
    void logMessage(QString message, LOG::TYPE type); ///< \brief emits a request to log a message
};

#endif // NEWFLIGHTWINDOW_H
