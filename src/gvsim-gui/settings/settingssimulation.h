#ifndef SETTINGSSIMULATION_H
#define SETTINGSSIMULATION_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QSettings>
#include <QComboBox>
#include <QRadioButton>
#include <QGroupBox>
#include <simengine.h>
/*!
 * \brief The settingsSimulation class holds the settings meeting "simulation" criteria, such as stop simulation on stall.
 */
class settingsSimulation : public QDialog
{
    Q_OBJECT
public:
    explicit settingsSimulation(QWidget *parent = 0);
private:
    QCheckBox *stoponstall;
    QCheckBox *stoponbetamax;
    QGroupBox *simulationFreq;
    QComboBox *integrationMethod;
    QRadioButton *simFreq[2];
signals:
    void updateExitConditions(bool,bool);
    void updateSimFreq(int);
    void updateIntMethod(simEngine::INT_METHOD);
public slots:
    void restoreSettings();
private slots:
    void saveSettings();
    void simFreqChanged();
    void integMethodChanged(int);
    void exitConditionsChanged();

};

#endif // SETTINGSSIMULATION_H
