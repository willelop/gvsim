#include "view3dwidget.h"

view3dWidget::view3dWidget(QWidget *parent) :
    QGLWidget(parent)
{
    this->enable3d = false;
    this->enableElevation = false;

    vertexVBOBuffer.create();
    texCoordVBOBuffer.create();
    normalVBOBuffer.create();
    colorVBOBuffer.create();
#ifdef USING_QT5
        texCoordVBOBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
        normalVBOBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
        colorVBOBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
        vertexVBOBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
#else
        texCoordVBOBuffer.setUsagePattern(QGLBuffer::DynamicDraw);
        normalVBOBuffer.setUsagePattern(QGLBuffer::DynamicDraw);
        colorVBOBuffer.setUsagePattern(QGLBuffer::DynamicDraw);
        vertexVBOBuffer.setUsagePattern(QGLBuffer::DynamicDraw);
#endif
    if(vertexVBOBuffer.isCreated()) vertexVBOBuffer.release();
    if(texCoordVBOBuffer.isCreated()) texCoordVBOBuffer.release();
    if(normalVBOBuffer.isCreated()) normalVBOBuffer.release();
    if(colorVBOBuffer.isCreated()) colorVBOBuffer.release();

    leftWingBar << QPoint(-(5+18+14),-2) <<
                   QPoint(-(5+14),-2) <<
                   QPoint(-(5+14),8) <<
                   QPoint(-(5+14)-4,8) <<
                   QPoint (-(5+14)-4,2) <<
                   QPoint(-(5+18+14),2);
    rightWingBar << QPoint(+(5+18+14),-2) <<
                   QPoint(+(5+14),-2) <<
                   QPoint(+(5+14),8) <<
                   QPoint(+(5+14)+4,8) <<
                   QPoint (+(5+14)+4,2) <<
                   QPoint(+(5+18+14),2);

    int numverts = mat_size*2*(mat_size+1);
    vboVertex = new Verts[numverts];
    vboTexCoords = new TexCoords[numverts];
    vboNorms = new Norms[numverts];
    vboColors = new Colors[numverts];
    vertexVBOCount = numverts;
    z_gain = 1;
    sq_scale = 0.12; // degress
    sq_scale = 0.009; // degress
    sq_scale_inv = 1/sq_scale;
    text_multiplier = 16;
    fovy = 3.14159265/2.5;
    pos_lat = 0;
    pos_long = 0;
    center_long = 0.0f;
    center_lat = 0.0f;
    this->alpha = 0.0f;
    this->beta = 0.0f;
    this->roll = 0.0f;
    this->elevation = 0.0f;
    this->altitude = 0.0f;
    this->altitude_3d = 0.0f;
    this->course = 0.0f;
    this->speed = 0.0f;
    this->aglaltitude = 0.0f;
    reset = true;
    this->updateMesh();
}
void view3dWidget::initializeGL()
{
        int numverts = mat_size*2*(mat_size+1);
        vertexVBOCount = numverts;
        texture = bindTexture(QPixmap(":/images/grass.jpg"));
        texture = bindTexture(QPixmap(":/images/terrain2.png"));

        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fAnisLargest);

        this->makeCurrent();
        vertexVBOBuffer.create();
        texCoordVBOBuffer.create();
        normalVBOBuffer.create();
        colorVBOBuffer.create();
#ifdef USING_QT5
        texCoordVBOBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
        normalVBOBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
        colorVBOBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
        vertexVBOBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
#else
        texCoordVBOBuffer.setUsagePattern(QGLBuffer::DynamicDraw);
        normalVBOBuffer.setUsagePattern(QGLBuffer::DynamicDraw);
        colorVBOBuffer.setUsagePattern(QGLBuffer::DynamicDraw);
        vertexVBOBuffer.setUsagePattern(QGLBuffer::DynamicDraw);
