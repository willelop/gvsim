#include "curvehandler.h"
using namespace std;
curveHandler::curveHandler()
{
    this->clear();
}
/*!
 * \brief curveHandler::addPoint add a point to the curve
 * \param _point Point to add
 */
void curveHandler::addPoint(QPointF _point)
{
    if(points.front().first > _point.x()) this->points.push_front(make_pair(_point.x(),_point.y()));
    else                                this->points.push_back(make_pair(_point.x(),_point.y()));
}
/*!
 * \brief curveHandler::clear empties the curve
 */
void curveHandler::clear()
{
    this->points.clear();
}
/*!
 * \brief curveHandler::getValue retrives the curve value (y coordinate)
 * \param x "x" coordinate of the request
 * \param ok pointer to boolean status
 * \return the curve value at the given "x" value
 */
float curveHandler::getValue(float x,bool *ok)
{
        *ok = true;
    if(points.size() <= 0){
        *ok = false;
        return 0.0f;
    }else if(points.size() == 1){
        return points.at(0).second;
    }else{
        if(x < points[0].first){
            return points[0].second;
        }else if(x > points[points.size()-1].first){
            return points[points.size()-1].second;
        }else{
            for(unsigned int i = 1; i < points.size(); i++)
            {
                if(x >= points.at(i-1).first && x <= points.at(i).first)
                {
                    return interpolate(&points.at(i-1),&points.at(i),&x);
                }
            }
        }
    }
    *ok = false;
    return 0.0f;
}
/*!
 * \brief curveHandler::interpolate interpolation method
 */
float curveHandler::interpolate(pair<float, float> *low, pair<float, float> *high, float *x)
{
    return low->second + ((high->second - low->second)/(high->first - low->first))*(*x - low->first);
}
/*!
 * \brief curveHandler::interpolateR inverse interpolation method
 */
float curveHandler::interpolateR(pair<float, float> *low, pair<float, float> *high, float *y)
{
    return low->first+((high->first-low->first)/(high->second-low->second))*(*y-low->second);
}
/*!
 * \brief curveHandler::getInverseValue inverse searth method for the curve
 * \param val value of the curve, "y" coordinate
 * \param ok boolean status
 * \return the "x" value for the "y" coordinate, if there are multiple value, returns the first one
 */
float curveHandler::getInverseValue(float val, bool *ok)
{
    *ok = true;
if(points.size() <= 0){
    *ok = false;
    return 0.0f;
}else if(points.size() == 1){
    return points.at(0).second;
}else{
        for(unsigned i = 1; i < points.size(); i++)
        {
            if(val >= points.at(i-1).second && val <= points.at(i).second)
            {
                return interpolateR(&points.at(i-1),&points.at(i),&val);
            }
        }
        if(val < points[0].second){
            return points[0].first;
        }else if(val > points[points.size()-1].second){
            return points[points.size()-1].first;
        }
}
return 0.0f;
}
float curveHandler::getValue(double x, bool *ok)
{
    return this->getValue((float)x,ok);
}
