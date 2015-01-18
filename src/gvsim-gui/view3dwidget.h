#ifndef VIEW3DWIDGET_H
#define VIEW3DWIDGET_H

#include <QGLWidget>
#include <QGLShader>
#include <QGLShaderProgram>
#ifdef USING_QT5
#include <QOpenGLBuffer>
#else
#include <QGLBuffer>
#endif

#ifndef USING_QT5
#ifdef Q_OS_WIN32
    #define GL_GLEXT_PROTOTYPES
    #include <glext.h>
#endif
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include <QDebug>
#include <QVector>
#include <QVector3D>
#include "planestatusdata.h"
#include "math.h"
#include "simutils.h"

#define PI 3.14159

#define mat_size 30 //Must be divisible by 2
#define mat_half mat_size/2
#define kmgrad_rel 111.12
#define gradrad 0.0174532925
#define radgrad 57.2957795
#define terrain_offset 2 //margin of terrain to prevent plane from going underground(meters)
#define mps2knots 1.94384449
#define meters2feet 3.2808399

/*!
 * \brief The view3dWidget class provices an artificial horizon with 3D lines to improve look and feel
 */
class view3dWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit view3dWidget(QWidget *parent = 0);
private:
    bool enableElevation;
    bool enable3d;
    float course;
    float elevation;
    float roll;
    float speed;
    float alpha;
    float beta;
    float aglaltitude;
    GLuint texture;
    QPolygon leftWingBar,rightWingBar;
    float fovy;
    bool even;
    QVector3D Norm[mat_size][mat_size];
    //normal calculation
    QVector3D A;
    QVector3D B;
    QVector3D C;
    QVector3D D;
    QVector3D N1;
    QVector3D N2;
    float altitude;
    float altitude_3d;
    int text_multiplier;
    GLfloat sq_scale;
    GLfloat sq_scale_inv;
    GLfloat fAnisLargest;
    float kmgrad_rel_long;
    float z_gain;
    float Q[mat_size][mat_size];
    float Qmax, Qmin;
    float posx,posy,posz;
    double pos_lat;
    double pos_long;
    double max_lat;
    double min_lat;
    double max_long;
    double min_long;
    double pos_land_lat;
    double pos_land_lon;
    float  pos_land_alt;
    double center_lat;
    double center_long;
    bool reset;
    float vario_measure;
    //VBO
    int vertexLocation;
    int matrixLocation;
    int colorLocation;

    GLuint vertexVBOCount;
#ifdef USING_QT5
    QOpenGLBuffer vertexVBOBuffer;
    QOpenGLBuffer texCoordVBOBuffer;
    QOpenGLBuffer normalVBOBuffer;
    QOpenGLBuffer colorVBOBuffer;
#else
    QGLBuffer vertexVBOBuffer;
    QGLBuffer texCoordVBOBuffer;
    QGLBuffer normalVBOBuffer;
    QGLBuffer colorVBOBuffer;
#endif

    class Verts{
    public:
    float x;
    float y;
    float z;
    };
    class Norms{
    public:
    float x;
    float y;
    float z;
    };
    class Colors{
    public:
    float r;
    float g;
    float b;
    float a;
    };
    class TexCoords{
    public:
    float u;
    float v;
    };
    Verts *vboVertex;
    TexCoords *vboTexCoords;
    Norms *vboNorms;
    Colors *vboColors;

    void updateMesh();
    void resetMesh();
    void new_lat(bool north);
    void new_long(bool west);
    void addVertex(float x, float y, float z, float r, float g, float b, float u, float v, float nx, float ny, float nz, int curr);
    void drawVolume();
    void setLights();
    void paintHUD(QPainter *);
    float demaltitude(double lat, double lon);
    void paint_GL();
protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintEvent(QPaintEvent *);
    
public slots:
    void receivePackage(planeStatusData);
    void updateSettings(bool show3d,bool showElev);
    
};


#endif // VIEW3DWIDGET_H
