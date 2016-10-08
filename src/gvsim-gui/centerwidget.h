#ifndef CENTERWIDGET_H
#define CENTERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QTabWidget>
#include "planestatusdata.h"
#include "graphiccharts/numericaldata.h"
#include "graphiccharts/graphicaldata.h"
#include "graphiccharts/chartsdata.h"
#ifdef USE_MAPVIEWER
#include "3dworld/map3d.h"
#endif

/*!
 * \brief The centerWidget class is the central widget of the software window
 */
class centerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit centerWidget(QWidget *parent = 0);
    QTabWidget *datacontainer01; ///< \brief container with all the graphical information
private:
    numericalData *numericaldata01;
    graphicalData *graphicaldata01;
    chartsData *charts01;
#ifdef USE_MAPVIEWER
    Map3D *mapviewer01;
#endif
public slots:
    void updateStatus(planeStatusData);
    void setSpeed(int speed);
    void setSimulationFreq(int freq);
    void updateShownTabs(bool num, bool graph, bool live, bool map);
};

#endif // CENTERWIDGET_H
