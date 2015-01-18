#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QStackedLayout>
#include <QSize>
#include "settingsmap.h"
#include "settingsflightgear.h"
#include "settingsjoystick.h"
#include "settingssimulation.h"
#include "settingsinterface.h"
#include <QHBoxLayout>
#include <QListWidget>
#include <QStackedLayout>
/*!
 * \brief The settingsWindow class is the interactive windows to configure the software
 */
class settingsWindow : public QDialog
{
    Q_OBJECT
public:
    explicit settingsWindow(QWidget *parent = 0);
    settingsMap *settingsmap01;
    settingsFlightGear *settingsfg01;
    settingsJoystick *settingsjoy01;
    settingsSimulation *settingssim01;
    settingsInterface *settingsiface01;
private:
    QStackedLayout *stacklayout;
    QListWidget *indexView;

signals:

public slots:
    void showJoystick();
    void showMap();
    void showFlightGear();
    void showSimulation();
    void showInterface();
    void restoreSettings();

};

#endif // SETTINGSWINDOW_H
