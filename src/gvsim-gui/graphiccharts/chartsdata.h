#ifndef CHARTSDATA_H
#define CHARTSDATA_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QComboBox>
#include <QLabel>
#include <QString>
#include "graphiccharts/livegraph.h"
#include "planestatusdata.h"
#define CHARTS_NUM 8
/*!
 * \brief The chartsData class contains all the real time graphics
 */
class chartsData : public QWidget
{
    Q_OBJECT
public:
    explicit chartsData(QWidget *parent = 0);
    liveGraph *charts[CHARTS_NUM];
    int frequency;
    int speed;
private:
    QComboBox *timeBox;
signals:
private slots:
    void changedBaseTime(QString entry);
public slots:
    void updateValues(planeStatusData status);
    void setSpeed(int _speed);
    void setFreq(int _freq);
    
};

#endif // CHARTSDATA_H
