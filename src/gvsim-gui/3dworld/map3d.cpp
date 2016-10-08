#include "map3d.h"
using namespace osgEarth;
using namespace osgEarth::Drivers;
Map3D::Map3D(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::Dialog);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    this->cam_heading = 0;
    this->cam_pitch = 20.0f/RAD2DEG_;
    this->cam_range = 3000;

    manipulator = new osgEarth::Util::EarthManipulator();

    //this->loadProgramatically(mapNode);
    //mapNode = osgEarth::MapNode::findMapNode(osgDB::readNodeFile(QString(QApplication::applicationDirPath()+"/openstreetmap.earth").toStdString()));
#ifdef WIN32
    QString file = QString(QApplication::applicationDirPath()+"/openstreetmap_full.earth");
    QString vehicle_file  QString(QApplication::applicationDirPath()+"/predator.3ds");
#else
    QString file = QString(QDir::homePath()+"/.gvsim/openstreetmap_full.earth");
    QString vehicle_file = QString(QDir::homePath()+"/.gvsim/predator.3ds");
#endif




    planeNode = NULL;
    planeNode = osgDB::readNodeFile(vehicle_file.toStdString());

    mapNode = osgEarth::MapNode::findMapNode(osgDB::readNodeFile(file.toStdString()));
    if ( !mapNode || !planeNode )
    {
        qDebug() << "Error";
        return;
    }else{
        qDebug() << "Map Loaded Ok";
    }

    planeTransform = new osg::PositionAttitudeTransform();
    planeTransform->addChild(planeNode);
    skyNode = osgEarth::Util::SkyNode::create(mapNode);
    const Config& externals = mapNode->externalConfig();
    Config skyConf = externals.child( "sky" );
    double hours = skyConf.value( "hours", 3.0 );
    skyNode->setDateTime(osgEarth::DateTime(2011, 3, 6, hours ));

    viewer.setSceneData(mapNode);
    viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
    osg::DisplaySettings::instance()->setMinimumNumStencilBits(8);
    viewer.setRunFrameScheme(viewer.CONTINUOUS);



    viewWidget = new osgEarth::QtGui::ViewerWidget(&viewer);
    viewWidget->setFocusPolicy(Qt::NoFocus);
    viewer.getDatabasePager()->setUnrefImageDataAfterApplyPolicy( false, false );
    // add it to the composite viewer.
    double x,y,z;
    eModel = mapNode->getTerrainEngine()->getEllipsoidModel();
    eModel->convertLatLongHeightToXYZ(0/_RAD2DEG,0/_RAD2DEG,1000.0,x,y,z);
    viewer.getCamera()->setViewMatrixAsLookAt(osg::Vec3f(x,y,z), osg::Vec3f(0,0,0), osg::Vec3f(0,0,1));
    viewer.getCamera()->setProjectionMatrixAsPerspective(65,1,1.0,30000);
    viewer.getCamera()->setNearFarRatio(0.0000001);
    viewer.setCameraManipulator(manipulator);
    manipulator->setTetherNode(planeNode);
    manipulator->setDistance(50);
    mapNode->addChild(planeTransform);
    mainLayout->addWidget(viewWidget);
    this->setLayout(mainLayout);
    this->setMinimumSize(400,400);
    this->setContentsMargins(0,0,0,0);
    QTimer *timer = new QTimer();
    timer->setSingleShot(false);
    timer->setInterval(30);
    timer->start();
    QObject::removeEventFilter(this->viewWidget);
//    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
}
void Map3D::paintEvent(QPaintEvent*)
{
    // refresh all the views.
    if (viewer.checkNeedToDoFrame() )
    {
        viewer.frame();
    }
}
void Map3D::resize(int w, int h)
{
    viewer.getCamera()->setProjectionMatrixAsPerspective(65,(float)w/(float)h,0.5,50000);
}

/*!
 * \brief Map3D::updatePosition updates the view with a new plane status
 * \param _state plane status
 */
void Map3D::updatePosition(planeStatusData _state)
{
    double posx,posy,posz;
    eModel->convertLatLongHeightToXYZ(_state.state.position[0],_state.state.position[1],_state.state.position[2]+1.0,posx,posy,posz);
    this->planeTransform->setPosition(osg::Vec3d(posx,posy,posz));

    osg::Matrixd a, b, l2ba,l2bb,l2bc, b2m;
    static float angle = 0;
    angle += 0.05;
    a.makeRotate(_state.state.position[1],osg::Vec3d(0,0,1));
    b.makeRotate(-M_PI_2-(_state.state.position[0]),osg::Vec3d(0,1,0));
    l2ba.makeRotate(_state.state.eulerangles[0],osg::Vec3d(0,0,1));
    l2bb.makeRotate(_state.state.eulerangles[1],osg::Vec3d(0,1,0));
    l2bc.makeRotate(_state.state.eulerangles[2],osg::Vec3d(1,0,0));
    b2m.makeRotate(M_PI,osg::Vec3d(0,1,0),M_PI_2,osg::Vec3d(0,0,1),0,osg::Vec3d(1,0,0));
    osg::Quat q;
    q.set(b2m*l2bc*l2bb*l2ba*b*a);
    this->planeTransform->setAttitude(q);
    this->update();

}
void Map3D::local2earth(double x_l[3],double lat, double lon, double x_e[3])
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
osgEarth::Map* Map3D::getMap()
{
    return this->mapNode->getMap();
}

void Map3D::wheelEvent(QWheelEvent *event)
{
    this->cam_range += event->delta()*0.1f;
    this->update();
}
void Map3D::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "buttons" << event->buttons();
    static bool prev_pressed = false;
    static QPointF prev_pos = QPoint(0.0f,0.0f);
    qDebug() << "buttons" << event->buttons();
    if(event->buttons() | Qt::LeftButton)
    {
        QPointF delta = event->pos() - prev_pos;
        event->pos();
        this->cam_pitch += (delta.y() / this->height())*30/57.3;
        this->cam_heading += (delta.x() / this->width())*30/57.3;
        this->update();
        qDebug() << this->cam_heading << this->cam_pitch;
    }
    prev_pos = event->pos();
}