#endif
        if(vertexVBOBuffer.isCreated()) vertexVBOBuffer.release();
        if(texCoordVBOBuffer.isCreated()) texCoordVBOBuffer.release();
        if(normalVBOBuffer.isCreated()) normalVBOBuffer.release();
        if(colorVBOBuffer.isCreated()) colorVBOBuffer.release();

        reset= true;
        this->updateMesh();
}
void view3dWidget::resizeGL(int w, int h)
{
    // Prevent a divde by zero
    if ( h == 0 )  h = 1;
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    // Establish the clipping volume by setting up an orthographic projection
    double aspectRatio = double( w ) / double( h );
    double verticalViewingAngle = fovy*57.3;
    gluPerspective( verticalViewingAngle, aspectRatio, 0.001, 1000.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    reset = true;
}
void view3dWidget::setLights() {

    GLfloat light_ambient[] = { 0.5, 0.5, 0.5 };
    GLfloat light_diffuse[] = { 1, 1, 1, 1 };
    GLfloat light_specular[] = { 1, 1, 1 };
    GLfloat light_position[] = { 2, 2, 0.6, 0 };

    glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv (GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);

    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
}
void view3dWidget::paintEvent(QPaintEvent *){

    this->makeCurrent();

    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    glClearColor(0, 0.6, 0.8, 1);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    glShadeModel(GL_SMOOTH);
    kmgrad_rel_long = kmgrad_rel*cos(center_lat*gradrad);
    updateMesh();
    //redraw_count++;
    posx = (pos_lat - center_lat)*kmgrad_rel;
    posy = -(pos_long - center_long)*kmgrad_rel_long;
    posz = altitude_3d*0.001*z_gain;

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING);

    if(this->enable3d)
    {
        this->paint_GL();
    }

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    glDisable(GL_DEPTH_TEST); // IMPORTANT
    glDisable( GL_TEXTURE_2D );
    glShadeModel(GL_FLAT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glDisable(GL_MULTISAMPLE);
    //  Reset OpenGL state for overlays.
    glDisable( GL_BLEND );
    glDisable( GL_LINE_SMOOTH );
    glDisable( GL_TEXTURE_2D);

    paintHUD(&painter);
    painter.end();
}
void view3dWidget::paint_GL()
{
    //end paintevent
    glEnable(GL_NORMALIZE);
    glMatrixMode( GL_MODELVIEW );

    glEnable(GL_SMOOTH);
    glLoadIdentity();
    setLights();
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    GLfloat centerx = posx+50*cos(course)*cos(elevation);
    GLfloat centery = posy+ 50*(sin(course)*cos(elevation));
    GLfloat centerz = posz+50*(-sin(elevation));

    GLfloat upx = (sin(elevation)*cos(course)*cos(roll)+sin(course)*sin(roll));
    GLfloat upy = (cos(roll)*sin(course)*sin(elevation)-cos(course)*sin(roll));
    GLfloat upz = cos(elevation)*cos(roll);
    gluLookAt(posx,posy,posz,centerx,centery,centerz,upx,upy,upz);

    glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, texture );
    glMatrixMode( GL_MODELVIEW );
    setLights();
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    drawVolume();
    glFlush();

}
/*!
 * \brief view3dWidget::receivePackage update the horizon with a new set of values
 * \param _status new set of values
 */
