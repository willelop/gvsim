#include "world3d.h"
using namespace osgEarth;
using namespace osgEarth::Drivers;
world3d::world3d(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::Dialog);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);

    //this->loadProgramatically(mapNode);
    //mapNode = osgEarth::MapNode::findMapNode(osgDB::readNodeFile(QString(QApplication::applicationDirPath()+"/openstreetmap.earth").toStdString()));
#ifdef WIN32
//    QString file = QString(QApplication::applicationDirPath()+"/googlesat.earth");
    QString file = QString(QApplication::applicationDirPath()+"/openstreetmap_full.earth");
#else
//    QString file = QString(QDir::homePath()+"/.gvsim/googlesat.earth");
    QString file = QString(QDir::homePath()+"/.gvsim/openstreetmap_full.earth");
#endif

    mapNode = osgEarth::MapNode::findMapNode(osgDB::readNodeFile(file.toStdString()));
    if ( !mapNode )
    {
        qDebug() << "Error";
        return;
    }else{
        qDebug() << "Map Loaded Ok";
    }
    viewer.setSceneData(mapNode);
    viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
    osg::DisplaySettings::instance()->setMinimumNumStencilBits(8);
    viewer.setRunFrameScheme(viewer.CONTINUOUS);


    viewWidget = new osgEarth::QtGui::ViewerWidget(&viewer);
    viewWidget->setParent(this);
    viewer.getDatabasePager()->setUnrefImageDataAfterApplyPolicy( false, false );
    // add it to the composite viewer.
    double x,y,z;
    eModel = mapNode->getTerrainEngine()->getEllipsoidModel();
    eModel->convertLatLongHeightToXYZ(0/_RAD2DEG,0/_RAD2DEG,50000000.0,x,y,z);
    viewer.getCamera()->setViewMatrixAsLookAt(osg::Vec3f(x,y,z), osg::Vec3f(0,0,0), osg::Vec3f(0,0,1));
    viewer.getCamera()->setProjectionMatrixAsPerspective(65,1,0.5,50000);
    viewer.getCamera()->setNearFarRatio(0.000001);
    mainLayout->addWidget(viewWidget);
    this->setLayout(mainLayout);
    this->setMinimumSize(400,400);
    this->setContentsMargins(0,0,0,0);
#ifdef USING_3DMAP
    this->enabled = false;
#else
    this->enabled = false;
#endif
    QTimer *timer = new QTimer();
    timer->setSingleShot(false);
    timer->setInterval(30);
    timer->start();
//    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
}
void world3d::paintEvent(QPaintEvent*)
{
    if(enabled){
        // refresh all the views.
        if (viewer.checkNeedToDoFrame() )
        {
            viewer.frame();
        }
    }
}
void world3d::resize(int w, int h)
{
    viewer.getCamera()->setProjectionMatrixAsPerspective(65,(float)w/(float)h,0.5,50000);
}

/*!
 * \brief world3d::updatePosition updates the view with a new plane status
 * \param _state plane status
 */
void world3d::updatePosition(planeStatusData _state)
{
    if(enabled)
    {
        double posx,posy,posz;
        eModel->convertLatLongHeightToXYZ(_state.state.position[0],_state.state.position[1],_state.state.position[2]+1.0,posx,posy,posz);

        double course = -_state.state.eulerangles[0];
        double elevation = -_state.state.eulerangles[1];
        double roll = _state.state.eulerangles[2];
        double center[3];

        center[0] = 50000*cos(course)*cos(elevation);
        center[1] = 50000*(sin(course)*cos(elevation));
        center[2] = 50000*(-sin(elevation));

        double center_earth[3];
        this->local2earth(center,_state.state.position[0],_state.state.position[1],center_earth);

        double up[3];
        up[0] = (sin(elevation)*cos(course)*cos(roll)+sin(course)*sin(roll));
        up[1] = (cos(roll)*sin(course)*sin(elevation)-cos(course)*sin(roll));
        up[2] = cos(elevation)*cos(roll);
        double up_earth[3];
//        center_earth[0] = 0.0;
//        center_earth[1] = 0.0;
//        center_earth[2] = 0.0;
//        up_earth[0] = 0.0;
//        up_earth[1] = 0.0;
//        up_earth[2] = 1.0;
        this->local2earth(up,_state.state.position[0],_state.state.position[1],up_earth);
        viewer.getCamera()->setViewMatrixAsLookAt(osg::Vec3d(posx,posy,posz),osg::Vec3d(posx+center_earth[0],posy+center_earth[1],posz+center_earth[2]),osg::Vec3d(up_earth[0],up_earth[1],up_earth[2]));
//        viewer.realize();
        this->update();

    }else{
        return;
    }
}
void world3d::local2earth(double x_l[3],double lat, double lon, double x_e[3])
{
    double x_l_in[3];
    double lon_in = lon;
    double lat_in = -lat;
    x_l_in[0] = x_l[2];
    x_l_in[1] = -x_l[1];
    x_l_in[2] = x_l[0];
    x_e[0] = cos(lat_in)*cos(lon_in)*x_l_in[0] - sin(lon_in)*x_l_in[1] + cos(lon_in)*sin(lat_in)*x_l_in[2];
    x_e[1] = cos(lat_in)*sin(lon_in)*x_l_in[0] + cos(lon_in)*x_l_in[1] + sin(lon_in)*sin(lat_in)*x_l_in[2];
    x_e[2] = -sin(lat_in)*x_l_in[0] + cos(lat_in)*x_l_in[2];
}
osgEarth::Map* world3d::getMap()
{
    return this->mapNode->getMap();
}

void world3d::setEnabled(bool _enabled)
{
    this->enabled = _enabled;
}

