#ifndef SETTINGSINTERFACE_H
#define SETTINGSINTERFACE_H

#include <QDialog>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSettings>
/*!
 * \brief The settingsInterface class provides a widget to modify the visual interface options
 */

class settingsInterface : public QDialog
{
    Q_OBJECT
public:
    explicit settingsInterface(QWidget *parent = 0);
private:
    QCheckBox *showMap;
    QCheckBox *showNumerical;
    QCheckBox *showGrapical;
    QCheckBox *showLiveGraph;

    QCheckBox *show3dhorizon;
    QCheckBox *showElevHorizon;
signals:
    void displayedTabsChanged(bool, bool, bool, bool); ///< \brief emits the settings to the tabs
    void updatedHorizSettings(bool,bool); ///< \brief emits the options to the 3d horizon
public slots:
    void restoreSettings();
    void saveDisplayedTabs();

};

#endif // SETTINGSINTERFACE_H
