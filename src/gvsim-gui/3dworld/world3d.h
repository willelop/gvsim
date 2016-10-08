#ifndef WORLD3D_H
#define WORLD3D_H

#include <QWidget>
#include <QFile>
#include <QDir>
#include <QLabel>

#include <QMouseEvent>
#include <QKeyEvent>
#include <osg/Notify>
#include <osgViewer/Viewer>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>
#include <osgEarthQt/ViewerWidget>
#include <osgEarth/MapNode>
#include <osgEarth/TerrainEngineNode>
#include <osg/Quat>
#include <osg/ref_ptr>
#include <osgEarth/Registry>
#include <osgEarth/Cache>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/xyz/XYZOptions>
#include <osgEarthDrivers/cache_filesystem/FileSystemCache>
#include <QDialog>
#include <QMainWindow>
#include <QPushButton>
#include <QApplication>
#include <QStackedLayout>
#include <QPushButton>
#include <QTimer>

#include <osgDB/ReadFile>
#include <osgDB/ReaderWriter>
#include <osgDB/Registry>
#include <QVBoxLayout>

#include "planestatusdata.h"

#include <QDebug>
#include <QPainter>
#include <QDockWidget>
#include <QTimer>

#define _RAD2DEG 57.2957795
/*!
 * \brief The world3d class provides a window with the 3d view from the plane with imagery from google maps
 * or openStreetMap and altimetry from SRTM30 data. This 3d view is currently disabled in windows due to some
 * memory leak problem although its still partially used to load the altimetry, used in the simengine.
 */
class world3d : public QWidget
{
    Q_OBJECT
public:
    explicit world3d(QWidget *parent = 0);
    osgEarth::Map* getMap();
private:
    osgEarth::MapNode *mapNode;
    osgViewer::Viewer viewer;
    QWidget* viewWidget ;
    bool enabled;
    osg::ref_ptr<osg::EllipsoidModel> eModel;
    void local2earth(double x_l[3],double lat, double lon, double x_e[3]);
protected:
    void resize(int w, int h);
    void paintEvent(QPaintEvent *);
public slots:
    void updatePosition(planeStatusData _state);
    void setEnabled(bool enabled);


};

#endif // WORLD3D_H
