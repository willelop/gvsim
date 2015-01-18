#ifndef CURVE3DHANDLER_H
#define CURVE3DHANDLER_H

#include "curvehandler.h"
#include <stdlib.h>
#include <vector>
#include <math.h>
using namespace std;
/*!
 * \brief The curve3DHandler class handles the 3d Maps provided in the planeFile.
 */
class curve3DHandler
{
public:
    explicit curve3DHandler();
    float getValue(float x, float y,bool *ok);
    float getHValue(float V, float val, bool *ok);
    float getVValue(float H, float val, bool *ok);
    void addCurve(float,curveHandler);
    void clear();
private:
    vector<curveHandler> curves;
    vector<float> values;
    float interpolate(curveHandler low, curveHandler high, float x_bar, float y, bool *ok);
    float interpolateR(curveHandler low, curveHandler high, float x_bar, float y, bool *ok); 
};

#endif // CURVE3DHANDLER_H
