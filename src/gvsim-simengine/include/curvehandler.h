#ifndef CURVEHANDLER_H
#define CURVEHANDLER_H

#include <QObject>
#include <QPointF>
#include <stdlib.h>
#include <math.h>
#include <QPolygon>
#include <vector>
#include <deque>


using namespace std;
/*!
 * \brief The curveHandler class manages a 2d curve filled with data from plane data file
 */
class curveHandler
{
public:
    explicit curveHandler();
    float getValue(float x,bool *ok);
    float getValue(double x,bool *ok);
    float getInverseValue(float val, bool *ok);
    void addPoint(QPointF);
    void clear();
private:
    deque<pair<float, float> > points;
    float interpolate(pair<float, float> *low, pair<float, float> *high, float *x);
    float interpolateR(pair<float, float> *low, pair<float, float> *high, float *y);
};

#endif // CURVEHANDLER_H
