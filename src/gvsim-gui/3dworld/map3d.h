#ifndef MAP3D_H
#define MAP3D_H

#include <QWidget>
#include <QFile>
#include <QDir>
#include <QLabel>
#include <QObject>

#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDragMoveEvent>
#include <QKeyEvent>
#include <osg/Notify>
#include <osgViewer/Viewer>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>
#include <osgEarthQt/ViewerWidget>
#include <osgEarth/MapNode>
#include <osgEarth/TerrainEngineNode>
#include <osg/Group>
#include <osg/Quat>
#include <osg/ref_ptr>
#include <osgEarth/Registry>
#include <osgEarth/Cache>
#include <osgEarth/GeoData>
#include <osgEarth/Viewpoint>
#include <osgEarth/Utils>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/xyz/XYZOptions>
#include <osgEarthDrivers/cache_filesystem/FileSystemCache>
#include <QDialog>
#include <QMainWindow>
#include <QPushButton>
#include <QApplication>
#include <QStackedLayout>
#include <QPushButton>
#include <osg/PositionAttitudeTransform>
#include <osg/AutoTransform>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarth/Utils>

#include <QTimer>

#include <osgDB/ReadFile>
#include <osgDB/ReaderWriter>
#include <osgDB/Registry>
#include <QVBoxLayout>
#include <osg/Quat>

#include "planestatusdata.h"

#include <QDebug>
#include <QPainter>
#include <QDockWidget>
#include <QTimer>

#define _RAD2DEG 57.2957795
/*!
 * \brief The Map3D class provides a window with the 3d view from the plane with imagery from google maps
 * or openStreetMap and altimetry from SRTM30 data. This 3d view is currently disabled in windows due to some
 * memory leak problem although its still partially used to load the altimetry, used in the simengine.
 */
class Map3D : public QWidget
{
    Q_OBJECT
public:
    explicit Map3D(QWidget *parent = 0);
    osgEarth::Map* getMap();
private:
    float cam_heading;
    float cam_pitch;
    float cam_range;
    osg::Node* planeNode;
    osg::PositionAttitudeTransform * planeTransform;
    osgEarth::Util::EarthManipulator * manipulator;
    osgEarth::Util::SkyNode *skyNode;
    osgEarth::MapNode *mapNode;
    osgViewer::Viewer viewer;
    QWidget* viewWidget ;
    osg::ref_ptr<osg::EllipsoidModel> eModel;
    void local2earth(double x_l[3],double lat, double lon, double x_e[3]);
protected:
    void resize(int w, int h);
    void paintEvent(QPaintEvent *);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
public slots:
    void updatePosition(planeStatusData _state);
};

#endif // MAP3D_H
