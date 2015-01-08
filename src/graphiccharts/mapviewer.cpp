#include "mapviewer.h"
using namespace Marble;

mapViewer::mapViewer(): MarbleWidget()
{
    predator = QPixmap(":/images/predator_top.png");
    this->setMapThemeId("/earth/openstreetmap/openstreetmap.dgml");
    this->setMouseTracking(false);
    this->setShowCrosshairs(false);
    this->setShowGrid(true);
    this->setShowCompass(true);
    this->setShowScaleBar(true);

    //this->inputHandler()->setKineticScrollingEnabled(false);
    //->inputHandler()->setPanViaArrowsEnabled(false);
    this->setShowBorders(false);
    this->setShowCities(false);
    this->setShowCompass(false);
    this->setShowOverviewMap(false);
    this->setShowOtherPlaces(false);
    this->setShowCrosshairs(false);
    this->inputHandler()->setPositionSignalConnected(false);
    lat = 0.0;
    lon = 0.0;
    this->roll = 0.0;
    this->yaw = 0.0;
    this->pitch = 0.0;
}

void mapViewer::customPaint(GeoPainter* painter)
{
    qreal x, y;
    if(!this->screenCoordinates(lon,lat,x,y))
    {
        this->centerOn(lon,lat);
        return;
    }

    painter->translate(x,y);
    painter->scale(0.5,0.5);
    painter->rotate(yaw*RAD2DEG_);
    painter->scale(cos(roll),cos(pitch));
    painter->drawPixmap(-predator.width()*0.5,-predator.height()*0.5,predator);

}
void mapViewer::mouseMoveEvent(QMouseEvent *)
{

}
void mapViewer::mousePressEvent(QMouseEvent *)
{

}
void mapViewer::mouseReleaseEvent(QMouseEvent *)
{

}
/*!
 * \brief mapViewer::updateState updates the plane position
 * \param state plane state
 */
void mapViewer::updateState(planeStatusData state)
{
    this->lon = state.state.position[1]*RAD2DEG_;
    this->lat = state.state.position[0]*RAD2DEG_;
    this->yaw = state.state.eulerangles[0];
    this->pitch = state.state.eulerangles[1];
    this->roll = state.state.eulerangles[2];
    this->update();
}
/*!
 * \brief mapViewer::updateMap updates the map theme, openStreetMap, GoogleSat
 * \param map map index, openStreetMap, GoogleSat
 */
void mapViewer::updateMap(int map)
{
    QString mapstr;
    switch(map)
    {
    case 0:
        mapstr = "/earth/openstreetmap/openstreetmap.dgml";
        break;
    case 1:
        mapstr = "/earth/googlesat/googlesat.dgml";
        break;
    default:
        return;
    }
    this->setMapThemeId(mapstr);
    this->update();
    qDebug() << "Changed Theme" << map;
}