void view3dWidget::receivePackage(planeStatusData _status)
{
    planeStatusData innerstatus = _status;
    this->course = -innerstatus.state.eulerangles[0];
    this->elevation = -innerstatus.state.eulerangles[1];
    this->roll = innerstatus.state.eulerangles[2];
    this->pos_lat = innerstatus.state.position[0]*radgrad;
    this->pos_long = innerstatus.state.position[1]*radgrad;
    this->altitude = innerstatus.state.position[2];
    this->aglaltitude = innerstatus.state.position[2]-innerstatus.state.ground_altitude;
    if(enableElevation){
        this->altitude_3d = innerstatus.state.position[2];
    }else{
        this->altitude_3d = innerstatus.state.position[2] - innerstatus.state.ground_altitude;
    }
    this->speed = innerstatus.state.speed_air;
    this->vario_measure = -innerstatus.state.velocity_e[2];
    this->alpha = innerstatus.state.alpha_beta[0];
    this->beta = innerstatus.state.alpha_beta[1];
    this->update();

}
void view3dWidget::updateMesh()
{
    bool recalculate = false;
    int loops,i,j;
    if(reset){
        reset = false;
        recalculate = true;
        resetMesh();
    }else{
    if((pos_lat - center_lat) >= sq_scale) {
        if((pos_lat-center_lat) >= 10*sq_scale)
        {
            resetMesh();
        }
        else if((pos_lat-center_lat) >= sq_scale)
        {
            loops = floor((pos_lat-center_lat)*sq_scale_inv);
            for(i = 0; i<loops;i++)
            {
                   new_lat(true);
            }

        }
        recalculate = true;
    }else if( (pos_lat - center_lat) <= -sq_scale){
        if(pos_lat-center_lat <= -10*sq_scale)
        {
            resetMesh();
        }
        else if((pos_lat-center_lat) <= -sq_scale)
        {
            loops = floor((center_lat-pos_lat)*sq_scale_inv);
            for(i = 0; i<loops;i++)
            {
                new_lat(false);
            }
    }
        recalculate = true;
    }

    if((pos_long - center_long) >= sq_scale) {
        if((pos_long - center_long) >= 10*sq_scale)
        {
            resetMesh();
        }
        else if((pos_long - center_long) >= sq_scale)
        {
            loops = floor((pos_long - center_long)*sq_scale_inv);
            for(i = 0; i<loops;i++)
            {
                new_long(false);
            }
        }
        recalculate = true;
    }else if( (pos_long - center_long)  <= -sq_scale){
        if((pos_long - center_long) <= -10*sq_scale)
        {
            resetMesh();
        }
        else if((pos_long - center_long) <= -sq_scale)
        {
            loops = floor((center_long-pos_long)*sq_scale_inv);
            for(i = 0; i<loops;i++)
            {
                new_long(true);
            }
        }
        recalculate = true;
    }
    }

    if(recalculate){
    //THis has to be improved to only calculate the new points
    //NORMALS
    Qmax = Q[0][0];
    Qmin = Q[0][0];
    for(i = 0; i< mat_size;i++)
    {
        for(j = 0; j< mat_size;j++)
        {
            float h = Q[i][j];
            if(h > Qmax) Qmax = h;
            if(h < Qmin) Qmin = h;
            if(i>0 && j >0 && j<mat_size-1 && i < mat_size-1)
            {
                QVector3D Center(0,0,h);
                QVector3D A(sq_scale*kmgrad_rel,0,Q[i-1][j]*2);
                QVector3D B(0,-sq_scale*kmgrad_rel_long ,Q[i][j+1]*2);
                QVector3D C(-sq_scale*kmgrad_rel,0,Q[i+1][j]*2);
                QVector3D D(0,sq_scale*kmgrad_rel_long ,Q[i][j-1]*2);
                QVector3D N_1= QVector3D::normal(Center,B,A);
                QVector3D N_2= QVector3D::normal(Center,C,B);
                QVector3D N_3= QVector3D::normal(Center,D,C);
                QVector3D N_4= QVector3D::normal(Center,A,D);
                QVector3D N0 = N_1 + N_2 + N_3 + N_4;
                N0.normalize();
                Norm[i][j] = N0;
            }else{
                Norm[i][j] = QVector3D(0,0,1);
            }
        }
    }
    //UPDATE VBO
    if(!vertexVBOBuffer.isCreated() ||
       !texCoordVBOBuffer.isCreated() ||
       !normalVBOBuffer.isCreated() ||
       !colorVBOBuffer.isCreated() ) return;
    int i = 0;
    int loopX, loopY;
    for(loopX = 0; loopX < mat_size-1; loopX++ )
    {
        even = true;
        loopY = 0;
        if(loopX > 0) {
            vboVertex[i].x = sq_scale*kmgrad_rel*0.5-(loopX-mat_half+1)*sq_scale*kmgrad_rel;
            vboVertex[i].y = -(0.5-mat_half+0)*sq_scale*kmgrad_rel_long;
            vboVertex[i].z = Q[abs(loopX)][abs(loopY)]-0.001*terrain_offset;
            addVertex(sq_scale*kmgrad_rel*0.5-(loopX-mat_half+1)*sq_scale*kmgrad_rel,-(0.5-mat_half+0)*sq_scale*kmgrad_rel_long,Q[abs(loopX)][abs(loopY)]-0.001*terrain_offset,
                    0.5,0.5,0.2,
                    0,0,
                    Norm[loopX][loopY].x(),Norm[loopX][loopY].y(),Norm[loopX][loopY].z(),i++);
        }
        for(loopY = 0; loopY < mat_size; loopY++ )
        {
            even = !even;
            float ca = 0.5*(-((altitude-200)*0.001-Q[abs(loopX)][abs(loopY)])/(Qmax-Qmin) + 1);
            float cb = 0.5*(-((altitude-200)*0.001-Q[abs(loopX+1)][abs(loopY)])/(Qmax-Qmin) +1);
            float ta = ((Q[abs(loopX)][abs(loopY)]-Qmin)/(Qmax-Qmin));
            float tb = ((Q[abs(loopX+1)][abs(loopY)]-Qmin)/(Qmax-Qmin));

            addVertex(sq_scale*kmgrad_rel*0.5-(loopX-mat_half+1)*sq_scale*kmgrad_rel,-(0.5-mat_half+loopY)*sq_scale*kmgrad_rel_long,Q[abs(loopX)][abs(loopY)]-0.001*terrain_offset,
                    (ta+ca)*0.5,1-(ta+ca)*0.5,0.2,
                    0,even*text_multiplier,
                    Norm[loopX][loopY].x(),Norm[loopX][loopY].y(),Norm[loopX][loopY].z(),i++);

            addVertex(-sq_scale*kmgrad_rel*0.5-(loopX-mat_half+1)*sq_scale*kmgrad_rel,-(0.5-mat_half+loopY)*sq_scale*kmgrad_rel_long,Q[abs(loopX+1)][abs(loopY)]-0.001*terrain_offset,
                    (tb+cb)*0.5,1-(tb+cb)*0.5,0.2,
                    text_multiplier,even*text_multiplier,
                    Norm[loopX+1][loopY].x(),Norm[loopX+1][loopY].y(),Norm[loopX+1][loopY].z(),i++);
        }
        //if(loopX < mat_size -2) {
            addVertex(-sq_scale*kmgrad_rel*0.5-(loopX-mat_half+1)*sq_scale*kmgrad_rel,-(0.5-mat_half+loopY-1)*sq_scale*kmgrad_rel_long,Q[abs(loopX+1)][abs(loopY-1)]-0.001*terrain_offset,
                    0.5,0.5,0.2,
                    0,0,
                    Norm[loopX][loopY].x(),Norm[loopX][loopY].y(),Norm[loopX][loopY].z(),i++);
        //}
    }
    vertexVBOCount = i;

    vertexVBOBuffer.bind();
    vertexVBOBuffer.allocate(vboVertex,vertexVBOCount* 3 * sizeof(float));
    vertexVBOBuffer.release();

    texCoordVBOBuffer.bind();
    texCoordVBOBuffer.allocate(vboTexCoords,vertexVBOCount * 2 * sizeof(float));
    texCoordVBOBuffer.release();

    normalVBOBuffer.bind();
    normalVBOBuffer.allocate(vboNorms,vertexVBOCount* 3 * sizeof(float));
    normalVBOBuffer.release();

    colorVBOBuffer.bind();
    colorVBOBuffer.allocate(vboColors,vertexVBOCount* 4 * sizeof(float));
    colorVBOBuffer.release();


    }//END RECALCULATE
}
void view3dWidget::resetMesh()
{
    center_lat = pos_lat;
    center_long = pos_long;
    int i,j;
    double longitude,latitude;
    float altitude;
    for(i = 0; i< mat_size;i++)
    {
        longitude = center_long +(-mat_half + 0.5 +i )*sq_scale;
        for(j = 0; j< mat_size;j++)
        {
            latitude= center_lat -(-mat_half + 0.5 +j )*sq_scale;
            altitude = demaltitude(latitude, longitude);
            Q[j][i] = altitude*0.001*z_gain;
            //Q[j][i] = 0;
        }
    }
    max_lat = center_lat + (mat_half -0.5)*sq_scale;
    min_lat = center_lat - (mat_half +0.5)*sq_scale;
    max_long = center_long + (mat_half -0.5)*sq_scale;
    min_long = center_long - (mat_half +0.5)*sq_scale;
}
void view3dWidget::new_long(bool west)
{
    int i,j;
    double latitude;
    double longitude;
    float altitude;

    if(west){  // Case update West
        longitude = center_long - (mat_half +0.5)*sq_scale;
        for(i=0;i<mat_size;i++){
            for(j=mat_size-1;j>0;j--){
                Q[i][j] = Q[i][j-1];
            }
        }
        for(j = 0;j<mat_size;j++)
        {
            latitude = center_lat - (-mat_half +0.5 +j)*sq_scale;
            altitude = demaltitude(latitude,longitude);
            Q[j][0] = altitude*0.001*z_gain;
        }
        center_long = center_long - sq_scale;
    }else{ // Case update East
        longitude = center_long + (mat_half +0.5)*sq_scale;
        for(i=0;i<mat_size;i++){
            for(j=0;j<mat_size-1;j++){
                Q[i][j] = Q[i][j+1];
            }
        }
        for(j = 0;j<mat_size;j++)
        {
            latitude = center_lat - (-mat_half +0.5 +j)*sq_scale;
            altitude = demaltitude(latitude,longitude);
            Q[j][mat_size-1] = altitude*0.001*z_gain;
        }
        center_long = center_long + sq_scale;
    }
}

