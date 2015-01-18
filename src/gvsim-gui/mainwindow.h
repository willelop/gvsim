#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Es importante poner esta linea delante del resto
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QLabel>
#include <QTimer>
#include <QToolBar>
#include <QDockWidget>
#include <osgEarth/Map>
#include "centerwidget.h"
#include "commandstatus.h"
#include "mytoolbar.h"
#include "planestatusdata.h"
#include <QDebug>
#include <QTimer>
#include <QCoreApplication>
#include <QMetaType>
#include <QToolBox>
#include <QRadioButton>
#include "simengine.h"
#include "view3dwindow.h"
#include "recordplayworker.h"
#include "inputworker.h"
#include "logworker.h"
#include "newflightwindow.h"
#include "about.h"
#include "controlpanel.h"
#include "toolwidgets/simtools.h"
#include "toolwidgets/windtools.h"
#include "toolwidgets/fueltools.h"
#include "toolwidgets/autopilottools.h"
#include "exportdata.h"
#include "exportdataworker.h"
#include "curvehandler.h"
#include "planefilereader.h"
#include "logdisplay.h"
#include "settings/settingswindow.h"
#include "typedefs.h"
#include "settingsrecordplay.h"
#include "myeventfilter.h"

/*!
 * \brief The MainWindow class is the main window of the software, containing everything else
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    inputWorker *inputworker01;
    planeStatusData *outputstatus;
    recordPlayWorker *recordplay01;
    QMenuBar *menubar;
    centerWidget *centralwidget01;
    mytoolbar *toolbar;
    QDockWidget *view3d;
    commandStatus *commandstatus01;
    settingsRecordPlay  *settingsrecord01;
    simEngine *simengine01;
    QThread *thread01;
    QThread *thread02;
    logWorker *logWorker01;
    newFlightWindow *newflight01;
    about *about01;
    planeFileReader *fileReader01;
    //Tools
    QDockWidget *leftDock;
    simTools *simtools01;
    autopilotTools *autopilottools01;
    fuelTools *fueltools01;
    windTools *windtools01;
    logDisplay *logDisplay01;
    controlPanel *controlpanel01;

    exportData *exportData01;
    exportDataWorker *exportDataWorker01;
    settingsWindow *settingswidget01;
    int statusno;
    void setupLeftToolBox();
signals:
    void simSpeedChanged(int);
private slots:
    void fileError(QString);
    void simAlert(QString);
    void updateSimSpeed();

protected:
    void closeEvent(QCloseEvent*);
};


#endif // MAINWINDOW_H
