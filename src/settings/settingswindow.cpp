#include "settingswindow.h"

settingsWindow::settingsWindow(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowIcon(QPixmap(":/icons/settings.png"));
    this->setWindowTitle(tr("Preferences"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *centralLayout = new QHBoxLayout();
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    QPushButton *closeButton = new QPushButton(tr("Close"));
    stacklayout = new QStackedLayout();
    indexView = new QListWidget();
    indexView->setIconSize(QSize(30,30));
    indexView->setFixedWidth(200);

    settingsjoy01 = new settingsJoystick();
    settingssim01 = new settingsSimulation();
    settingsfg01 = new settingsFlightGear();
    settingsmap01 = new settingsMap();
    settingsiface01 = new settingsInterface();

    stacklayout->addWidget(settingsjoy01);
    indexView->addItem(new QListWidgetItem(QPixmap(":/icons/joystick.png"),tr("Joystick & Inputs")));
    stacklayout->addWidget(settingssim01);
    indexView->addItem(new QListWidgetItem(QPixmap(":/icons/flight.png"),tr("Simulation")));
    stacklayout->addWidget(settingsfg01);
    indexView->addItem(new QListWidgetItem(QPixmap(":/icons/flightgear.png"),tr("FlightGear")));;
    stacklayout->addWidget(settingsmap01);
    indexView->addItem(new QListWidgetItem(QPixmap(":/icons/world.png"),tr("Map")));
    stacklayout->addWidget(settingsiface01);
    indexView->addItem(new QListWidgetItem(QPixmap(":/icons/monitor.png"),tr("Interface")));


    centralLayout->addWidget(indexView,1);
    centralLayout->addLayout(stacklayout,2);
    bottomLayout->addStretch(10);
    bottomLayout->addWidget(closeButton);
    mainLayout->addLayout(centralLayout,100);
    mainLayout->addLayout(bottomLayout,1);
    connect(indexView,SIGNAL(currentRowChanged(int)),stacklayout,SLOT(setCurrentIndex(int)));
    connect(closeButton,SIGNAL(clicked()),this,SLOT(close()));
    this->setFixedSize(this->sizeHint());
}
void settingsWindow::showFlightGear()
{
    this->indexView->setCurrentRow(2);
    this->show();
}
void settingsWindow::showJoystick()
{
    this->indexView->setCurrentRow(0);
    this->show();
}
void settingsWindow::showSimulation()
{
    this->indexView->setCurrentRow(1);
    this->show();
}
void settingsWindow::showInterface()
{
    this->indexView->setCurrentRow(4);
    this->show();
}
void settingsWindow::showMap()
{
    this->indexView->setCurrentRow(3);
    this->show();
}
void settingsWindow::restoreSettings()
{
    this->settingssim01->restoreSettings();
    this->settingsmap01->restoreSettings();
    this->settingsfg01->restoreSettings();
    this->settingsiface01->restoreSettings();
    this->settingsjoy01->restoreSettings();
}
