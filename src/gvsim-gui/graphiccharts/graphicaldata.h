#ifndef GRAPHICALDATA_H
#define GRAPHICALDATA_H

#include <QWidget>
#include <QScrollArea>
#include "graphiccharts/chartalphabeta.h"
#include "graphiccharts/chartcompass.h"
#include "graphiccharts/chartliftdrag.h"
#include "simengine/planestatusdata.h"
#include "graphiccharts/turnbank.h"
#include "QGridLayout"
/*!
 * \brief The graphicalData class is a widget containing some graphical widget such as chartCompass, chartLiftDrag, turnbank, etc...
 */
class graphicalData : public QWidget
{
    Q_OBJECT
public:
    explicit graphicalData(QWidget *parent = 0);
private:
    QScrollArea *scroll01;
    QWidget *scrollInner;
    chartCompass *compass;
    chartLiftDrag *liftDrag;
    //turnbank *bolabaston;
signals:
    
public slots:
    void receivePackage(planeStatusData);
    
};

#endif // GRAPHICALDATA_H
