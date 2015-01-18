#include "curve3dhandler.h"

curve3DHandler::curve3DHandler()
{
    this->clear();
}
/*!
 * \brief curve3DHandler::clear clears the saved map
 */
void curve3DHandler::clear()
{
    this->curves.clear();
    this->values.clear();
}
/*!
 * \brief curve3DHandler::addCurve adds a curve to the 3D map, the curve values are f(x), meanwhile the curve has a given y coordinate
 * \param _value the "y" value for every point of the given curve
 * \param _curve the curve to add
 */
void curve3DHandler::addCurve(float _value, curveHandler _curve)
{
    this->values.push_back(_value);
    this->curves.push_back(_curve);
}
/*!
 * \brief curve3DHandler::getValue retrieves the map value for a given set of coordinates
 * \param x x coordinate
 * \param y y coordinate
 * \param ok pointer to a boolean that will contain information about the success of the operation
 * \return the value for x,y pair fo values
 */
float curve3DHandler::getValue(float x, float y, bool *ok)
{
    *ok = true;
    if(curves.size() <= 0){
        *ok = false;
        return 0.0f;
    }else if(values.size() == 1){
        return curves.at(0).getValue(y,ok);
    }else{
        if(x < values[0]){
            return curves[0].getValue(y,ok);
        }else if(x > values[values.size()-1]){
            return curves[values.size()-1].getValue(y,ok);
        }else{
            //SHOULD APPLY BOLZANO
//            for(unsigned i = 1; i < values.size(); i++)
//            {
//                if(x >= values.at(i-1) && x <= values.at(i)){
//                    float x_bar = (x - values.at(i-1))/(values.at(i)-values.at(i-1));
//                    return interpolate(curves.at(i-1),curves.at(i),x_bar,y,ok);
//                }
//            }
            //NEWSTUFF BOLZANO
            float middleindex = values.size()/2;
            float topindex = values.size();
            float bottomindex = 0;
            while((topindex-bottomindex) > 1)
            {
                middleindex = (topindex + bottomindex)/2;
                if(x > values.at(middleindex)){
                    bottomindex = middleindex;
                }else{
                    topindex = middleindex;
                }
            }
            float x_bar = (x - values.at(bottomindex))/(values.at(topindex)-values.at(bottomindex));
            return interpolate(curves.at(bottomindex),curves.at(topindex),x_bar,y,ok);
        }
    }
    *ok = false;
    return 0.0f;
}
/*!
 * \brief curve3DHandler::getHValue provides an inverse search method, given a map value and the vertical coordinate
 * \param V vertical coordinate
 * \param val map value
 * \param ok pointer to a boolean containing informatin about the success of the operation
 * \return the H value
 */
float curve3DHandler::getHValue(float V, float val, bool *ok)
{
    *ok = true;
    if(curves.size() <= 0){
        *ok = false;
        return 0.0f;
    }else if(values.size() == 1){
        return curves.at(0).getInverseValue(val,ok);
    }else{
        if(V < values[0]){
            return curves[0].getInverseValue(val,ok);
        }else if(V > values[values.size()-1]){
            return curves[values.size()-1].getInverseValue(val,ok);
        }else{
            for(unsigned i = 1; i < values.size(); i++)
            {
                if(V >= values.at(i-1) && V <= values.at(i)){
                    float x_bar = (V - values.at(i-1))/(values.at(i)-values.at(i-1));
                    return interpolateR(curves.at(i-1),curves.at(i),x_bar,val,ok);
                }
            }
        }
    }
    *ok = false;
    return 0.0f;
}
/*!
 * \brief curve3DHandler::getVValue retrieve a vertical coordinate value for a given map and horizontal value
 * \param H horizontal alue
 * \param val map value
 * \param ok boolean with the success status of the operation
 * \return the Vertical value
 */
float curve3DHandler::getVValue(float H, float val, bool *ok)
{
    curveHandler reverse;
    for(unsigned i = 0; i < this->curves.size();i++)
    {
        reverse.addPoint(QPointF(values.at(i),curves.at(i).getValue(H,ok)));
        if(!ok) return 0.0f;
    }
    return reverse.getInverseValue(val,ok);

}
/*!
 * \brief curve3DHandler::interpolate interpolation method
 */
float curve3DHandler::interpolate(curveHandler low, curveHandler high, float x_bar, float y, bool *ok)
{
    bool ok1, ok2;
    float value_low = low.getValue(y,&ok1);
    float value_high = high.getValue(y,&ok2);
    *ok = ok1 && ok2;
    return value_low*(1-x_bar)+value_high*x_bar;
}
/*!
 * \brief curve3DHandler::interpolate inverse interpolation method
 */
float curve3DHandler::interpolateR(curveHandler low, curveHandler high, float x_bar, float y, bool *ok)
{
    bool ok1, ok2;
    float value_low = low.getInverseValue(y,&ok1);
    float value_high = high.getInverseValue(y,&ok2);
    *ok = ok1 && ok2;
    return value_low*(1-x_bar)+value_high*x_bar;
}
