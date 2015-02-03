#include "mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // REGISTER SPECIAL SIGNALS METATYPES
    qRegisterMetaType<planeStatusData>("planeStatusData");
    qRegisterMetaType<commandStatusData>("commandStatusData");
    qRegisterMetaType<planeModelData>("planeModelData");
    qRegisterMetaType<Sint16>("Sint16");
    qRegisterMetaType<inputs::CONTROLAXES>("inputs::CONTROLAXES");
    qRegisterMetaType<inputs::CONTROLKEYS>("inputs::CONTROLKEYS");
    qRegisterMetaType<LOG::TYPE>("LOG::TYPE");


    this->setWindowIcon(QPixmap(":/icons/gsim-icon.png"));
    this->setWindowTitle("GVSim Predator Simulator");
    this->setMinimumHeight(600);
    this->setMinimumWidth(900);
    QCoreApplication::setOrganizationName("Guiloma");
    QCoreApplication::setOrganizationDomain("guiloma.es");
    QCoreApplication::setApplicationName("GVSim");
    logWorker01 = new logWorker(this);
    //ITS IMPORTANT THA COMMAND STATUS IS STARTED BEFORE SETTINGS JOYSTICK
    commandstatus01 = new commandStatus();
    menubar = new QMenuBar(this);
    centralwidget01 = new centerWidget(this);
    toolbar = new mytoolbar(this);
    about01 = new about(this);
    view3d = new QDockWidget(tr("Artificial Horizon"),this);

    fileReader01 = new planeFileReader(this);
    view3d->setFeatures(QDockWidget::NoDockWidgetFeatures);
    view3d->setContentsMargins(0,0,0,0);
    view3dWindow *viewguille = new view3dWindow(view3d);
    view3d->setWidget(viewguille);

    controlpanel01 = new controlPanel(this);
    controlpanel01->hide();

    logDisplay01 = new logDisplay(this);
    QDockWidget *logWidget = new QDockWidget(tr("Log Display"),this);
    logWidget->setWidget(logDisplay01);
    logWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    settingsrecord01 = new settingsRecordPlay(this);
    recordplay01 = new recordPlayWorker();
    inputworker01 = new inputWorker();
    newflight01 = new newFlightWindow(this);
    exportData01 = new exportData(this);
    exportDataWorker01 = new exportDataWorker();

    this->settingswidget01 = new settingsWindow(this);

    thread01 = new QThread();
    thread02 = new QThread();
    simengine01 = new simEngine();
    simengine01->moveToThread(simengine01);

    toolbar->setMovable(false);
    toolbar->setFixedHeight(50);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // MENUS
    QMenu *flightMenu = menubar->addMenu(tr("&Flight"));
    QAction *newFlightAction = new QAction(QIcon(QPixmap(":/icons/flight.png")),tr("&New Flight"),this);
    QAction *exitProgramAction = new QAction(QIcon(QPixmap(":/icons/exit.png")),tr("&Quit"),this);


    QMenu *settingsMenu = menubar->addMenu(tr("&Settings"));
    QAction *joystickAction = new QAction(QIcon(QPixmap(":/icons/joystick.png")),tr("&Joystick"),this);
    QAction *fgMenuAction = new QAction(QIcon(QPixmap(":/icons/flightgear.png")),tr("&FlightGear I/O"),this);
    QAction *settingsMapAction = new QAction(QIcon(QPixmap(":/icons/world.png")),tr("&Map View Settings"),this);
    QAction *recordplayAction = new QAction(QIcon(QPixmap(":/icons/record.png")),tr("&Record Play Settings I/O"),this);
    QAction *mainSettingsAction = new QAction(QIcon(QPixmap(":/icons/settings.png")),tr("&Configuration"),this);

    QMenu *dataMenu = menubar->addMenu(tr("&Data"));
    QAction *exportAction = new QAction(QIcon(QPixmap(":/icons/export.png")),tr("&Export to CSV"),this);
    QAction *planeModelDataAction = new QAction(QIcon(QPixmap(":/icons/flight.png")),tr("&Plane Model Data"),this);


    QMenu *viewMenu = menubar->addMenu(tr("&View"));
    QAction *showCPanelAction = new QAction(QIcon(QPixmap(":/icons/cockpit.png")),tr("&ControlPanel"),this);
    QAction *showToolsAction = new QAction(QIcon(QPixmap(":/icons/tools.png")),tr("&Tools"),this);



    QMenu *helpMenu = menubar->addMenu(tr("&Help"));
    QAction *aboutAction = new QAction(QIcon(QPixmap(":/icons/gsim-icon.png")),tr("&About"),this);
    settingsMenu->addAction(recordplayAction);
    settingsMenu->addSeparator();
    settingsMenu->addAction(joystickAction);
    settingsMenu->addAction(fgMenuAction);
    settingsMenu->addAction(settingsMapAction);
    settingsMenu->addSeparator();
    settingsMenu->addAction(mainSettingsAction);

    flightMenu->addAction(newFlightAction);
    flightMenu->addAction(exitProgramAction);

    dataMenu->addAction(exportAction);
    dataMenu->addAction(planeModelDataAction);

    viewMenu->addAction(showCPanelAction);
    viewMenu->addAction(showToolsAction);
    helpMenu->addAction(aboutAction);

    this->setMenuBar(menubar);
    this->addToolBar(toolbar);
    this->setCentralWidget(centralwidget01);
    this->addDockWidget(Qt::RightDockWidgetArea,view3d);
    this->addDockWidget(Qt::RightDockWidgetArea,logWidget);


    this->setupLeftToolBox();

    // INPUT CONNECTS
    connect(settingswidget01->settingsjoy01,SIGNAL(requestOpenJoy(int)),inputworker01,SLOT(changeJoystick(int)));
    connect(settingswidget01->settingsjoy01,SIGNAL(newAxeValue(inputs::CONTROLAXES,int,bool)),inputworker01,SLOT(restoreAxis(inputs::CONTROLAXES,int,bool)));
    connect(settingswidget01->settingsjoy01,SIGNAL(newKeyValue(inputs::CONTROLKEYS,int)),inputworker01,SLOT(restoreKey(inputs::CONTROLKEYS,int)),Qt::QueuedConnection);
    connect(settingswidget01->settingsjoy01,SIGNAL(joystickEnabledChange(bool)),commandstatus01,SLOT(enabledJoy(bool)));


    //INPUT WORKER 2 COMMAND STATUS
    connect(inputworker01,SIGNAL(changedKeys(inputs::CONTROLKEYS)),commandstatus01,SLOT(setKeys(inputs::CONTROLKEYS)),Qt::QueuedConnection);
    connect(inputworker01,SIGNAL(changedSticks(Sint16,Sint16,Sint16,Sint16)),settingswidget01->settingsjoy01,SLOT(receivedChangedSticks(Sint16,Sint16,Sint16,Sint16)),Qt::QueuedConnection);
    connect(inputworker01,SIGNAL(changedSticks(Sint16,Sint16,Sint16,Sint16)),commandstatus01,SLOT(setSticks(Sint16,Sint16,Sint16,Sint16)),Qt::QueuedConnection);
    settingswidget01->settingsjoy01->restoreAll();
    connect(controlpanel01,SIGNAL(newControlPacket(commandStatusData,bool,bool,bool)),commandstatus01,SLOT(getNewControlStatus(commandStatusData,bool,bool,bool)));
    connect(settingswidget01->settingsjoy01,SIGNAL(requestOpenJoy(int)),inputworker01,SLOT(changeJoystick(int)));
    connect(simengine01,SIGNAL(newTrimmedCommand(commandStatusData)),controlpanel01,SLOT(getNewTrimmedCommand(commandStatusData)),Qt::QueuedConnection);
    connect(commandstatus01,SIGNAL(commandUpdated(commandStatusData)),controlpanel01,SLOT(getStatusFromJoy(commandStatusData)),Qt::QueuedConnection);

    //MENU CONNECTS
    connect(mainSettingsAction,SIGNAL(triggered()),settingswidget01,SLOT(show()));
    connect(joystickAction,SIGNAL(triggered()),settingswidget01,SLOT(showJoystick()));
    connect(settingsMapAction,SIGNAL(triggered()),settingswidget01,SLOT(showMap()));
    connect(fgMenuAction,SIGNAL(triggered()),settingswidget01,SLOT(showFlightGear()));

    connect(recordplayAction,SIGNAL(triggered()),settingsrecord01,SLOT(show()));
    connect(newFlightAction,SIGNAL(triggered()),newflight01,SLOT(show()));
    connect(exitProgramAction,SIGNAL(triggered()),this,SLOT(close()));
    connect(aboutAction,SIGNAL(triggered()),about01,SLOT(show()));
    connect(exportAction,SIGNAL(triggered()),exportData01,SLOT(show()));
    connect(planeModelDataAction,SIGNAL(triggered()),fileReader01,SLOT(show()));
    connect(showCPanelAction,SIGNAL(triggered()),controlpanel01,SLOT(show()));
    connect(showToolsAction,SIGNAL(triggered()),leftDock,SLOT(show()));


    //PLAY RECORD PAUSE STOP CONNECTIONS
    connect(settingsrecord01,SIGNAL(startPlaying()),recordplay01,SLOT(play()),Qt::QueuedConnection);
    connect(settingsrecord01,SIGNAL(pausePlaying()),recordplay01,SLOT(pause()),Qt::QueuedConnection);
    connect(settingsrecord01,SIGNAL(startPlaying()),simengine01,SLOT(pauseSim()),Qt::DirectConnection);
    connect(settingsrecord01,SIGNAL(stopPlayRecord()),recordplay01,SLOT(stop()),Qt::QueuedConnection);
    connect(settingsrecord01,SIGNAL(startRecording()),recordplay01,SLOT(record()),Qt::QueuedConnection);
    connect(settingsrecord01,SIGNAL(playFileChanged(QString)),recordplay01,SLOT(changePlayFile(QString)),Qt::QueuedConnection);
    connect(settingsrecord01,SIGNAL(recordFileChanged(QString)),recordplay01,SLOT(changeRecordFile(QString)),Qt::QueuedConnection);
    connect(recordplay01,SIGNAL(statusChanged(int)),settingsrecord01,SLOT(recordPlayStatus(int)));
    connect(recordplay01,SIGNAL(progressChanged(int,int,int)),settingsrecord01,SLOT(recordPlaySlide(int,int,int)));
    connect(settingsrecord01,SIGNAL(changedPlaySpeed(int)),recordplay01,SLOT(changePlaySpeed(int)));


    connect(settingsrecord01->playSlider,SIGNAL(sliderMoved(int)),recordplay01,SLOT(movedPlaySlider(int)),Qt::QueuedConnection);
    connect(recordplay01,SIGNAL(errorFile(QString)),this,SLOT(fileError(QString)));

    connect(toolbar->simStart,SIGNAL(triggered()),simengine01,SLOT(reset()),Qt::DirectConnection);
    connect(toolbar->simPause,SIGNAL(triggered()),simengine01,SLOT(pauseSim()),Qt::DirectConnection);
    connect(toolbar->simResume,SIGNAL(triggered()),simengine01,SLOT(resumeSim()),Qt::DirectConnection);
    connect(toolbar->simStop,SIGNAL(triggered()),simengine01,SLOT(stopSim()),Qt::DirectConnection);
    connect(settingswidget01->settingssim01,SIGNAL(updateSimFreq(int)),simengine01,SLOT(setSimulationFreq(int)),Qt::DirectConnection);
    connect(settingswidget01->settingssim01,SIGNAL(updateSimFreq(int)),recordplay01,SLOT(notifyDataSize(int)),Qt::DirectConnection);
    connect(settingswidget01->settingssim01,SIGNAL(updateSimFreq(int)),centralwidget01,SLOT(setSimulationFreq(int)),Qt::DirectConnection);

    connect(settingsrecord01,SIGNAL(updateSubsampling(int)),recordplay01,SLOT(setRecordSubsampling(int)),Qt::DirectConnection);
    connect(toolbar->toogleControP,SIGNAL(toggled(bool)),controlpanel01,SLOT(setVisible(bool)));
    connect(toolbar->toogleRecordP,SIGNAL(toggled(bool)),settingsrecord01,SLOT(setVisible(bool)));
    connect(settingsrecord01,SIGNAL(widgetClose()),toolbar,SLOT(rpanelClosed()));
    connect(settingsrecord01,SIGNAL(widgetShow()),toolbar,SLOT(rpanelOpened()));
    connect(controlpanel01,SIGNAL(widgetClose()),toolbar,SLOT(cpanelClosed()));
    connect(controlpanel01,SIGNAL(widgetShow()),toolbar,SLOT(cpanelOpened()));
    connect(simengine01,SIGNAL(reportStatus(int)),toolbar,SLOT(getSimStatus(int)),Qt::QueuedConnection);
    connect(commandstatus01,SIGNAL(commandUpdated(commandStatusData)),simengine01,SLOT(updateCommand(commandStatusData)),Qt::DirectConnection);
    connect(simengine01,SIGNAL(sendOutput(planeStatusData)),recordplay01,SLOT(receiveNewStatus(planeStatusData)),Qt::QueuedConnection);
    connect(recordplay01,SIGNAL(sendNewStatus(planeStatusData)),centralwidget01,SLOT(updateStatus(planeStatusData)),Qt::QueuedConnection);
    connect(recordplay01,SIGNAL(sendNewStatus(planeStatusData)),viewguille->view3d,SLOT(receivePackage(planeStatusData)),Qt::QueuedConnection);
    connect(recordplay01,SIGNAL(sendNewStatus(planeStatusData)),controlpanel01,SLOT(receivePackage(planeStatusData)),Qt::QueuedConnection);


    //TOOLS CONNNECTIONS
    connect(simtools01,SIGNAL(updatedSpeed(int)),simengine01,SLOT(setSpeedFactor(int)),Qt::DirectConnection);
    connect(simtools01,SIGNAL(updatedSpeed(int)),centralwidget01,SLOT(setSpeed(int)),Qt::DirectConnection);
    connect(simtools01,SIGNAL(updatedDofLock(bool,bool,bool)),simengine01,SLOT(setDOFLock(bool,bool,bool)),Qt::DirectConnection);
    connect(autopilottools01,SIGNAL(changedAPSettings(float,float,bool,bool)),simengine01,SLOT(setAutopilot(float,float,bool,bool)),Qt::DirectConnection);
    connect(autopilottools01,SIGNAL(lockLateralModes(bool)),simtools01,SLOT(setLockLateral(bool)));
    connect(fueltools01,SIGNAL(newFuelCuantity(float)),simengine01,SLOT(setNewFuelRatio(float)),Qt::DirectConnection);
    connect(settingswidget01->settingssim01,SIGNAL(updateExitConditions(bool,bool)),simengine01,SLOT(updateExitConditions(bool,bool)),Qt::DirectConnection);
    connect(settingswidget01->settingssim01,SIGNAL(updateIntMethod(simEngine::INT_METHOD)),simengine01,SLOT(setIntegrationMethod(simEngine::INT_METHOD)),Qt::DirectConnection);

    connect(windtools01,SIGNAL(changedIsaValue(int)),simengine01,SLOT(setISAtempDif(int)),Qt::DirectConnection);
    connect(recordplay01,SIGNAL(sendNewStatus(planeStatusData)),autopilottools01,SLOT(updateLastValues(planeStatusData)));

    // FLIGHTGEAR SETTINGS
    connect(settingswidget01->settingsfg01,SIGNAL(outdata2fg(bool)),recordplay01,SLOT(setdata2fg(bool)));
    connect(settingswidget01->settingsfg01,SIGNAL(indatafromfg(bool)),commandstatus01,SLOT(updatereadFG(bool)));
    //connect(settingswidget01->settingsmap01,SIGNAL(mapThemeChanged(int)),centralwidget01,SLOT(updateMap(int)));
    connect(settingswidget01->settingsiface01,SIGNAL(displayedTabsChanged(bool,bool,bool,bool)),centralwidget01,SLOT(updateShownTabs(bool,bool,bool,bool)));
    connect(settingswidget01->settingsiface01,SIGNAL(updatedHorizSettings(bool,bool)),viewguille->view3d,SLOT(updateSettings(bool,bool)));


    //NEW FLIGHT CONNECTIONS
    connect(newflight01,SIGNAL(startFlight(planeStatusData)),simengine01,SLOT(reset(planeStatusData)),Qt::DirectConnection);
    connect(newflight01,SIGNAL(pauseFlight()),simengine01,SLOT(pauseSim()),Qt::DirectConnection);

    //MODEL DATA CONNECTIONS
    connect(fileReader01,SIGNAL(newPlaneModel(planeModelData)),simengine01,SLOT(updatePlaneModel(planeModelData)),Qt::DirectConnection);
    //EXPORT CONNETIONS
    connect(exportData01,SIGNAL(startExport(QString,QString,QStringList)),exportDataWorker01,SLOT(startExport(QString,QString,QStringList)));
    connect(exportDataWorker01,SIGNAL(updateStatus(int,int)),exportData01,SLOT(statusUpdated(int,int)));
    connect(exportDataWorker01,SIGNAL(finished(bool)),exportData01,SLOT(setFinished(bool)));

    //LOG CONNECTS
    connect(simengine01,SIGNAL(logMessage(QString,LOG::TYPE)),logWorker01,SLOT(write2log(QString,LOG::TYPE)));
    connect(logWorker01,SIGNAL(messagePrinted(QString,LOG::TYPE)),logDisplay01,SLOT(showMessage(QString,LOG::TYPE)));
    connect(recordplay01,SIGNAL(message2log(QString,LOG::TYPE)),logWorker01,SLOT(write2log(QString,LOG::TYPE)));
    connect(settingsrecord01,SIGNAL(message2log(QString,LOG::TYPE)),logWorker01,SLOT(write2log(QString,LOG::TYPE)));
    connect(settingswidget01->settingsjoy01,SIGNAL(message2log(QString,LOG::TYPE)),logWorker01,SLOT(write2log(QString,LOG::TYPE)));
    connect(newflight01,SIGNAL(logMessage(QString,LOG::TYPE)),logWorker01,SLOT(write2log(QString,LOG::TYPE)));
    connect(exportData01,SIGNAL(logMessage(QString,LOG::TYPE)),logWorker01,SLOT(write2log(QString,LOG::TYPE)));
    connect(exportDataWorker01,SIGNAL(logMessage(QString,LOG::TYPE)),logWorker01,SLOT(write2log(QString,LOG::TYPE)));
    connect(fileReader01,SIGNAL(logMessage(QString,LOG::TYPE)),logWorker01,SLOT(write2log(QString,LOG::TYPE)));

    connect(simengine01,SIGNAL(alertMessage(QString)),this,SLOT(simAlert(QString)));


    exportDataWorker01->moveToThread(thread02);

    //recordplay01->moveToThread(thread01);
    inputworker01->moveToThread(thread01);
    simengine01->start();
    thread01->start();
    thread02->start();

    //fileReader01->openFile("/media/truecrypt4/PFC/documents/predator.xml");
    //FIXME change not now
    fileReader01->loadEditContent();

    settingswidget01->restoreSettings();
    myEventFilter *filter = new myEventFilter();
    connect(filter,SIGNAL(keyPress(QKeyEvent*)),inputworker01,SLOT(receiveKeyEvent(QKeyEvent*)),Qt::DirectConnection);
    QApplication::instance()->installEventFilter(filter);
}

