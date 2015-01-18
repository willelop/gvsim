#ifndef LIVEGRAPH_H
#define LIVEGRAPH_H

#include <QWidget>
#include <QGLWidget>
 #include <QContiguousCache>
#include <QDebug>
#include "math.h"

#define N_POINTS 128
/*!
 * \brief The liveGraph class provide a visualization method to study flight parameters in real time.
 * These graphics handle various simulation speeds.
 */
class liveGraph : public QGLWidget
{
    Q_OBJECT
public:
    explicit liveGraph(QString _name, float _max = 1.0, float _min = -1.0, float _timesize= 50.0,float _sample_time=DEFAULT_INTEGRATION_STEP*RECORDING_FACTOR,QWidget *parent = 0);
private:
    QString name;
    int counter;
    int counter2;
    float max;
    float min;
    int skip;
    int skip2;
    int basetime;
    float sample_time;
    float timesize;
    int margintext;
    int margin;
    QPolygonF points;

protected:
    virtual void	paintEvent ( QPaintEvent * event );
signals:
    
public slots:
    void clear();
    void newvalue(float);
    void setBaseTime(float secs);
    void setTimeSample(float secs_ps);
    
};

#endif // LIVEGRAPH_H
