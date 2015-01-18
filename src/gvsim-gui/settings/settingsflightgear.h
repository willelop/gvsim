#ifndef SETTINGSFLIGHTGEAR_H
#define SETTINGSFLIGHTGEAR_H

#include <QDialog>
#include <QCheckBox>
#include <QGroupBox>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QSettings>
/*!
 * \brief The settingsFlightGear class options widget to modify the interaction with flightgear
 */
class settingsFlightGear : public QDialog
{
    Q_OBJECT
public:
    settingsFlightGear(QWidget *parent = 0);
private:
    QGroupBox *flightgearout;
    QGroupBox *flightgearin;
    QCheckBox *datatofg;
    QCheckBox *controlfromfg;
signals:
    void outdata2fg(bool); ///< \brief emits if the data must be sent to FlightGear
    void indatafromfg(bool); ///< \brief emits if the control data has to be readed from FlightGear
public slots:
    void restoreSettings();
private slots:
    void saveOutData(bool);
    void saveInData(bool);


};

#endif // SETTINGSFLIGHTGEAR_H
