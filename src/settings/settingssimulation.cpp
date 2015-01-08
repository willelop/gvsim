#include "settingssimulation.h"

settingsSimulation::settingsSimulation(QWidget *parent) :
    QDialog(parent)
{
   QVBoxLayout *checkLayout = new QVBoxLayout();
   simulationFreq = new QGroupBox(tr("Simulation Frequency"));
   simFreq[0] = new QRadioButton(tr("50Hz"));
   simFreq[1] = new QRadioButton(tr("100Hz"));
   integrationMethod = new QComboBox();
   integrationMethod->addItem(tr("Runge-Kutta 4"));
   integrationMethod->addItem(tr("Adam-Bashforth 3"));
   integrationMethod->addItem(tr("Euler"));
    stoponstall = new QCheckBox(tr("Stop on Alpha Max"));
    stoponbetamax = new QCheckBox(tr("Stop on Beta Max"));
    QHBoxLayout *simFreqLayout = new QHBoxLayout(simulationFreq);
    simFreqLayout->addWidget(simFreq[0]);
    simFreqLayout->addWidget(simFreq[1]);
    QGroupBox *methodGroup = new QGroupBox(tr("Integration Method"));
    QVBoxLayout *methodLayout = new QVBoxLayout(methodGroup);
    methodLayout->addWidget(integrationMethod);
    checkLayout->addWidget(simulationFreq);
    checkLayout->addWidget(methodGroup);
    checkLayout->addWidget(stoponstall);
    checkLayout->addWidget(stoponbetamax);
    checkLayout->addStretch(100);

    connect(stoponstall,SIGNAL(clicked(bool)),this,SLOT(exitConditionsChanged()));
    connect(stoponstall,SIGNAL(clicked()),this,SLOT(saveSettings()));
    connect(stoponbetamax,SIGNAL(clicked(bool)),this,SLOT(exitConditionsChanged()));
    connect(stoponbetamax,SIGNAL(clicked()),this,SLOT(saveSettings()));
    connect(simFreq[0],SIGNAL(clicked()),this,SLOT(simFreqChanged()));
    connect(simFreq[1],SIGNAL(clicked()),this,SLOT(simFreqChanged()));
    connect(integrationMethod,SIGNAL(currentIndexChanged(int)),this,SLOT(integMethodChanged(int)));
    connect(integrationMethod,SIGNAL(currentIndexChanged(int)),this,SLOT(saveSettings()));
    this->setLayout(checkLayout);
}
void settingsSimulation::restoreSettings()
{
    QSettings settings;
    QVariant stopstall = settings.value("Simulation/StopStall");
    QVariant stopbetamax = settings.value("Simulation/StopBetaMax");
    QVariant simFrequency = settings.value(("Simulation/simFreq"));
    QVariant intMethod = settings.value(("Simulation/intMethod"));
    //STOP ON ALPHA
    if(stopstall.isValid()){
        stoponstall->setChecked(stopstall.toBool());
    }else{
        settings.setValue("Simulation/StopStall",true);
        stoponstall->setChecked(true);
    }
    //STOP ON BETA
    if(stopbetamax.isValid()){
        stoponbetamax->setChecked(stopbetamax.toBool());
    }else{
        settings.setValue("Simulation/StopBetaMax",true);
        stoponbetamax->setChecked(true);
    }
    //simFreq
    if(simFrequency.isValid())
    {
        if(simFrequency.toInt() == 50){
            simFreq[0]->setChecked(true);
        }else{
            simFreq[1]->setChecked(true);
        }
    }else{
        simFreq[1]->setChecked(true);
    }
    //Integration Method
    if(intMethod.isValid())
    {
        if(intMethod.toInt() == 1){
            integrationMethod->setCurrentIndex(1);
        }else if(intMethod.toInt() == 2){
            integrationMethod->setCurrentIndex(2);
        }else{
            integrationMethod->setCurrentIndex(0);
        }
    }else{
        integrationMethod->setCurrentIndex(0);
    }

    emit updateExitConditions(stoponstall->isChecked(),stoponbetamax->isChecked());
    if(simFreq[0]->isChecked()){
        emit updateSimFreq(50);
    }else{
        emit updateSimFreq(100);

    }
    this->integMethodChanged(integrationMethod->currentIndex());
}

void settingsSimulation::saveSettings()
{
    QSettings settings;
    int simFrequency;
    if(simFreq[0]->isChecked())    simFrequency = 50;
    else                            simFrequency = 100;

    settings.setValue("Simulation/StopStall",stoponstall->isChecked());
    settings.setValue("Simulation/StopBetaMax",stoponbetamax->isChecked());
    settings.setValue("Simulation/simFreq",simFrequency);
    settings.setValue("Simulation/intMethod",integrationMethod->currentIndex());

}
void settingsSimulation::simFreqChanged()
{
    if(simFreq[0]->isChecked()){
        emit updateSimFreq(50);
    }else{
        emit updateSimFreq(100);
    }
    this->saveSettings();
}
void settingsSimulation::exitConditionsChanged()
{
    emit updateExitConditions(this->stoponstall->isChecked(),stoponbetamax->isChecked());
}
void settingsSimulation::integMethodChanged(int method)
{
    switch(method)
    {
    case 0:
        emit updateIntMethod(simEngine::RUNGEKUTTA4);
        break;
    case 1:
        emit updateIntMethod(simEngine::ADAMBASH3);
        break;
    default:
        emit updateIntMethod(simEngine::EULER);
        break;
    }
}