void view3dWidget::new_lat(bool north)
{
    int i,j;
    double latitude;
    double longitude;
    float altitude;

    if(north){  // Case update North
        latitude = center_lat + (mat_half +0.5)*sq_scale;

        for(i=0;i<mat_size;i++){
            for(j=mat_size-1;j>0;j--){
                Q[j][i] = Q[j-1][i];
            }
        }
        for(j = 0;j<mat_size;j++)
        {
            longitude = center_long + (-mat_half +0.5 +j)*sq_scale;
            altitude = demaltitude(latitude,longitude);
            Q[0][j] = altitude*0.001*z_gain;
        }
        center_lat = center_lat + sq_scale;
    }else{  // Case update South
        latitude = center_lat - (mat_half +0.5)*sq_scale;
        for(i=0;i<mat_size;i++){
            for(j=0;j<mat_size-1;j++){
                Q[j][i] = Q[j+1][i];
            }
        }
        for(j = 0;j<mat_size;j++)
        {
            longitude = center_long + (-mat_half +0.5 +j)*sq_scale;
            altitude = demaltitude(latitude,longitude);
            Q[mat_size-1][j] = altitude*0.001*z_gain;
        }
        center_lat = center_lat - sq_scale;
    }
}
void  view3dWidget::addVertex(float x, float y, float z, float r, float g, float b, float u, float v, float nx, float ny, float nz, int curr)
{
    vboVertex[curr].x = x;
    vboVertex[curr].y = y;
    vboVertex[curr].z = z;
    vboColors[curr].r = r;
    vboColors[curr].g = g;
    vboColors[curr].b = b;
//Overriding colours
    vboColors[curr].r = 0.75;
    vboColors[curr].g = 0.45;
    vboColors[curr].b = 0.1;
    vboColors[curr].a = 1;
    vboTexCoords[curr].u = u;
    vboTexCoords[curr].v = v;
    vboNorms[curr].x = nx;
    vboNorms[curr].y = ny;
    vboNorms[curr].z = nz;
}
void view3dWidget::drawVolume()
{
    GLfloat mat_ambient_esfera[] = {0.7,0.7, 0.7,1};
    GLfloat mat_diffuse_esfera[] = {0.9, 0.9, 0.9};
    GLfloat mat_specular_esfera[] = {0, 0, 0, 1.0f};
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient_esfera);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse_esfera);
    glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular_esfera);
    glMaterialf (GL_FRONT, GL_SHININESS, 10.0f);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fAnisLargest*0.9);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glPushMatrix();
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    texCoordVBOBuffer.bind();
    glTexCoordPointer(2, GL_FLOAT, 0, (char *) NULL);
    texCoordVBOBuffer.release();


    glEnableClientState(GL_COLOR_ARRAY);
    colorVBOBuffer.bind();
    glColorPointer(4, GL_FLOAT, 0, (char *) NULL);
    colorVBOBuffer.release();

    glEnableClientState(GL_NORMAL_ARRAY);
    normalVBOBuffer.bind();
    glNormalPointer(GL_FLOAT, 0, (char *) NULL);
    normalVBOBuffer.release();

    glEnableClientState(GL_VERTEX_ARRAY);
    vertexVBOBuffer.bind();
    glVertexPointer(3, GL_FLOAT, 0, (char *) NULL);
    vertexVBOBuffer.release();


    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexVBOCount);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();
    glDisable(GL_COLOR_MATERIAL);
}
void view3dWidget::paintHUD(QPainter *painter)
{
    QColor foregroundColor = Qt::white;
    QColor innerColor = Qt::black;
    QColor commandedColor = Qt::yellow;
    QColor flightDirectorColor(127,18,111);
    QColor flightDirectorColorIN(157,38,141);
    QColor groundColor(138,54,5);
    QColor indicatedColor(200,235,235);
    QFont f ("Courier", 12, QFont::Normal);
    QFont numeral = QFont ("Courier",10);
    QFont small_size = QFont("Courier",9);
    QFont tiny_size = QFont("Courier",7);
    int widgetWidth = this->width();
    int widgetHeight = this->height();;
    float currentAlt = (int)(this->altitude*meters2feet);
    float IAS_knots = this->speed*mps2knots;
//    bool paintUpperAltTriangle= true;
//    bool paintLowerAltTriangle = true;
    float horizsWidth = 0.6;
    float backscale = widgetHeight/fovy;
    float middle_of_widget_h = width()*0.5f;
    float center_of_crosshairs_height = this->height()*0.5;
    float elevation_ladder_width = widgetWidth*0.10;
    float elevation_ladder_width1 = widgetWidth*0.05;
    float vFactor = widgetWidth*horizsWidth*0.125;
    float hFactor = widgetWidth*horizsWidth*0.125;
    int arrowSize = 7;
    QPolygon arrowedSquare;
    QPolygon commandTriangle;
    QPolygon arrowL;
    arrowL << QPoint(0,0)
           << QPoint(+arrowSize,-arrowSize)
           << QPoint(+arrowSize,arrowSize);
    QPolygon arrowR;
    arrowR << QPoint(0,0)
           << QPoint(-arrowSize,-arrowSize)
           << QPoint(-arrowSize,arrowSize);
    painter->setPen(foregroundColor);
    painter->setBrush(innerColor);
    painter->setFont(f);
    QRect display_curr;
    //Ladder painting elevation
    painter->setFont(numeral);
    painter->resetTransform();
    painter->setClipRect(50,50,widgetWidth-100,widgetHeight-100);
    painter->translate(middle_of_widget_h,center_of_crosshairs_height);
    painter->rotate(-roll*radgrad); // Rotate the roll degrees
    painter->translate(0,-elevation*backscale); // Vertically move the amount of degrees plus some multiplier
    if(!this->enable3d){
        painter->setClipping(0);
        painter->setPen(groundColor);
        painter->setBrush(groundColor);
        painter->drawRect(-this->width(),0,2*this->width(),2*this->height());
        painter->setPen(foregroundColor);
    }
    painter->setClipping(1);
    for (int i =-80;i<=80;i=i+10)
    {
        float rads = i*gradrad;
        if((rads < elevation + fovy*0.5) && (rads > elevation - fovy*0.5) ){

             if(i!=0){
                 painter->drawText(-elevation_ladder_width,rads*backscale+10,QString::number(-i));
                 painter->drawLine(-elevation_ladder_width,rads*backscale,+elevation_ladder_width,rads*backscale);
             } else {
                painter->drawLine (-widgetWidth*0.5,0,+widgetWidth*0.5,0);
             }
             painter->drawLine (-elevation_ladder_width1,((i-5)*gradrad)*backscale,+elevation_ladder_width1,((i-5)*gradrad)*backscale);
        }
    }

    //ROLL
    painter->resetTransform();
    painter->setClipRect(60,30,widgetWidth-120,widgetHeight-30);

    painter->translate(this->width()*0.5,this->width()*0.5);

    painter->rotate(-this->roll*RAD2DEG_ - 90);
    painter->translate(80,0);
    painter->drawPolygon(arrowR);
    painter->translate(-80,0);
    painter->rotate(+this->roll*RAD2DEG_ + 90);
    painter->setFont(small_size);

    painter->rotate(-60);

    for (int i =-45;i<=45;i=i+15)
    {
        painter->rotate(15);
        if(std::abs(i) == 30 || std::abs(i) == 40 || i == 0) {
            painter->drawLine(0,-80,0,-95);
            if(i != 0)
            {
                painter->drawText(-2,-85,QString::number(-i));
            }
        }else{
            painter->drawLine(0,-80,0,-85);
        }

    }
        //ALTITUDE
            painter->setPen(foregroundColor);
            painter->setBrush(innerColor);
            painter->resetTransform();
            painter->setClipRect(widgetWidth,30,-51,widgetHeight-60);
            painter->setClipping(1);
            painter->translate(widgetWidth-50,widgetHeight*0.5);
            painter->drawLine(0,widgetHeight*0.4,
                             0,-widgetHeight*0.4);
            painter->setFont(small_size);

            for (int i = -6; i <7;i++)
            {
                int altitude_of_cue= ((int)(currentAlt*0.01)*100 - 100*i);
                if(altitude_of_cue >=0){
                painter->drawLine(0, vFactor*(currentAlt-altitude_of_cue)*0.01,3, vFactor*(currentAlt-altitude_of_cue)*0.01);
                QRect altTopRect(+3,-12.5  + vFactor*(currentAlt-altitude_of_cue)*0.01  ,35,25);
                painter->drawText(altTopRect,Qt::AlignRight,QString::number(altitude_of_cue));

                }

            }
            painter->setPen(commandedColor);
             painter->setPen(foregroundColor);
            display_curr = QRect(0,-10,50,20);
            QPolygon arrowedSquareSpeed;
            arrowedSquareSpeed << QPoint(5,-10)
                          << QPoint(45,-10)
                          << QPoint(45, 10)
                          << QPoint(5, 10)
                          << QPoint(0, 0);
            painter->drawPolygon(arrowedSquareSpeed);
            painter->drawText(display_curr,Qt::AlignCenter,QString::number(currentAlt));
            //VARIOMETER
            painter->resetTransform();
            painter->translate(widgetWidth-5,widgetHeight*0.5);
            painter->drawLine(0,-widgetHeight*0.3,
                             0,widgetHeight*0.3);
            //painter->setFont(small);
            for (int i = -6; i <7;i++)
            {
                int altitude_of_cue= i;
                if(abs(altitude_of_cue) == 3 || abs(altitude_of_cue) == 6)
                {
                painter->drawLine(0, vFactor*0.6*(altitude_of_cue),
                                3, vFactor*0.6*(altitude_of_cue)
                                 );

                //QRect altTopRect(0,-15 + vFactor*0.6*(altitude_of_cue)  ,11,11);
                //painter->drawText(altTopRect,Qt::AlignRight,QString::number(-altitude_of_cue));

                }
            }

            painter->setFont(numeral);
            painter->setPen(indicatedColor);
            painter->setBrush(Qt::black);
            painter->translate(0, -vFactor*0.6*(this->vario_measure));
            painter->drawPolygon(arrowL);
            painter->setPen(foregroundColor);
            //painter->translate(-15,0);
        //SPEED
        painter->setPen(foregroundColor);
        painter->setBrush(innerColor);
            painter->resetTransform();
            painter->setFont(numeral);
            painter->setClipRect(0,30,50,widgetHeight-60);
            painter->setClipping(1);
            painter->setBrush(innerColor);
            painter->resetTransform();
            painter->translate(50,widgetHeight*0.5);
            painter->drawLine(0,-widgetHeight*0.4,0,widgetHeight*0.4);

            for (int i = -6; i <7;i++)
            {
                int ias_of_cue= ((int)(IAS_knots*0.1)*10 - 10*i);
                if(ias_of_cue >=0){
                    //                    painter->drawLine(0, vFactor*(currentAlt-altitude_of_cue)*0.1,-10, vFactor*(currentAlt-altitude_of_cue)*0.1);
                    //                    QRect altTopRect(-50,-12.5 + vFactor*(currentAlt-altitude_of_cue)*0.1  ,40,25);
                    //                    painter->drawText(altTopRect,Qt::AlignRight,QString::number(altitude_of_cue));
                    painter->drawLine(0, vFactor*(IAS_knots-ias_of_cue)*0.1,-5, vFactor*(IAS_knots-ias_of_cue)*0.1);
                    QRect altTopRect(-50,-12.5  + vFactor*(IAS_knots-ias_of_cue)*0.1  ,38,25);
                    painter->drawText(altTopRect,Qt::AlignCenter,QString::number(ias_of_cue));
                }
            }
            QPolygon arrowedSquareAlt;
            display_curr = QRect(-50,-10,50,20);
            arrowedSquareAlt << QPoint(-40,-10)
                          << QPoint(-5,-10)
                          << QPoint(0,   0)
                          << QPoint(-5, 10)
                          << QPoint(-40, 10);
            painter->drawPolygon(arrowedSquareAlt);
            painter->drawText(display_curr,Qt::AlignCenter,QString::number((int)IAS_knots));
            painter->setClipping(0);
        //COURSE

                float course_show = -course*57.3;
                if(course_show < 0) {
                        course_show = course_show + 360;
                }
                    painter->setPen(foregroundColor);
                    painter->setBrush(innerColor);

                    painter->resetTransform();
                    painter->setClipRect(60,0,widgetWidth-120,60);
                    painter->setClipping(1);

                    painter->setFont(tiny_size);
                    painter->translate(widgetWidth*0.5,15);

                    for (int i = -4; i <5;i++)
                    {
                        int crs_of_cue= ((int)(course_show*0.1)*10 - 10*i);
                        painter->drawLine(-hFactor*(course_show-crs_of_cue)*0.1,-10 ,
                                        -hFactor*(course_show-crs_of_cue)*0.1, -7.5
                                         );
                        QRect altTopRect(-15 - hFactor*(course_show-crs_of_cue)*0.1, -7.5  ,30,15);
                        int crs_cue_show = crs_of_cue;
                        if(crs_of_cue < 0) {
                                crs_cue_show = crs_of_cue + 360;
                        }
                        QString showcrs;
                        if(crs_cue_show == 0) showcrs = "N";
                        else if(crs_cue_show == 90) showcrs = "E";
                        else if(crs_cue_show == 180) showcrs = "S";
                        else if(crs_cue_show == 270) showcrs = "W";
                        else showcrs = QString::number(crs_cue_show);
                        painter->drawText(altTopRect,Qt::AlignCenter,showcrs);

                    }
                    painter->setPen(foregroundColor);
                    display_curr = QRect(-20,-10,40,20);
                    painter->drawRect(display_curr);
                    painter->setFont(small_size);
                    painter->drawText(display_curr,Qt::AlignCenter,QString::number((int)course_show));
            painter->setClipping(0);

        painter->setBrush(Qt::transparent);
        //GLIDING PATH
        if(this->speed > 5.0f) {
            int circleSize = 8;
            int lineSize = 8;
            painter->resetTransform();
            painter->translate(widgetWidth*0.5,widgetHeight*0.5);
            painter->translate(this->beta*backscale,this->alpha*backscale);

            painter->drawEllipse(QPoint(0,0),circleSize,circleSize);
            painter->drawLine(0,-circleSize,0,-(circleSize+lineSize));
            painter->drawLine(circleSize,0,circleSize+lineSize,0);
            painter->drawLine(-circleSize,0,-(circleSize+lineSize),0);
        }

    //WINGS
    // Print Crosshairs.
    painter->resetTransform();
    painter->setPen(foregroundColor);
    painter->translate (middle_of_widget_h,center_of_crosshairs_height);
    painter->drawLine(-15,0,15,0);
    painter->drawLine(0,-15,0,15);
    painter->setBrush(innerColor);
    painter->drawPolygon(leftWingBar);
    painter->drawPolygon(rightWingBar);
    painter->setPen(flightDirectorColor);
    painter->setBrush(flightDirectorColorIN);
    painter->setClipping(0);

    //AGL
    painter->resetTransform();
    painter->setPen(foregroundColor);
    painter->setBrush(Qt::transparent);
    painter->translate(this->width()-80,this->height()-20);
    QRect textRect(0,0,30,15);
    painter->drawText(textRect,"AGL");
    painter->translate(30,0);
    painter->drawRect(QRect (0,0,45,15));
    painter->drawText(QRect (0,0,41,11),Qt::AlignRight,QString::number((int)(this->aglaltitude*meters2feet)));

    //LABELS
    painter->resetTransform();
    painter->setPen(foregroundColor);
    painter->setBrush(Qt::transparent);
    painter->translate(this->width()-40,10);
    painter->drawRect(textRect);
    painter->drawText(textRect,Qt::AlignCenter,"ALT");

    painter->resetTransform();
    painter->setPen(foregroundColor);
    painter->setBrush(Qt::transparent);
    painter->translate(10,10);
    painter->drawRect(textRect);
    painter->drawText(textRect,Qt::AlignCenter,"IAS");

}

void view3dWidget::updateSettings(bool show3d, bool showElev)
{
    this->enable3d = show3d;
    this->enableElevation = showElev;
    this->reset = true;
    this->update();
}
float view3dWidget::demaltitude(double lat, double lon)
{
    Q_UNUSED(lat);
    Q_UNUSED(lon);
    return 0.0f;
}
