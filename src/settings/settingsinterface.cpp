#include "settingsinterface.h"

settingsInterface::settingsInterface(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QGroupBox *artificialHoriz = new QGroupBox(tr("Artificial Horizon"));
    QVBoxLayout *artificialLayout = new QVBoxLayout(artificialHoriz);
    showGrapical = new QCheckBox(tr("ShowGraphicalData"));
    showNumerical = new QCheckBox(tr("Show Numerical Data"));
    showMap = new QCheckBox(tr("Show Map View"));
    showLiveGraph  = new QCheckBox(tr("Show RealTime Graphs"));
    show3dhorizon = new QCheckBox(tr("Show 3d Artificial Horizon"));
    showElevHorizon = new QCheckBox(tr("Show Terrain Elevation in the Artificial Horizon"));

    artificialLayout->addWidget(show3dhorizon);
    artificialLayout->addWidget(showElevHorizon);

    mainLayout->addWidget(showGrapical);
    mainLayout->addWidget(showNumerical);
    mainLayout->addWidget(showMap);
    mainLayout->addWidget(showLiveGraph);
    mainLayout->addWidget(artificialHoriz);

    mainLayout->addStretch(100);
    connect(showGrapical,SIGNAL(clicked()),this,SLOT(saveDisplayedTabs()));
    connect(showNumerical,SIGNAL(clicked()),this,SLOT(saveDisplayedTabs()));
    connect(showMap,SIGNAL(clicked()),this,SLOT(saveDisplayedTabs()));
    connect(showLiveGraph,SIGNAL(clicked()),this,SLOT(saveDisplayedTabs()));
    connect(show3dhorizon,SIGNAL(clicked()),this,SLOT(saveDisplayedTabs()));
    connect(showElevHorizon,SIGNAL(clicked()),this,SLOT(saveDisplayedTabs()));

}
/*!
 * \brief settingsInterface::restoreSettings starts the restoring process
 */
void settingsInterface::restoreSettings()
{
    QSettings settings;
    if(settings.value("Interface/Tabs/ShowMap").isValid()){
        showNumerical->setChecked(settings.value("Interface/Tabs/ShowNumerical").toBool());
        showGrapical->setChecked(settings.value("Interface/Tabs/ShowGraphical").toBool());
        showLiveGraph->setChecked(settings.value("Interface/Tabs/ShowLiveGraphs").toBool());
        showMap->setChecked(settings.value("Interface/Tabs/ShowMap").toBool());
        show3dhorizon->setChecked(settings.value("Interface/Horiz/3dEnabled").toBool());
        showElevHorizon->setChecked(settings.value("Interface/Horiz/elevationEnabled").toBool());

    }else{
        showNumerical->setChecked(true);
        showGrapical->setChecked(true);
        showLiveGraph->setChecked(true);
        showMap->setChecked(true);
        show3dhorizon->setChecked(false);
        showElevHorizon->setChecked(false);
    }
    this->saveDisplayedTabs();
}
void settingsInterface::saveDisplayedTabs()
{
    QSettings settings;
    settings.setValue("Interface/Tabs/ShowMap",showMap->isChecked());
    settings.setValue("Interface/Tabs/ShowGraphical",showGrapical->isChecked());
    settings.setValue("Interface/Tabs/ShowLiveGraphs",showLiveGraph->isChecked());
    settings.setValue("Interface/Tabs/ShowNumerical",showNumerical->isChecked());
    if(!show3dhorizon->isChecked()){
        showElevHorizon->setChecked(false);
        showElevHorizon->setEnabled(false);
    }else{
        showElevHorizon->setEnabled(true);
    }
    settings.setValue("Interface/Horiz/3dEnabled",show3dhorizon->isChecked());
    settings.setValue("Interface/Horiz/elevationEnabled",showElevHorizon->isChecked());
    emit updatedHorizSettings(show3dhorizon->isChecked(),showElevHorizon->isChecked());
    emit displayedTabsChanged(showNumerical->isChecked(),showGrapical->isChecked(),showLiveGraph->isChecked(),showMap->isChecked());
}
