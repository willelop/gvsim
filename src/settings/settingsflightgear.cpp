#include "settingsflightgear.h"

settingsFlightGear::settingsFlightGear(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog);
    this->setWindowTitle(tr("FlightGear Options"));
    this->setWindowIcon(QPixmap(":/icons/flightgear.png"));
    QVBoxLayout *mainLayout = new QVBoxLayout();
    // FLIGHT GEAR
    flightgearout = new QGroupBox(tr("Output data to FlightGear"));
    flightgearin = new QGroupBox(tr("Receive Controls from FlightGear"));
    datatofg = new QCheckBox(tr("Live data out to FlightGear"));
    controlfromfg = new QCheckBox(tr("Capture input from FlightGear"));
    controlfromfg->setChecked(false);
    datatofg->setChecked(false);
    QLabel *fginfo = new QLabel(tr("Out Port: <b>5500 UDP</b>.\n Samples per Second: <b>100</b> <br/> <i>--native-fdm=socket,in,100,,5500,udp --fdm=external</i>"));
    QLabel *fginfo_in = new QLabel(tr("In Port: <b>5700 UDP</b>.\n Samples per Second: <b>50</b> <br/> <i>--native-ctrls=socket,out,50,,5700,udp</i>"));

    QVBoxLayout *fglayout = new QVBoxLayout();
    fglayout->addWidget(fginfo);
    fglayout->addWidget(datatofg);

    QVBoxLayout *fglayout_control = new QVBoxLayout();
    fglayout_control->addWidget(fginfo_in);
    fglayout_control->addWidget(controlfromfg);

    flightgearout->setLayout(fglayout);
    flightgearin->setLayout(fglayout_control);
    connect(datatofg,SIGNAL(clicked(bool)),this,SIGNAL(outdata2fg(bool)));
    connect(controlfromfg,SIGNAL(clicked(bool)),this,SIGNAL(indatafromfg(bool)));
    connect(datatofg,SIGNAL(clicked(bool)),this,SLOT(saveOutData(bool)));
    connect(controlfromfg,SIGNAL(clicked(bool)),this,SLOT(saveInData(bool)));
    mainLayout->addWidget(flightgearin);
    mainLayout->addWidget(flightgearout);
    this->setLayout(mainLayout);
    this->setFixedSize(this->sizeHint());
}
/*!
 * \brief settingsInterface::restoreSettings starts the restoring process
 */
void settingsFlightGear::restoreSettings()
{
    QSettings settings;
    QVariant dataOutEnable = settings.value("FlightGear/DataOut/Enable");
    QVariant dataInEnable = settings.value("FlightGear/DataIn/Enable");

    if(dataOutEnable.isValid())
    {
        datatofg->setChecked(dataOutEnable.toBool());

    }
    if(dataInEnable.isValid())
    {
        controlfromfg->setChecked(dataInEnable.toBool());
    }
    emit indatafromfg(controlfromfg->isChecked());
    emit outdata2fg(datatofg->isChecked());

}
void settingsFlightGear::saveOutData(bool state){
    QSettings settings;
    settings.setValue("FlightGear/DataOut/Enable",state);
}
void settingsFlightGear::saveInData(bool state){
    QSettings settings;
    settings.setValue("FlightGear/DataIn/Enable",state);
}