MainWindow::~MainWindow()
{
    this->simengine01->stopSim();
    this->simengine01->exit();
    this->thread01->exit();
    this->thread02->exit();
}
void MainWindow::closeEvent(QCloseEvent *)
{
    qApp->exit();
}

void MainWindow::setupLeftToolBox()
{
    simtools01 = new simTools();
    fueltools01 = new fuelTools();
    windtools01 = new windTools();
    autopilottools01 = new autopilotTools();
    leftDock = new QDockWidget(tr("Tools"));
    //leftDock->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    QToolBox *toolbox = new QToolBox(this);
    toolbox->setMinimumWidth(250);
    leftDock->setWidget(toolbox);
//    leftDock->setFeatures(QDockWidget::NoDockWidgetFeatures);

    toolbox->addItem(windtools01,tr("Atmosphere Setup"));
    toolbox->addItem(simtools01,tr("Simulation"));
    toolbox->addItem(fueltools01,tr("Fuel Tools"));
    toolbox->addItem(autopilottools01,tr("Automatic Control"));
    toolbox->setCurrentIndex(1);
    toolbox->resize(toolbox->sizeHint());
    this->addDockWidget(Qt::LeftDockWidgetArea,leftDock);

}

void MainWindow::fileError(QString _string)
{
    QMessageBox wrongFile;
    wrongFile.setText(tr("Wrong file, Not ") +_string);
    wrongFile.exec();
}
void MainWindow::updateSimSpeed()
{

}
void MainWindow::simAlert(QString msg){
    QMessageBox simalert;
    simalert.setText(tr("Sim Alert: ") +msg);
    simalert.exec();
}

